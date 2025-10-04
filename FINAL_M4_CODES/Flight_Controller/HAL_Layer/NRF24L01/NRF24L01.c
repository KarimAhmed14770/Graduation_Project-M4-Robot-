/**
@file: NRF24L01.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "NRF24L01.h"

/********************************INCLUDES SECTION END******************************************/


static uint8_t debug;
static uint8_t config;
static uint8_t status;
//uint8_t debug5[5];
/********************************DATA TYPES DEFINITIONS SECTION START**************************/
static uint8_t fifostatus;


/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
/**************initialization functions***************/
static Std_ReturnType_t NRF24L01_pins_init(NRF24L01_t *nrf_ptr);
static Std_ReturnType_t NRF24L01_PA_DR_init(NRF24L01_t *nrf_ptr);
static Std_ReturnType_t NRF24L01_set_mode(NRF24L01_t *nrf_ptr);

/*****************enable,disable ,select,unselect the chip***********/
static Std_ReturnType_t NRF24L01_CE_disable(NRF24L01_t *nrf_ptr);
static  Std_ReturnType_t NRF24L01_CE_enable(NRF24L01_t *nrf_ptr);
static Std_ReturnType_t NRF24L01_CSN_select(NRF24L01_t *nrf_ptr);
static Std_ReturnType_t NRF24L01_CSN_unselect(NRF24L01_t *nrf_ptr);

/*****************to acces nrf2 registers***************/
static void	NRF24L01_write_reg(NRF24L01_t *nrf_ptr,uint8_t reg_address,uint8_t data);
static Std_ReturnType_t	NRF24L01_write_reg_multi(NRF24L01_t *nrf_ptr,uint8_t reg_address,uint8_t *data,uint32_t size);
static Std_ReturnType_t	NRF24L01_read_reg(NRF24L01_t *nrf_ptr,uint8_t reg_address,uint8_t *data);
static Std_ReturnType_t	NRF24L01_read_reg_multi(NRF24L01_t *nrf_ptr,uint8_t reg_address,uint8_t *data,uint32_t size);
static void NRF24L01_sendcmd (NRF24L01_t *nrf_ptr,uint8_t cmd);
static void NRF24L01_reset(NRF24L01_t *nrf_ptr,uint8_t reg_address);
/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t NRF24L01_init(NRF24L01_t *nrf_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		/*************initialize module pins************/
		retval=NRF24L01_pins_init(nrf_ptr);
		/**************disable module before condiguration********/
		retval=NRF24L01_CE_disable(nrf_ptr);
		/*******************chip unselect**************/
		NRF24L01_CSN_unselect(nrf_ptr);
		/*********************reset the module*****************/
		NRF24L01_reset(nrf_ptr,0);
		/****************88write 0 in the configuration register***/
		NRF24L01_write_reg(nrf_ptr,CONFIG,0X00);
		
		/************setup auto ACK**************/
		NRF24L01_write_reg(nrf_ptr,EN_AA,0X3F);

		// Not Enabling any data pipe right now
		NRF24L01_write_reg(nrf_ptr,EN_RXADDR,0);
		
		// 5 Bytes for the TX/RX address
		NRF24L01_write_reg(nrf_ptr,SETUP_AW,0X03);
		
		// setup retransmission
		NRF24L01_write_reg(nrf_ptr,SETUP_RETR,0x5F);

	  // will be setup during Tx or RX
		NRF24L01_write_reg(nrf_ptr,RF_CH,0x00);
		//adjust power and data rate 0X0E written for the max power and data rate
		NRF24L01_PA_DR_init(nrf_ptr);
		
		NRF24L01_set_mode(nrf_ptr);
		/***********8enable the module after configuration*********/
		retval=NRF24L01_CE_enable(nrf_ptr);
	}
	return retval;
}

