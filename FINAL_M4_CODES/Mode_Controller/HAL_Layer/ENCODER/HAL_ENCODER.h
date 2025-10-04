/**
@file: HAL_ENCODER.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef HAL_ENCODER_H
#define HAL_ENCODER_H

/*******************INCLUDES SECTION START************************/
#include "../../MCAL_Layer/GPIO/MCAL_GPIO.h"
#include "../../MCAL_Layer/INTERRUPT/MCAL_EXTERNAL_INTERRUPTS.h"
/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/

/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/

/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/
typedef struct{
	void (*Encoder_Interrupt)(void);
	Port_INDEX_t 	gpio_port_index;
	Pin_Index_t 	gpio_pin_index;
	Port_INDEX_t 	Exti_port_index;
	Pin_Index_t 	Exti_pin_index;
}Encoder_t;
/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t Encoder_init(Encoder_t *Encoder_ptr);
Std_ReturnType_t Encoder_deinit(Encoder_t *Encoder_ptr);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/




#endif

