/**
@file: MCAL_SPI1.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef MCAL_SPI1_H
#define MCAL_SPI1_H

/*******************INCLUDES SECTION START************************/
#include "MCAL_SPI_GEN_CFG.h"
#include "../GPIO/MCAL_GPIO.h"

/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/
#define SPI1_MOSI_PORT 	(PORTA)
#define SPI1_MOSI_PIN		(PIN7)
#define SPI1_MISO_PORT 	(PORTA)
#define SPI1_MISO_PIN		(PIN6)
#define SPI1_SCK_PORT 	(PORTA)
#define SPI1_SCK_PIN		(PIN5)
#define SPI1_NSS_PORT 	(PORTA)
#define SPI1_NSS_PIN		(PIN4)
/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/
#define SPI1_ENABLE()					(SET_BIT_K(SPI1_CR1,SPIX_CR1_SPE))

#define SPI1_DISABLE()				(CLEAR_BIT_K(SPI1_CR1,SPIX_CR1_SPE))


#define SPI1_IS_MASTER()			(SET_BIT_K(SPI1_CR1,SPIX_CR1_MSTR))
#define SPI1_IS_SLAVE()				(CLEAR_BIT_K(SPI1_CR1,SPIX_CR1_MSTR))


#define SPI1_CPOL_HIGH()			(SET_BIT_K(SPI1_CR1,SPIX_CR1_CPOL))
#define SPI1_CPOL_LOW()				(CLEAR_BIT_K(SPI1_CR1,SPIX_CR1_CPOL))





#define SPI1_CPHA_FIRST()			(CLEAR_BIT_K(SPI1_CR1,SPIX_CR1_CPHA))
#define SPI1_CPHA_SECOND()		(SET_BIT_K(SPI1_CR1,SPIX_CR1_CPHA))


#define SPI1_EIGHT_BIT()			(CLEAR_BIT_K(SPI1_CR1,SPIX_CR1_DDF))
#define SPI1_SIXTEEN_BIT()		(SET_BIT_K(SPI1_CR1,SPIX_CR1_DDF))

#define SPI1_ENABLE_SSM()			(SET_BIT_K(SPI1_CR1,SPIX_CR1_SSM))
#define SPI1_DISABLE_SSM()		(CLEAR_BIT_K(SPI1_CR1,SPIX_CR1_SSM))

#define SPI1_NSS_HIGH()				(SET_BIT_K(SPI1_CR1,SPIX_CR1_SSI))
#define SPI1_NSS_LOW()				(SET_BIT_K(SPI1_CR1,SPIX_CR1_SSI))
/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/
typedef enum{
	SPI1_BR_HZ_36000000,//baud rate 72000000 FPLCK for spi1
	SPI1_BR_HZ_18000000,
	SPI1_BR_HZ_9000000,
	SPI1_BR_HZ_4500000,
	SPI1_BR_HZ_2250000,
	SPI1_BR_HZ_1125000,
	SPI1_BR_HZ_562500,
	SPI1_BR_HZ_281250,
}spi1_baudrate_t;


typedef struct{
	spi_mode_t 							mode;
	spi_clock_polarity_t 		clock_polarity;	
	spi_clock_phase_t				clock_phase;
	spi_data_frame_t				data_frame;
	#if SPI1_MASTER
	spi1_baudrate_t					baud_rate;
	Pin_Config_t SLAVE_SELECT;
	#endif

}Spi1_t;

/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t spi1_init(Spi1_t *spi1_ptr);
Std_ReturnType_t spi1_transmit_8(uint8_t *data,uint32_t size);
Std_ReturnType_t spi1_transmit_16(uint16_t *data,uint32_t size);
Std_ReturnType_t spi1_recieve_8(uint8_t *data,uint32_t size);
Std_ReturnType_t spi1_recieve_16(uint16_t *data,uint32_t size);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/




#endif

