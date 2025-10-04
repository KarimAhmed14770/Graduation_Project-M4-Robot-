/**
@file: MMCAL_SPI2.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "MCAL_SPI2.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
static Pin_Config_t SPI2_MOSI={
	.port_index=SPI2_MOSI_PORT,
	.pin_index=SPI2_MOSI_PIN,
	
};

static Pin_Config_t SPI2_MISO={
	.port_index=SPI2_MISO_PORT,
	.pin_index=SPI2_MISO_PIN,
	
};

static Pin_Config_t SPI2_SCK={
	.port_index=SPI2_SCK_PORT,
	.pin_index=SPI2_SCK_PIN,
	
};


	#if SPI2_SLAVE
	Pin_Config_t SPI2_NSS_SLAVE={
		.port_index=SPI2_NSS_PORT,
		.pin_index=SPI2_NSS_PIN,
		.mode=INPUT_MODE,
		.conf=FLOATING_INPUT
	};
	#endif


/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
static Std_ReturnType_t spi2_pins_init(Spi2_t *spi2_ptr);
static Std_ReturnType_t spi2_polarity_phase(Spi2_t *spi2_ptr);
static Std_ReturnType_t spi2_data_frame(Spi2_t *spi2_ptr);
#if SPI2_MASTER
static Std_ReturnType_t spi2_baudrate(Spi2_t *spi2_ptr);
#endif
	

	
/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t spi2_init(Spi2_t *spi2_ptr){
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		
		RCC_SPI2_CLK_ENABLE();
		retval=spi2_pins_init(spi2_ptr);
		retval=spi2_polarity_phase(spi2_ptr);
		retval=spi2_data_frame(spi2_ptr);
		#if SPI2_MASTER
		retval=spi2_baudrate(spi2_ptr);
		#endif
		SPI2_ENABLE();
		
		
	}
	return retval;
}

Std_ReturnType_t spi2_transmit_8(uint8_t *data,uint32_t size){
	Std_ReturnType_t retval=E_OK;
	if( NULL==data)
	{
		retval=E_NOT_OK;
	}
	else
	{
			
	uint32_t i=0;
	uint16_t temp;

	while(i<size)
	{
		/*Wait until TXE is set*/
		while(!(SPI2->SR & (SPI_SR_TXE))){}

		/*Write the data to the data register*/
		SPI2->DR = data[i];
		i++;
	}
	/*Wait until TXE is set*/
	while(!(SPI2->SR & (SPI_SR_TXE))){}

	/*Wait for BUSY flag to reset*/
	while((SPI2->SR & (SPI_SR_BSY))){}

	/*Clear OVR flag*/
	temp = SPI2->DR;
	temp = SPI2->SR;
}
	return retval;
	}


Std_ReturnType_t spi2_transmit_16(uint16_t *data,uint32_t size){
	Std_ReturnType_t retval=E_OK;
	if( NULL==data)
	{
		retval=E_NOT_OK;
	}
	else
	{
			uint32_t i=0;
	uint16_t temp;

	while(i<size)
	{
		/*Wait until TXE is set*/
		while(!(SPI2->SR & (SPI_SR_TXE))){}

		/*Write the data to the data register*/
		SPI2->DR = data[i];
		i++;
	}
	/*Wait until TXE is set*/
	while(!(SPI2->SR & (SPI_SR_TXE))){}

	/*Wait for BUSY flag to reset*/
	while((SPI2->SR & (SPI_SR_BSY))){}

	/*Clear OVR flag*/
	temp = SPI2->DR;
	temp = SPI2->SR;
	}

	return retval;
}

Std_ReturnType_t spi2_recieve_8(uint8_t *data,uint32_t size){
	Std_ReturnType_t retval=E_OK;
	if( NULL==data)
	{
		retval=E_NOT_OK;
	}
	else
	{
		while(size)
	{
		/*Send dummy data*/
		SPI2_DR =0;

		/*Wait for RXNE flag to be set*/
		while(!(READ_BIT_K(SPI2_SR,SPIX_SR_RXNE))){}

		/*Read data from data register*/
		*(data++) = (SPI2->DR);
		size--;
	}
	}
	return retval;
}


