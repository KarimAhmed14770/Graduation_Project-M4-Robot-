/**
@file: MMCAL_SPI1.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "MCAL_SPI1.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
static Pin_Config_t SPI1_MOSI={
	.port_index=SPI1_MOSI_PORT,
	.pin_index=SPI1_MOSI_PIN,
	
};

static Pin_Config_t SPI1_MISO={
	.port_index=SPI1_MISO_PORT,
	.pin_index=SPI1_MISO_PIN,
	
};

static Pin_Config_t SPI1_SCK={
	.port_index=SPI1_SCK_PORT,
	.pin_index=SPI1_SCK_PIN,
	
};


	#if SPI1_SLAVE
	Pin_Config_t SPI1_NSS_SLAVE={
		.port_index=SPI1_NSS_PORT,
		.pin_index=SPI1_NSS_PIN,
		.mode=INPUT_MODE,
		.conf=FLOATING_INPUT
	};
	#endif


/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
static Std_ReturnType_t spi1_pins_init(Spi1_t *spi1_ptr);
static Std_ReturnType_t spi1_polarity_phase(Spi1_t *spi1_ptr);
static Std_ReturnType_t spi1_data_frame(Spi1_t *spi1_ptr);
#if SPI1_MASTER
static Std_ReturnType_t spi1_baudrate(Spi1_t *spi1_ptr);
#endif
	
	
static void SPI1_ENABLE(void);

static void SPI1_DISABLE(void);


static void SPI1_IS_MASTER(void);
static void SPI1_IS_SLAVE(void);


static void SPI1_CPOL_HIGH(void);
static void SPI1_CPOL_LOW(void);





static void SPI1_CPHA_FIRST(void);
static void SPI1_CPHA_SECOND(void);


static void SPI1_EIGHT_BIT(void);
static void SPI1_SIXTEEN_BIT(void);

static void SPI1_ENABLE_SSM(void);
static void SPI1_DISABLE_SSM(void);

static void SPI1_NSS_HIGH(void);
static void SPI1_NSS_LOW(void);

	
/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t spi1_init(Spi1_t *spi1_ptr){
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi1_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{

		RCC_SPI1_CLK_ENABLE();
		retval=spi1_pins_init(spi1_ptr);
		retval=spi1_polarity_phase(spi1_ptr);
		retval=spi1_data_frame(spi1_ptr);
		#if SPI1_MASTER
		retval=spi1_baudrate(spi1_ptr);
		#endif
		SPI1_ENABLE();
		
		
	}
	return retval;
}

Std_ReturnType_t spi1_transmit_8(uint8_t *data,uint32_t size){
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
		while(!(SPI1->SR & (SPI_SR_TXE))){}

		/*Write the data to the data register*/
		SPI1->DR = data[i];
		i++;
	}
	/*Wait until TXE is set*/
	while(!(SPI1->SR & (SPI_SR_TXE))){}

	/*Wait for BUSY flag to reset*/
	while((SPI1->SR & (SPI_SR_BSY))){}

	/*Clear OVR flag*/
	temp = SPI1->DR;
	temp = SPI1->SR;
}
	return retval;
	}


Std_ReturnType_t spi1_transmit_16(uint16_t *data,uint32_t size){
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
		while(!(SPI1->SR & (SPI_SR_TXE))){}

		/*Write the data to the data register*/
		SPI1->DR = data[i];
		i++;
	}
	/*Wait until TXE is set*/
	while(!(SPI1->SR & (SPI_SR_TXE))){}

	/*Wait for BUSY flag to reset*/
	while((SPI1->SR & (SPI_SR_BSY))){}

	/*Clear OVR flag*/
	temp = SPI1->DR;
	temp = SPI1->SR;
	}

	return retval;
}

Std_ReturnType_t spi1_recieve_8(uint8_t *data,uint32_t size){
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
		SPI1->DR =0;

		/*Wait for RXNE flag to be set*/
		while(!(READ_BIT(SPI1->SR,SPI_SR_RXNE))){}

		/*Read data from data register*/
		*(data++) = (SPI1->DR);
		size--;
	}
	}
	return retval;
}


Std_ReturnType_t spi1_recieve_16(uint16_t *data,uint32_t size){
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
		SPI1->DR =0;

		/*Wait for RXNE flag to be set*/
		while(!(READ_BIT(SPI1->SR,SPI_SR_RXNE))){}

		/*Read data from data register*/
		*(data++) = (SPI1->DR);
		size--;
	}
	}
	return retval;
}

