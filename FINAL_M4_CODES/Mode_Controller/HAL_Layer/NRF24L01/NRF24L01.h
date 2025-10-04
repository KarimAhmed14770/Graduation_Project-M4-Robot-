/**
@file: NRF24L01.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef NRF24L01_H
#define NRF24L01_H

/*******************INCLUDES SECTION START************************/
#include "../../MCAL_Layer/SPI/MCAL_SPI1.h"
#include "../../MCAL_Layer/SPI/MCAL_SPI2.h"
#include "../../MCAL_Layer/TIMERS/General_Purpose_Timers/MCAL_TIMERS.h"
//using timer 3 for delay functionality
/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/

/*****spi module choose*******************/
#define SPI1_P			1
#define SPI2_P			2
#define NRF2L401_SPI_MODULE			SPI1_P

/* Memory Map */
#define CONFIG      0x00
#define EN_AA       0x01
#define EN_RXADDR   0x02
#define SETUP_AW    0x03
#define SETUP_RETR  0x04
#define RF_CH       0x05
#define RF_SETUP    0x06
#define STATUS      0x07
#define OBSERVE_TX  0x08
#define CD          0x09
#define RX_ADDR_P0  0x0A
#define RX_ADDR_P1  0x0B
#define RX_ADDR_P2  0x0C
#define RX_ADDR_P3  0x0D
#define RX_ADDR_P4  0x0E
#define RX_ADDR_P5  0x0F
#define TX_ADDR     0x10
#define RX_PW_P0    0x11
#define RX_PW_P1    0x12
#define RX_PW_P2    0x13
#define RX_PW_P3    0x14
#define RX_PW_P4    0x15
#define RX_PW_P5    0x16
#define FIFO_STATUS 0x17
#define DYNPD	   		0x1C
#define FEATURE	    0x1D

/* Instruction Mnemonics */
#define R_REGISTER    0x00
#define W_REGISTER    0x20
#define REGISTER_MASK 0x1F
#define ACTIVATE      0x50
#define R_RX_PL_WID   0x60
#define R_RX_PAYLOAD  0x61
#define W_TX_PAYLOAD  0xA0
#define W_ACK_PAYLOAD 0xA8
#define FLUSH_TX      0xE1
#define FLUSH_RX      0xE2
#define REUSE_TX_PL   0xE3
#define NOP           0xFF


/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/

/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/
typedef enum{
	RECIEVER,
	TRANSMITTER
}NRF24L01_mode_t;

typedef enum{
	PA_MIN,
	PA_LOW,
	PA_HIGH,
	PA_MAX
}NRF24L01_PA_t;

typedef enum{
	MBPS1,
	MBPS2
}NRF24L01_DR_t;

typedef struct{
	Pin_Config_t 					CE_PIN;
	NRF24L01_mode_t 			mode;
	NRF24L01_PA_t 				PA;
	NRF24L01_DR_t					data_rate;
	uint8_t								pipe_address[5];
	uint8_t 							RF_channel; //value should be fro 0 to 125
	
	#if NRF2L401_SPI_MODULE==SPI1_P
	Spi1_t spi1;
	#endif
	#if NRF2L401_SPI_MODULE==SPI2_P
	Spi2_t spi2;
	#endif
	
}NRF24L01_t;
/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t NRF24L01_init(NRF24L01_t *nrf_ptr);
Std_ReturnType_t NRF24L01_transmit(NRF24L01_t *nrf_ptr,uint8_t *data);
Std_ReturnType_t NRF24L01_is_data_available(NRF24L01_t *nrf_ptr,uint8_t pipenum);
Std_ReturnType_t NRF24L01_recieve(NRF24L01_t *nrf_ptr,uint8_t *data);
void NRF24L01_read_all_reg(NRF24L01_t *nrf_ptr,uint8_t *data,uint8_t *data1,uint8_t *data2,
														uint8_t *data3,uint8_t *data4,uint8_t *data5,
														uint8_t *data6);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/




#endif