Std_ReturnType_t NRF24L01_transmit(NRF24L01_t *nrf_ptr,uint8_t *data)
{
	Std_ReturnType_t retval=E_NOT_OK;
	if(NULL==nrf_ptr || NULL==data)
	{
		retval=E_NOT_OK;
	}
	else
	{
		uint8_t cmdtosend = 0;

	// select the device
		NRF24L01_CSN_select(nrf_ptr);
	// payload command
		cmdtosend = W_TX_PAYLOAD;
		spi1_transmit_8( &cmdtosend, 1);

	// send the payload
		spi1_transmit_8(data, 32);

	// Unselect the device
		NRF24L01_CSN_unselect(nrf_ptr);

		
	  NRF24L01_read_reg(nrf_ptr,FIFO_STATUS,&fifostatus);

	// check the fourth bit of FIFO_STATUS to know if the TX fifo is empty
	if ((fifostatus&(1<<4)) && (!(fifostatus&(1<<3))))
	{
		cmdtosend = FLUSH_TX;
		NRF24L01_sendcmd(nrf_ptr,cmdtosend);

		// reset FIFO_STATUS
		NRF24L01_reset (nrf_ptr,FIFO_STATUS);
		retval=E_OK;
	}
	else
	{
		retval=E_NOT_OK;
	}


	}
	return retval;
}

