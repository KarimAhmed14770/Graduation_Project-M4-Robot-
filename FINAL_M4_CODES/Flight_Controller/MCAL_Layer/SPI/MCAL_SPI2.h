/**
@file: MCAL_SPI2.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef MCAL_SPI2_H
#define MCAL_SPI2_H

/*******************INCLUDES SECTION START************************/
#include "MCAL_SPI_GEN_CFG.h"
#include "../GPIO/MCAL_GPIO.h"

/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/
#define SPI2_MOSI_PORT 	(PORTB)
#define SPI2_MOSI_PIN		(PIN15)
#define SPI2_MISO_PORT 	(PORTB)
#define SPI2_MISO_PIN		(PIN14)
#define SPI2_SCK_PORT 	(PORTB)
#define SPI2_SCK_PIN		(PIN13)
#define SPI2_NSS_PORT 	(PORTB)
#define SPI2_NSS_PIN		(PIN12)
/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/

/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/
typedef enum{
	SPI2_BR_HZ_18000000,//baud rate 36000000 for spi2
	SPI2_BR_HZ_9000000,
	SPI2_BR_HZ_4500000,
	SPI2_BR_HZ_2250000,
	SPI2_BR_HZ_1125000,
	SPI2_BR_HZ_562500,
	SPI2_BR_HZ_281250,
	SPI2_BR_HZ_140625,
}spi2_baudrate_t;


typedef struct{
	#if SPI2_MASTER
	spi2_baudrate_t					baud_rate;
	Pin_Config_t SLAVE_SELECT;
	#endif
	spi_mode_t 							mode;
	spi_clock_polarity_t 		clock_polarity;
	spi_clock_phase_t				clock_phase;
	spi_data_frame_t				data_frame;


}Spi2_t;

/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t spi2_init(Spi2_t *spi2_ptr);
Std_ReturnType_t spi2_transmit_8(uint8_t *data,uint32_t size);
Std_ReturnType_t spi2_transmit_16(uint16_t *data,uint32_t size);
Std_ReturnType_t spi2_recieve_8(uint8_t *data,uint32_t size);
Std_ReturnType_t spi2_recieve_16(uint16_t *data,uint32_t size);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/




#endif