/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/
static Std_ReturnType_t spi1_pins_init(Spi1_t *spi1_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi1_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		
				RCC_SPI1_CLK_ENABLE();
					switch(spi1_ptr->mode)
					{
						case MASTER:
						SPI1_SCK.mode=OUTPUT_MODE_50;
						SPI1_SCK.conf=AFO_PP;
						SPI1_MOSI.mode=OUTPUT_MODE_50;
						SPI1_MOSI.conf=AFO_PP;
						SPI1_MISO.mode=INPUT_MODE;
						SPI1_MISO.conf=FLOATING_INPUT;
						#if SPI1_MASTER		
						spi1_ptr->SLAVE_SELECT.mode=OUTPUT_MODE_50;
						spi1_ptr->SLAVE_SELECT.conf=OUTPUT_PP;
						#endif
						retval=gpio_pin_init(&SPI1_SCK);
						retval=gpio_pin_init(&SPI1_MOSI);
						retval=gpio_pin_init(&SPI1_MISO);
						#if SPI1_MASTER		
						retval=gpio_pin_init(&spi1_ptr->SLAVE_SELECT);
						#endif
						SPI1_ENABLE_SSM();
						SPI1_NSS_HIGH();
						SPI1_IS_MASTER();
						break;
					
						case SLAVE:
						SPI1_SCK.mode=INPUT_MODE;
						SPI1_SCK.conf=FLOATING_INPUT;
						SPI1_MOSI.mode=INPUT_MODE;
						SPI1_MOSI.conf=FLOATING_INPUT;
						SPI1_MISO.mode=OUTPUT_MODE_50;
						SPI1_MISO.conf=AFO_PP;
						retval=gpio_pin_init(&SPI1_SCK);
						retval=gpio_pin_init(&SPI1_MOSI);
						retval=gpio_pin_init(&SPI1_MISO);
						#if SPI1_SLAVE
						retval=gpio_pin_init(&SPI1_NSS_SLAVE);
						#endif
						SPI1_IS_SLAVE();
						break;
						default:retval=E_NOT_OK;
				}
					
	}
	return retval;
}

Std_ReturnType_t spi1_polarity_phase(Spi1_t *spi1_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi1_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(spi1_ptr->clock_polarity)
		{
			case IDLE_HIGH:
				SPI1_CPOL_HIGH();
			break;
			
			case IDLE_LOW:
				SPI1_CPOL_LOW();
			break;
			
			default:retval=E_NOT_OK;
		}
		
		switch(spi1_ptr->clock_phase)
		{
			case FIRST_EDGE:
				SPI1_CPHA_FIRST();
			break;
			
			case TRAILING_EDGE:
				SPI1_CPHA_SECOND();
			break;
			
			default:retval=E_NOT_OK;
		}
	}
	return retval;
}

static Std_ReturnType_t spi1_data_frame(Spi1_t *spi1_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi1_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(spi1_ptr->data_frame)
		{
			case SPI_EIGHT_BIT:
				SPI1_EIGHT_BIT();
			break;
			
			case SPI_SIXTEEN_BIT:
				SPI1_SIXTEEN_BIT();
			break;
			
			default:retval=E_NOT_OK;
		}
	}
	return retval;
}


#if SPI1_MASTER
static Std_ReturnType_t spi1_baudrate(Spi1_t *spi1_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==spi1_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		if(spi1_ptr->baud_rate==SPI1_BR_HZ_18000000 ||spi1_ptr->baud_rate==SPI1_BR_HZ_4500000 ||spi1_ptr->baud_rate==SPI1_BR_HZ_1125000
			||spi1_ptr->baud_rate==SPI1_BR_HZ_281250)
		{
			SET_BIT(SPI1->CR1,SPI_CR1_BR_0);
		}
		uint8_t temp=(spi1_ptr->baud_rate)>>1;//11<<4   110000
		SPI1->CR1&=0XFFCF;
		SPI1->CR1|= (temp<<4);		
	}
	return retval;
	
}
#endif


static void SPI1_ENABLE(void){
	(SET_BIT(SPI1->CR1,SPI_CR1_SPE));
}
static void SPI1_DISABLE(void){
	(CLEAR_BIT(SPI1->CR1,SPI_CR1_SPE));
}

static void SPI1_IS_MASTER(void){
	(SET_BIT(SPI1->CR1,SPI_CR1_MSTR));
}
	
static void SPI1_IS_SLAVE(void){
	(CLEAR_BIT(SPI1->CR1,SPI_CR1_MSTR));
}

static void SPI1_CPOL_HIGH(void){
	(SET_BIT(SPI1->CR1,SPI_CR1_CPOL));
}

static void SPI1_CPOL_LOW(void){
	(CLEAR_BIT(SPI1->CR1,SPI_CR1_CPOL));
}




static void SPI1_CPHA_FIRST(void){
	(CLEAR_BIT(SPI1->CR1,SPI_CR1_CPHA));
}
	
static void SPI1_CPHA_SECOND(void){
	(SET_BIT(SPI1->CR1,SPI_CR1_CPHA));
}

static void SPI1_EIGHT_BIT(void){
	(CLEAR_BIT(SPI1->CR1,SPI_CR1_DFF));
}
	
static void SPI1_SIXTEEN_BIT(void){
	(SET_BIT(SPI1->CR1,SPI_CR1_DFF));
}

static void SPI1_ENABLE_SSM(void){
	(SET_BIT(SPI1->CR1,SPI_CR1_SSM));
}
	
static void SPI1_DISABLE_SSM(void){
	(CLEAR_BIT(SPI1->CR1,SPI_CR1_SSM));
}

static void SPI1_NSS_HIGH(void){
	(SET_BIT(SPI1->CR1,SPI_CR1_SSI));
}
static void SPI1_NSS_LOW(void){
	(SET_BIT(SPI1->CR1,SPI_CR1_SSI));
}
/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/