Std_ReturnType_t NRF24L01_is_data_available(NRF24L01_t *nrf_ptr,uint8_t pipenum,uint8_t * status_ptr)
{
	Std_ReturnType_t retval=E_NOT_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		  NRF24L01_read_reg(nrf_ptr,STATUS,&status);
		
			 
	if ((status&(1<<6))&&(status&(pipenum<<1)))
	{

		NRF24L01_write_reg(nrf_ptr,STATUS, (1<<6));
		NRF24L01_read_reg(nrf_ptr,STATUS,status_ptr);
		 retval=E_OK;
	}
	}
	*status_ptr=status;
	return retval;
}
Std_ReturnType_t NRF24L01_recieve(NRF24L01_t *nrf_ptr,uint8_t *data)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
	uint8_t cmdtosend = 0;

	// select the device
	NRF24L01_CSN_select(nrf_ptr);

	// payload command
	cmdtosend = R_RX_PAYLOAD;
	spi1_transmit_8( &cmdtosend, 1);

	// Receive the payload
	spi1_recieve_8(data, 32);

	// Unselect the device
	NRF24L01_CSN_unselect(nrf_ptr);


	cmdtosend = FLUSH_RX;
	NRF24L01_sendcmd(nrf_ptr,cmdtosend);
	}
	return retval;
}
void NRF24L01_read_all_reg(NRF24L01_t *nrf_ptr,uint8_t *data,uint8_t *data1,uint8_t *data2,
														uint8_t *data3,uint8_t *data4,uint8_t *data5,
														uint8_t *data6)
{
	//reciever mode ARDUINO NANO 
	/*
	NRF24L01_read_reg(nrf_ptr,CONFIG,&data[0]);//0x0F
	NRF24L01_read_reg(nrf_ptr,EN_AA,&data[1]);//0x3F
	NRF24L01_read_reg(nrf_ptr,EN_RXADDR,&data[2]);//0X03
	NRF24L01_read_reg(nrf_ptr,SETUP_AW,&data[3]);//0X03
	NRF24L01_read_reg(nrf_ptr,SETUP_RETR,&data[4]);//0X5F
	NRF24L01_read_reg(nrf_ptr,RF_CH,&data[5]);//0X4C
	NRF24L01_read_reg(nrf_ptr,RF_SETUP,&data[6]);//0X0F
	NRF24L01_read_reg(nrf_ptr,STATUS,&data[7]);//0X0F
	NRF24L01_read_reg(nrf_ptr,CD,&data[8]);//0X00
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P0,data1,5);//{0XCC,0XCE,0XCC,0XCE,0XCC}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P1,data2,5);//{0XC2*5}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P2,data3,5);//{0XC3*5}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P3,data4,5);//{0XC4*5}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P4,data5,5);//{0XC5*5}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P5,data6,5);//{0XC6*6}
	NRF24L01_read_reg(nrf_ptr,TX_ADDR,&data[9]);//{0XE7*5}
	NRF24L01_read_reg(nrf_ptr,RX_PW_P0,&data[10]);//0x20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P1,&data[11]);//0x20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P2,&data[12]);//0x20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P3,&data[13]);//0x20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P4,&data[14]);//0x20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P5,&data[15]);//0x20
	NRF24L01_read_reg(nrf_ptr,FIFO_STATUS,&data[16]);//0x21
	NRF24L01_read_reg(nrf_ptr,DYNPD,&data[17]);//0x00
	NRF24L01_read_reg(nrf_ptr,FEATURE,&data[18]);//0x00
	*/
	//Transmitter Mode ARDUINO NANO 
	/*
	NRF24L01_read_reg(nrf_ptr,CONFIG,&data[0]);//0X0E
	NRF24L01_read_reg(nrf_ptr,EN_AA,&data[1]);//0X3F
	NRF24L01_read_reg(nrf_ptr,EN_RXADDR,&data[2]);//0X03
	NRF24L01_read_reg(nrf_ptr,SETUP_AW,&data[3]);//0X03
	NRF24L01_read_reg(nrf_ptr,SETUP_RETR,&data[4]);//0X5F
	NRF24L01_read_reg(nrf_ptr,RF_CH,&data[5]);//0X4C
	NRF24L01_read_reg(nrf_ptr,RF_SETUP,&data[6]);//0X07
	NRF24L01_read_reg(nrf_ptr,STATUS,&data[7]);//0X0E
	NRF24L01_read_reg(nrf_ptr,CD,&data[8]);//0X00
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P0,data1,5);//{0XCC,0XCE,0XCC,0XCE,0XCC}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P1,data2,5);//{0XC2*5}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P2,data3,5);//{0XC3*5}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P3,data4,5);//{0XC4*5}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P4,data5,5);//{0XC5*5}
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P5,data6,5);//{0XC6*5}
	NRF24L01_read_reg(nrf_ptr,TX_ADDR,&data[9]);//{0XCC,0XCE,0XCC,0XCE,0XCC}
	NRF24L01_read_reg(nrf_ptr,RX_PW_P0,&data[10]);//0X20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P1,&data[11]);//0X20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P2,&data[12]);//0X20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P3,&data[13]);//0X20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P4,&data[14]);//0X20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P5,&data[15]);//0X20
	NRF24L01_read_reg(nrf_ptr,FIFO_STATUS,&data[16]);//0X11
	NRF24L01_read_reg(nrf_ptr,DYNPD,&data[17]);//0X00
	NRF24L01_read_reg(nrf_ptr,FEATURE,&data[18]);//0X00
	*/
	//Reciever mode blue pill
	/*
	NRF24L01_read_reg(nrf_ptr,CONFIG,&data[0]);//0x0F
	NRF24L01_read_reg(nrf_ptr,EN_AA,&data[1]);//0x3F
	NRF24L01_read_reg(nrf_ptr,EN_RXADDR,&data[2]);//0x03
	NRF24L01_read_reg(nrf_ptr,SETUP_AW,&data[3]);//0x03
	NRF24L01_read_reg(nrf_ptr,SETUP_RETR,&data[4]);//0x5F
	NRF24L01_read_reg(nrf_ptr,RF_CH,&data[5]);//0x4c
	NRF24L01_read_reg(nrf_ptr,RF_SETUP,&data[6]);//0x0F
	NRF24L01_read_reg(nrf_ptr,STATUS,&data[7]);//0X0E
	NRF24L01_read_reg(nrf_ptr,CD,&data[8]);//0X00
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P0,data1,5);//0XCC,CE,CC,CE,CC
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P1,data2,5);//0XC2*5
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P2,data3,5);//0XC3*5
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P3,data4,5);//0XC4*5
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P4,data5,5);//0XC5*5
	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P5,data6,5);//0XC6*5
	NRF24L01_read_reg(nrf_ptr,TX_ADDR,&data[9]);//0XE7*5
	NRF24L01_read_reg(nrf_ptr,RX_PW_P0,&data[10]);//0X20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P1,&data[11]);//0X20
	NRF24L01_read_reg(nrf_ptr,RX_PW_P2,&data[12]);//0X00
	NRF24L01_read_reg(nrf_ptr,RX_PW_P3,&data[13]);//0X00
	NRF24L01_read_reg(nrf_ptr,RX_PW_P4,&data[14]);//0X00
	NRF24L01_read_reg(nrf_ptr,RX_PW_P5,&data[15]);//0X00
	NRF24L01_read_reg(nrf_ptr,FIFO_STATUS,&data[16]);//0X11
	NRF24L01_read_reg(nrf_ptr,DYNPD,&data[17]);//0X00
	NRF24L01_read_reg(nrf_ptr,FEATURE,&data[18]);//0X00
	*/
	
}
/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/
static Std_ReturnType_t NRF24L01_pins_init(NRF24L01_t *nrf_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		#if SPI1_MASTER
		nrf_ptr->spi1.mode=MASTER;
		nrf_ptr->spi1.baud_rate=SPI1_BR_HZ_4500000;
		nrf_ptr->spi1.clock_polarity=LOW;
		nrf_ptr->spi1.clock_phase=FIRST_EDGE;
		nrf_ptr->spi1.data_frame=SPI_EIGHT_BIT;
		nrf_ptr->CE_PIN.mode=OUTPUT_MODE_50;
		nrf_ptr->CE_PIN.conf=OUTPUT_PP;
		#endif
		#if NRF2L401_SPI_MODULE==SPI1_P
		retval=spi1_init(&nrf_ptr->spi1);
		#endif
		
		#if NRF2L401_SPI_MODULE==SPI2_P
		retval=spi1_init(&nrf_ptr->spi2);
		#endif

}
		return retval;
	}

 static Std_ReturnType_t NRF24L01_CE_disable(NRF24L01_t *nrf_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		retval=gpio_pin_write(&nrf_ptr->CE_PIN,LOW);

	}
		return retval;
}
 static Std_ReturnType_t NRF24L01_CE_enable(NRF24L01_t *nrf_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		retval=gpio_pin_write(&nrf_ptr->CE_PIN,HIGH);

	}
		return retval;
}

