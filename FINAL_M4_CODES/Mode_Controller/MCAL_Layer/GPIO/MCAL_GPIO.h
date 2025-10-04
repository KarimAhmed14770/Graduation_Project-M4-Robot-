/**
@file: MCAL_GPIO.H
@author: M4_TEAM
@creation_date: 9/3/2024
*/

#ifndef MCAL_GPIO_H
#define MCAL_GPIO_H

/******************************************************INCLUDES SECTION START****************************************************/
#include "../REGISTERS_DEFINITIONS/REGISTERS_DEFINITIONS.h"
#include "../RCC_CLOCK/MCAL_RCC.h"



/******************************************************INCLUDES SECTION END  ***************************************************/



/******************************************************MACROS DEFINITION SECTION START******************************************/



/******************************************************MACROS DEFINITION SECTION END  *******************************************/




/******************************************************MACROS LIKE FUNCTIONS SECTION START************************************/

/******************************************************MACROS LIKE FUNCTIONS SECTION END  ****************************************/



/******************************************************DATA TYPES DECLARATION SECTION START***************************************/


typedef enum{
	GPIO_PORTA_INDEX,
	GPIO_PORTB_INDEX,
	GPIO_PORTC_INDEX
}Port_INDEX_t;


typedef enum{
		GPIO_PIN0_INDEX,
		GPIO_PIN1_INDEX,
		GPIO_PIN2_INDEX,
		GPIO_PIN3_INDEX,
		GPIO_PIN4_INDEX,
		GPIO_PIN5_INDEX,
		GPIO_PIN6_INDEX,
		GPIO_PIN7_INDEX,
		GPIO_PIN8_INDEX,
		GPIO_PIN9_INDEX,
		GPIO_PIN10_INDEX,
		GPIO_PIN11_INDEX,
		GPIO_PIN12_INDEX,
		GPIO_PIN13_INDEX,
		GPIO_PIN14_INDEX,
		GPIO_PIN15_INDEX
}Pin_Index_t;


typedef enum{
		GPIO_LOW,
		GPIO_HIGH
}Logic_t;

typedef enum{
		INPUT_MODE,					/*Reset state*/
		OUTPUT_MODE_10,			/*Max speed 10 MHZ*/
		OUTPUT_MODE_2,			/*Max speed 2 MHZ*/
		OUTPUT_MODE_50			/*Max speed 50 MHZ*/
}Mode_t;


typedef enum{
	/*OUTPUT MODE*/
	OUTPUT_PP=0,   	/*GENERAL PURPOSE OUTPUT PUSH PULL*/
	OUTPUT_OD,			/*GENERAL PURPOSE OUTPUT open drain */
	AFO_PP,					/*Alternating Function Output push pull*/
	AFO_OD,					/*Alternating Function Output open drain*/
	/*INPUT MODE*/
	ANALOG_MODE=0,
	FLOATING_INPUT,
	INPUT_PP,				/*INPUT pull up pull down*/
	RESERVED
	
	
}Cnf_t;



typedef struct{
	Port_INDEX_t 	port_index;
	Pin_Index_t 	pin_index;
	Mode_t 				mode;
	Cnf_t 				conf;
	Logic_t 			logic;
}Pin_Config_t;

/******************************************************DATA TYPES DECLARATION SECTION END  ***************************************/




/*********************************************FUCNCTIONS PROTOTYPES (APIS) SECTION START******************************************/

/*
@name: gpio_pin_init.
@param: a pointer to a pin object.
@sequence: the pin object must be initialized with the user desired values before passing it to the function
@description:initializes the corresponding pin as the user initialized the pin object.
@note: the user should initialize all GPIO pin objects to Logic Low in the initialization phase
if the user did initialize the pin object as high, this function initialize it as low as a protection
@return:returns 1(E_OK) IF SUCCESS, 0(E_NOT_OK) if failed.
*/

Std_ReturnType_t 	gpio_pin_init(const Pin_Config_t *pin_ptr);

/*
@name: gpio_pin_write.
@param: a pointer to a pin object that is initialized with the user values, Logic (HIGH or LOW)
@sequence: the pin object must be initialized as an output before passing it to the function
@description:writes HIGH or LOW to the correspondig GPIO pin.
@return:returns 1(E_OK) IF SUCCESS, 0(E_NOT_OK) if failed.
*/


Std_ReturnType_t	gpio_pin_write( Pin_Config_t *pin_ptr,Logic_t logic);

/*
@name: gpio_pin_read.
@param: a pointer to a pin object that is initialized with the user values, address of the variable that will hold 
the logic value of the pin
@sequence: the pin object must be initialized before passing it to the function
@description:Reads HIGH or LOW of the correspondig GPIO pin.
@return:returns 1(E_OK) IF SUCCESS, 0(E_NOT_OK) if failed,returns the read value in the given address.
*/

Std_ReturnType_t	gpio_pin_read(const Pin_Config_t *pin_ptr,uint8_t *logic);

/*
@name: gpio_pin_toggle.
@param: a pointer to a pin object that is initialized with the user values.
@sequence: the pin object must be initialized as an output before passing it to the function
@description:toggles the logic level on the corresponding gpio pin.
@return:returns 1(E_OK) IF SUCCESS, 0(E_NOT_OK) if failed,returns the read value in the given address.
*/

Std_ReturnType_t	gpio_pin_toggle( Pin_Config_t *pin_ptr);


Std_ReturnType_t gpio_test_all_port_high(void);



/******************************************************FUCNCTIONS PROTOTYPES (APIS) SECTION END  **********************************/


/******************************************************FUNCTION DOCUMENTATION EXAMPLE**********************************************/

/*
@name: function_name
@param: function parameters
@description:function_description
@return:function output
*/


/******************************************************FUNCTION DOCUMENTATION EXAMPLE**********************************************/



/******************************************************EDIT DETAILS SECTION START**************************************************/
//EXAMPLE:
/**EDIT 1
@name: karim
@edit_date: 9/3/2024 ,2:39 pm
@details:changed function XXX , line XXX from XXX to XXX
@status: problem solved 
*/


/******************************************************EDIT DETAILS SECTION END  **************************************************/



#endif