Std_ReturnType_t spi2_recieve_16(uint16_t *data,uint32_t size){
	Std_ReturnType_t retval=E_OK;
	if( NULL==data)
	{
		retval=E_NOT_OK;
	}
	else
	{
		while(size)
	{
		/*Send dummy data*/
		SPI2_DR =0;

		/*Wait for RXNE flag to be set*/
		while(!(READ_BIT_K(SPI2_SR,SPIX_SR_RXNE))){}

		/*Read data from data register*/
		*(data++) = (SPI2->DR);
		size--;
	}
	}
	return retval;
}

/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/
static Std_ReturnType_t spi2_pins_init(Spi2_t *spi2_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		
				RCC_SPI2_CLK_ENABLE();
					switch(spi2_ptr->mode)
					{
						case MASTER:
						SPI2_SCK.mode=OUTPUT_MODE_50;
						SPI2_SCK.conf=AFO_PP;
						SPI2_MOSI.mode=OUTPUT_MODE_50;
						SPI2_MOSI.conf=AFO_PP;
						SPI2_MISO.mode=INPUT_MODE;
						SPI2_MISO.conf=FLOATING_INPUT;
						#if SPI2_MASTER		
						spi2_ptr->SLAVE_SELECT.mode=OUTPUT_MODE_50;
						spi2_ptr->SLAVE_SELECT.conf=OUTPUT_PP;
						#endif
						retval=gpio_pin_init(&SPI2_SCK);
						retval=gpio_pin_init(&SPI2_MOSI);
						retval=gpio_pin_init(&SPI2_MISO);
						#if SPI2_MASTER		
						retval=gpio_pin_init(&spi2_ptr->SLAVE_SELECT);
						#endif
						SPI2_ENABLE_SSM();
						SPI2_NSS_HIGH();
						SPI2_IS_MASTER();
						break;
					
						case SLAVE:
						SPI2_SCK.mode=INPUT_MODE;
						SPI2_SCK.conf=FLOATING_INPUT;
						SPI2_MOSI.mode=INPUT_MODE;
						SPI2_MOSI.conf=FLOATING_INPUT;
						SPI2_MISO.mode=OUTPUT_MODE_50;
						SPI2_MISO.conf=AFO_PP;
						retval=gpio_pin_init(&SPI2_SCK);
						retval=gpio_pin_init(&SPI2_MOSI);
						retval=gpio_pin_init(&SPI2_MISO);
						#if SPI2_SLAVE
						retval=gpio_pin_init(&SPI2_NSS_SLAVE);
						#endif
						SPI2_IS_SLAVE();
						break;
						default:retval=E_NOT_OK;
				}
					
	}
	return retval;
}

Std_ReturnType_t spi2_polarity_phase(Spi2_t *spi2_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(spi2_ptr->clock_polarity)
		{
			case IDLE_HIGH:
				SPI2_CPOL_HIGH();
			break;
			
			case IDLE_LOW:
				SPI2_CPOL_LOW();
			break;
			
			default:retval=E_NOT_OK;
		}
		
		switch(spi2_ptr->clock_phase)
		{
			case FIRST_EDGE:
				SPI2_CPHA_FIRST();
			break;
			
			case TRAILING_EDGE:
				SPI2_CPHA_SECOND();
			break;
			
			default:retval=E_NOT_OK;
		}
	}
	return retval;
}

static Std_ReturnType_t spi2_data_frame(Spi2_t *spi2_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(spi2_ptr->data_frame)
		{
			case SPI_EIGHT_BIT:
				SPI2_EIGHT_BIT();
			break;
			
			case SPI_SIXTEEN_BIT:
				SPI2_SIXTEEN_BIT();
			break;
			
			default:retval=E_NOT_OK;
		}
	}
	return retval;
}


#if SPI2_MASTER
static Std_ReturnType_t spi2_baudrate(Spi2_t *spi2_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		if(spi2_ptr->baud_rate==SPI2_BR_HZ_9000000 ||spi2_ptr->baud_rate==SPI2_BR_HZ_2250000 ||spi2_ptr->baud_rate==SPI2_BR_HZ_562500
			||spi2_ptr->baud_rate==SPI2_BR_HZ_140625)
		{
			SET_BIT_K(SPI2_CR1,SPIX_CR1_BR0);
		}
		uint8_t temp=(spi2_ptr->baud_rate)>>1;//11<<4   110000
		SPI2_CR1&=0XFFCF;
		SPI2_CR1|= (temp<<4);		
	}
	return retval;
	
}
#endif

/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/