static Std_ReturnType_t NRF24L01_CSN_select(NRF24L01_t *nrf_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		#if SPI1_MASTER
		retval=gpio_pin_write(&nrf_ptr->spi1.SLAVE_SELECT,LOW);
		#endif
	}
		return retval;
}

static Std_ReturnType_t NRF24L01_CSN_unselect(NRF24L01_t *nrf_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		#if SPI1_MASTER
		retval=gpio_pin_write(&nrf_ptr->spi1.SLAVE_SELECT,HIGH);
		#endif
	}
		return retval;
}


static void	NRF24L01_write_reg(NRF24L01_t *nrf_ptr,uint8_t reg_address,uint8_t data)
{
	reg_address=reg_address|(1<<5);
	uint8_t buf[2]={reg_address,data};
	//select the device
	NRF24L01_CSN_select(nrf_ptr);
	#if NRF2L401_SPI_MODULE==SPI1_P
	spi1_transmit_8(buf,sizeof(buf));

	#endif
	
	#if NRF2L401_SPI_MODULE==SPI2_P
	spi2_transmit(buf,sizeof(buf));

	#endif
	NRF24L01_CSN_unselect(nrf_ptr);

}



static Std_ReturnType_t	NRF24L01_write_reg_multi(NRF24L01_t *nrf_ptr,uint8_t reg_address,uint8_t *data,uint32_t size)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr||NULL==data)
	{
		retval=E_NOT_OK;
	}
	else
	{
		uint8_t buf;
		reg_address|=1<<5;
		buf=reg_address;
		NRF24L01_CSN_select(nrf_ptr);
		
		#if NRF2L401_SPI_MODULE==SPI1_P
		spi1_transmit_8(&buf,1);
		spi1_transmit_8(data,size);

	#endif
	
	#if NRF2L401_SPI_MODULE==SPI2_P
		spi2_transmit_8(&buf,1);
		spi2_transmit_8(data,size);
	
	#endif
		
		NRF24L01_CSN_unselect(nrf_ptr);

	}
		return retval;
}

 static Std_ReturnType_t	NRF24L01_read_reg(NRF24L01_t *nrf_ptr,uint8_t reg_address,uint8_t *data)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr ||NULL==data)
	{
		retval=E_NOT_OK;
	}
	else
	{

	// Pull the CS Pin LOW to select the device
		NRF24L01_CSN_select(nrf_ptr);

		#if NRF2L401_SPI_MODULE==SPI1_P
			spi1_transmit_8(&reg_address,1);
			spi1_recieve_8(data,1);

		#endif
	
	#if NRF2L401_SPI_MODULE==SPI2_P
			spi1_transmit_8(&reg_address,1);
			spi1_recieve_8(data,1);
	
	#endif
	
	// Pull the CS HIGH to release the device
		NRF24L01_CSN_unselect(nrf_ptr);

	}
	return retval;
}

 static Std_ReturnType_t	NRF24L01_read_reg_multi(NRF24L01_t *nrf_ptr,uint8_t reg_address,uint8_t *data,uint32_t size)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr||NULL==data)
	{
		retval=E_NOT_OK;
	}
	else
	{
		
		NRF24L01_CSN_select(nrf_ptr);

		#if NRF2L401_SPI_MODULE==SPI1_P
			spi1_transmit_8(&reg_address, 1);
			spi1_recieve_8(data, size);

		#endif
	
	#if NRF2L401_SPI_MODULE==SPI2_P
			spi2_transmit(&reg_address, 1);
			spi2_recieve(data, size);
	
	#endif
	
	// Pull the CS HIGH to release the device
		NRF24L01_CSN_unselect(nrf_ptr);

	}
		return retval;
}


static Std_ReturnType_t NRF24L01_PA_DR_init(NRF24L01_t *nrf_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		uint8_t reg_value=0;
		reg_value|=nrf_ptr->data_rate<<3;
		reg_value|=nrf_ptr->PA<<1;
		reg_value|=0x01;//for enabling LNA
		NRF24L01_write_reg(nrf_ptr,RF_SETUP,reg_value);
		
	}
		return retval;
}


static Std_ReturnType_t NRF24L01_set_mode(NRF24L01_t *nrf_ptr)//i was here
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==nrf_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(nrf_ptr->mode)
		{
			case RECIEVER:


				NRF24L01_reset (nrf_ptr,STATUS);
				// enabling data pipe 1,2
				uint8_t en_rxaddr = 0x03;
				NRF24L01_write_reg (nrf_ptr,EN_RXADDR, en_rxaddr);
				NRF24L01_write_reg (nrf_ptr,RF_CH, nrf_ptr->RF_channel);  // select the channel
				NRF24L01_write_reg_multi(nrf_ptr,RX_ADDR_P1, nrf_ptr->pipe_address, 5);  // Write the recieving address in pipe0
				//set payload to 32 bytes for pipe 0 and pipe 1
				NRF24L01_write_reg (nrf_ptr,RX_PW_P0, 0x20);  
				NRF24L01_write_reg (nrf_ptr,RX_PW_P1, 0x20); 
				// power up the device in Rx mode
				config = NRF24L01_read_reg(nrf_ptr,CONFIG,&config);
				config = config | 0x0F;
				NRF24L01_write_reg (nrf_ptr,CONFIG, config);

			
			break;
			
			case TRANSMITTER:
				//writing channel
				NRF24L01_write_reg(nrf_ptr,RF_CH,nrf_ptr->RF_channel);
			//writing the pipe address which the module will transmit on
				NRF24L01_write_reg_multi(nrf_ptr,TX_ADDR,nrf_ptr->pipe_address,5);
				config=0;
				//reading the config register value
				NRF24L01_read_reg(nrf_ptr,CONFIG,&config);
				config|=0X02;
				//powering it up and set it in transmitter mode
				NRF24L01_write_reg(nrf_ptr,CONFIG,config);
			break;
			
			default:retval=E_NOT_OK;
		}
		
	}
		return retval;
}
static void NRF24L01_sendcmd (NRF24L01_t *nrf_ptr,uint8_t cmd){
		NRF24L01_CSN_select(nrf_ptr);
		#if NRF2L401_SPI_MODULE==SPI1_P
			spi1_transmit_8(&cmd, 1);
		#endif
	
	#if NRF2L401_SPI_MODULE==SPI2_P
			spi2_transmit(&cmd, 1);
	#endif
	
	// Pull the CS HIGH to release the device
		NRF24L01_CSN_unselect(nrf_ptr);
}


static void NRF24L01_reset(NRF24L01_t *nrf_ptr,uint8_t reg_address)
{
	if (reg_address == STATUS)
	{
		NRF24L01_write_reg(nrf_ptr,STATUS,0X00);
	}

	else if (reg_address == FIFO_STATUS)
	{
		NRF24L01_write_reg(nrf_ptr,FIFO_STATUS,0x11);
	}

	else {

				NRF24L01_write_reg(nrf_ptr,CONFIG,0x08);
				//NRF24L01_read_reg(nrf_ptr,CONFIG,&debug);
		
	
				NRF24L01_write_reg(nrf_ptr,EN_AA,0x3F);
				//NRF24L01_read_reg(nrf_ptr,EN_AA,&debug);
				
				NRF24L01_write_reg(nrf_ptr,EN_RXADDR,0x03);
				//NRF24L01_read_reg(nrf_ptr,EN_AA,&debug);
		
				NRF24L01_write_reg(nrf_ptr,SETUP_AW,0x03);
				//NRF24L01_read_reg(nrf_ptr,SETUP_AW,&debug);
		
				NRF24L01_write_reg(nrf_ptr,SETUP_RETR,0x03);
			//	NRF24L01_read_reg(nrf_ptr,SETUP_RETR,&debug);
		
				NRF24L01_write_reg(nrf_ptr,RF_CH,0x02);
				//NRF24L01_read_reg(nrf_ptr,RF_CH,&debug);
		
				NRF24L01_write_reg(nrf_ptr,RF_SETUP,0x0E);
			//	NRF24L01_read_reg(nrf_ptr,RF_SETUP,&debug);
				
				NRF24L01_write_reg(nrf_ptr,STATUS,0x00);
			//	NRF24L01_read_reg(nrf_ptr,STATUS,&debug);
				
				NRF24L01_write_reg(nrf_ptr,OBSERVE_TX,0x00);
			//	NRF24L01_read_reg(nrf_ptr,OBSERVE_TX,&debug);
				
				NRF24L01_write_reg(nrf_ptr,CD,0x00);
			//	NRF24L01_read_reg(nrf_ptr,CD,&debug);
		
				uint8_t rx_addr_p0_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
				NRF24L01_write_reg_multi(nrf_ptr,RX_ADDR_P0,rx_addr_p0_def,5);
			//	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P0,debug5,5);
				
				uint8_t rx_addr_p1_def[5] = {0xC2, 0xC2, 0xC2, 0xC2, 0xC2};
				NRF24L01_write_reg_multi(nrf_ptr,RX_ADDR_P1,rx_addr_p1_def,5);
			//	NRF24L01_read_reg_multi(nrf_ptr,RX_ADDR_P1,debug5,5);
			
				NRF24L01_write_reg(nrf_ptr,RX_ADDR_P2,0xC3);
			//	NRF24L01_read_reg(nrf_ptr,RX_ADDR_P2,&debug);
				
				NRF24L01_write_reg(nrf_ptr,RX_ADDR_P3,0xC4);
			//	NRF24L01_read_reg(nrf_ptr,RX_ADDR_P3,&debug);
	
				NRF24L01_write_reg(nrf_ptr,RX_ADDR_P4,0xC5);
			//	NRF24L01_read_reg(nrf_ptr,RX_ADDR_P4,&debug);
	
				NRF24L01_write_reg(nrf_ptr,RX_ADDR_P5,0xC6);
			//	NRF24L01_read_reg(nrf_ptr,RX_ADDR_P5,&debug);
				
				uint8_t tx_addr_def[5] = {0xE7, 0xE7, 0xE7, 0xE7, 0xE7};
				NRF24L01_write_reg_multi(nrf_ptr,TX_ADDR,tx_addr_def,5);
				
				NRF24L01_write_reg(nrf_ptr,RX_PW_P0,0);
				NRF24L01_write_reg(nrf_ptr,RX_PW_P1,0);
				NRF24L01_write_reg(nrf_ptr,RX_PW_P2,0);
				NRF24L01_write_reg(nrf_ptr,RX_PW_P3,0);
				NRF24L01_write_reg(nrf_ptr,RX_PW_P4,0);
				NRF24L01_write_reg(nrf_ptr,RX_PW_P5,0);
				NRF24L01_write_reg(nrf_ptr,FIFO_STATUS,0x11);
				NRF24L01_write_reg(nrf_ptr,DYNPD,0);
				NRF24L01_write_reg(nrf_ptr,FEATURE,0);
	}
}
/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/



