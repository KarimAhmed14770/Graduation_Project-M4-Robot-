/**
@file: MCAL_GPIO.c
@author: M4_TEAM
@creation_date: 9/3/2024
*/


/******************************************************INCLUDES SECTION START****************************************************************************************/
#include "MCAL_GPIO.h"


/******************************************************INCLUDES SECTION END  ****************************************************************************************/



/******************************************************DATA TYPES DEFINITIONS SECTION START*************************************************************************/

/******************************************************DATA TYPES DEFINITIONS SECTION END  ************************************************************************/

/******************************************************STATIC FUNCTIONS PROTOTYPES START*************************************************************************/
static Std_ReturnType_t gpio_enable_clk(const Pin_Config_t *pin_ptr);
static Std_ReturnType_t gpio_afio_clk_enable(const Pin_Config_t *pin_ptr);
static Std_ReturnType_t gpio_set_pin_mode(const Pin_Config_t *pin_ptr);
static Std_ReturnType_t gpio_set_pin_conf(const Pin_Config_t *pin_ptr);
static Std_ReturnType_t  gpio_port_initialize(Port_INDEX_t port);

/******************************************************STATIC FUNCTIONS PROTOTYPES END  ************************************************************************/



/******************************************************FUCNCTIONS DEFINITIONS(APIS) SECTION START******************************************************************/

/*
@name: gpio_pin_init.
@param: a pointer to a pin object.
@sequence: the pin object must be initialized with the user desired values before passing it to the function
@description:initializes the corresponding pin as the user initialized the pin object.
@note: the user should initialize all GPIO pin objects to Logic Low in the initialization phase
if the user did initialize the pin object as high, this function initialize it as low as a protection
@return:returns 1(E_OK) IF SUCCESS, 0(E_NOT_OK) if failed.
*/

Std_ReturnType_t 	gpio_pin_init(const Pin_Config_t *pin_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==pin_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		/****************enable clk****************/
		retval=gpio_enable_clk(pin_ptr);
		/****************afio enable***************/
		retval=gpio_afio_clk_enable(pin_ptr);
		/****************set mode******************/
		if(retval)
		{
		retval=gpio_set_pin_mode(pin_ptr);
		}
		
		/***************set configuration**********/
		if(retval)
		{
		retval=gpio_set_pin_conf(pin_ptr);
		}
		/**** reset value is low logic*****/
		switch(pin_ptr->port_index){
			case GPIO_PORTA_INDEX:
				CLEAR_BIT_K(GPIOA_ODR,pin_ptr->pin_index);
			break;
			
			case GPIO_PORTB_INDEX:
				CLEAR_BIT_K(GPIOB_ODR,pin_ptr->pin_index);
			break;
			
			case GPIO_PORTC_INDEX:
				CLEAR_BIT_K(GPIOC_ODR,pin_ptr->pin_index);
			break;
			
			default:retval=E_NOT_OK;
			
		}
	}
	return retval;
}




/*
@name: gpio_pin_write.
@param: a pointer to a pin object that is initialized with the user values, Logic (HIGH or LOW)
@sequence: the pin object must be initialized as an output before passing it to the function
@description:writes HIGH or LOW to the correspondig GPIO pin.
@return:returns 1(E_OK) IF SUCCESS, 0(E_NOT_OK) if failed.
*/
Std_ReturnType_t	gpio_pin_write( Pin_Config_t *pin_ptr,Logic_t logic)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==pin_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(pin_ptr->port_index)
		{
				case GPIO_PORTA_INDEX:
					switch(logic)
					{
						case GPIO_LOW:
						CLEAR_BIT_K(GPIOA_ODR,pin_ptr->pin_index);
						break;
						
						case GPIO_HIGH:
						SET_BIT_K(GPIOA_ODR,pin_ptr->pin_index);
						break;
						
						default:retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTB_INDEX:
					switch(logic)
					{
						case GPIO_LOW:
						CLEAR_BIT_K(GPIOB_ODR,pin_ptr->pin_index);
						break;
						
						case GPIO_HIGH:
						SET_BIT_K(GPIOB_ODR,pin_ptr->pin_index);
						break;
						
						default:retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTC_INDEX:
					switch(logic)
					{
						case GPIO_LOW:
						CLEAR_BIT_K(GPIOC_ODR,pin_ptr->pin_index);
						break;
						
						case GPIO_HIGH:
						SET_BIT_K(GPIOC_ODR,pin_ptr->pin_index);
						break;
						
						default:retval=E_NOT_OK;
					}
				break;
				
				default:retval=E_NOT_OK;
		}
		if(retval)
		{
			pin_ptr->logic=logic;
		}
	}
	return retval;
}


/*
@name: gpio_pin_read.
@param: a pointer to a pin object that is initialized with the user values, address of the variable that will hold 
the logic value of the pin
@sequence: the pin object must be initialized before passing it to the function
@description:Reads HIGH or LOW of the correspondig GPIO pin.
@return:returns 1(E_OK) IF SUCCESS, 0(E_NOT_OK) if failed,returns the read value in the given address.
*/

Std_ReturnType_t	gpio_pin_read(const Pin_Config_t *pin_ptr,uint8_t *logic)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==pin_ptr ||NULL==logic)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(pin_ptr->port_index)
		{
				case GPIO_PORTA_INDEX:
					*logic=READ_BIT_K(GPIOA_IDR,pin_ptr->pin_index);
				break;
				
				case GPIO_PORTB_INDEX:
					*logic=READ_BIT_K(GPIOB_IDR,pin_ptr->pin_index);
				break;
				
				case GPIO_PORTC_INDEX:
					*logic=READ_BIT_K(GPIOC_IDR,pin_ptr->pin_index);
				break;
				
				default:retval=E_NOT_OK;
		}
	}
	return retval;
}


/*
@name: gpio_pin_toggle.
@param: a pointer to a pin object that is initialized with the user values.
@sequence: the pin object must be initialized as an output before passing it to the function
@description:toggles the logic level on the corresponding gpio pin.
@return:returns 1(E_OK) IF SUCCESS, 0(E_NOT_OK) if failed,returns the read value in the given address.
*/

Std_ReturnType_t	gpio_pin_toggle(Pin_Config_t *pin_ptr)
{

	Std_ReturnType_t retval=E_OK;
	if(NULL==pin_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(pin_ptr->port_index)
		{
				case GPIO_PORTA_INDEX:
					switch(pin_ptr->logic){
						case GPIO_LOW:
						SET_BIT_K(GPIOA_ODR,pin_ptr->pin_index);
						pin_ptr->logic=GPIO_HIGH;
						break;
						
						case GPIO_HIGH:
						CLEAR_BIT_K(GPIOA_ODR,pin_ptr->pin_index);
						pin_ptr->logic=GPIO_LOW;
						break;
						
						default: retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTB_INDEX:
					switch(pin_ptr->logic){
						case GPIO_LOW:
						SET_BIT_K(GPIOB_ODR,pin_ptr->pin_index);
						pin_ptr->logic=GPIO_HIGH;
						break;
						
						case GPIO_HIGH:
						CLEAR_BIT_K(GPIOB_ODR,pin_ptr->pin_index);
						pin_ptr->logic=GPIO_LOW;
						break;
						
						default: retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTC_INDEX:
					switch(pin_ptr->logic){
						case GPIO_LOW:
						SET_BIT_K(GPIOC_ODR,pin_ptr->pin_index);
						pin_ptr->logic=GPIO_HIGH;
						break;
						
						case GPIO_HIGH:
						CLEAR_BIT_K(GPIOC_ODR,pin_ptr->pin_index);
						pin_ptr->logic=GPIO_LOW;
						break;
						
						default: retval=E_NOT_OK;
					}
				break;
				
				default:retval=E_NOT_OK;
		}
	}
	return retval;
}


Std_ReturnType_t gpio_test_all_port_high(void){
	Std_ReturnType_t retval=0;
	RCC_IOPA_CLK_ENABLE();
	RCC_IOPB_CLK_ENABLE();
	RCC_IOPC_CLK_ENABLE();
	GPIOA_CRL=0X11111111;
	GPIOA_CRH=0X11111111;
	GPIOB_CRL=0X11111111;
	GPIOB_CRH=0X11111111;
	GPIOC_CRL=0X11111111;
	GPIOC_CRH=0X11111111;
	GPIOA_ODR=0XFFFF;
	GPIOB_ODR=0XFFFF;
	GPIOC_ODR=0XFFFF;
	return retval;
}

/******************************************************FUCNCTIONS DEFINITIONS (APIS) SECTION END  ******************************************************************/







/******************************************************STATIC FUNCTIONS PROTOTYPES START***********				****************************/
static Std_ReturnType_t gpio_enable_clk(const Pin_Config_t *pin_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==pin_ptr){
		retval=E_NOT_OK;
	}
	else
	{
		switch(pin_ptr->port_index){
			case GPIO_PORTA_INDEX:
				RCC_IOPA_CLK_ENABLE();
			break;
			
			case GPIO_PORTB_INDEX:
				RCC_IOPB_CLK_ENABLE();
			break;
			
			case GPIO_PORTC_INDEX:
				RCC_IOPC_CLK_ENABLE();
			break;
			default:retval=E_NOT_OK;
		}
	}
	return retval;
}
static Std_ReturnType_t gpio_afio_clk_enable(const Pin_Config_t *pin_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==pin_ptr){
		retval=E_NOT_OK;
	}
	else
	{
		if(pin_ptr->conf==AFO_PP || pin_ptr->conf==AFO_OD)
		{
			RCC_AFIO_CLK_ENABLE();
		}
	}
	return retval;
}

static Std_ReturnType_t gpio_set_pin_mode(const Pin_Config_t *pin_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==pin_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		if(pin_ptr->pin_index <=CRL_MAX_PIN_INDEX)
		{
			switch(pin_ptr->port_index)
			{
				case GPIO_PORTA_INDEX:
					switch(pin_ptr->mode){
						case INPUT_MODE:
							CLEAR_BIT_K(GPIOA_CRL,(pin_ptr->pin_index*4));
							CLEAR_BIT_K(GPIOA_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						case OUTPUT_MODE_10:
							SET_BIT_K(GPIOA_CRL,(pin_ptr->pin_index*4));
							CLEAR_BIT_K(GPIOA_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						case OUTPUT_MODE_2:
							CLEAR_BIT_K(GPIOA_CRL,(pin_ptr->pin_index*4));
							SET_BIT_K(GPIOA_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						case OUTPUT_MODE_50:
							SET_BIT_K(GPIOA_CRL,(pin_ptr->pin_index*4));
							SET_BIT_K(GPIOA_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						default: retval=E_NOT_OK;
					}
					break;
				
				case GPIO_PORTB_INDEX:
					switch(pin_ptr->mode){
						case INPUT_MODE:
							CLEAR_BIT_K(GPIOB_CRL,(pin_ptr->pin_index*4));
							CLEAR_BIT_K(GPIOB_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						case OUTPUT_MODE_10:
							SET_BIT_K(GPIOB_CRL,(pin_ptr->pin_index*4));
							CLEAR_BIT_K(GPIOB_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						case OUTPUT_MODE_2:
							CLEAR_BIT_K(GPIOB_CRL,(pin_ptr->pin_index*4));
							SET_BIT_K(GPIOB_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						case OUTPUT_MODE_50:
							SET_BIT_K(GPIOB_CRL,(pin_ptr->pin_index*4));
							SET_BIT_K(GPIOB_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						default: retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTC_INDEX:
					switch(pin_ptr->mode){
						case INPUT_MODE:
							CLEAR_BIT_K(GPIOC_CRL,(pin_ptr->pin_index*4));
							CLEAR_BIT_K(GPIOC_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						case OUTPUT_MODE_10:
							SET_BIT_K(GPIOC_CRL,(pin_ptr->pin_index*4));
							CLEAR_BIT_K(GPIOC_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						case OUTPUT_MODE_2:
							CLEAR_BIT_K(GPIOC_CRL,(pin_ptr->pin_index*4));
							SET_BIT_K(GPIOC_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						case OUTPUT_MODE_50:
							SET_BIT_K(GPIOC_CRL,(pin_ptr->pin_index*4));
							SET_BIT_K(GPIOC_CRL,((pin_ptr->pin_index*4)+1));
						break;
						
						default: retval=E_NOT_OK;
					}
				break;
				
				default:retval=E_NOT_OK;
			}
		}
		else if(pin_ptr->pin_index<=CRH_MAX_PIN_INDEX)
		{
			switch(pin_ptr->port_index)
			{
				case GPIO_PORTA_INDEX:
					switch(pin_ptr->mode){
						case INPUT_MODE:
							CLEAR_BIT_K(GPIOA_CRH,((pin_ptr->pin_index-8)*4));
							CLEAR_BIT_K(GPIOA_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						case OUTPUT_MODE_10:
							SET_BIT_K(GPIOA_CRH,((pin_ptr->pin_index-8)*4));
							CLEAR_BIT_K(GPIOA_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						case OUTPUT_MODE_2:
							CLEAR_BIT_K(GPIOA_CRH,((pin_ptr->pin_index-8)*4));
							SET_BIT_K(GPIOA_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						case OUTPUT_MODE_50:
							SET_BIT_K(GPIOA_CRH,((pin_ptr->pin_index-8)*4));
							SET_BIT_K(GPIOA_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						default: retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTB_INDEX:
					switch(pin_ptr->mode){
						case INPUT_MODE:
							CLEAR_BIT_K(GPIOB_CRH,((pin_ptr->pin_index-8)*4));
							CLEAR_BIT_K(GPIOB_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						case OUTPUT_MODE_10:
							SET_BIT_K(GPIOB_CRH,((pin_ptr->pin_index-8)*4));
							CLEAR_BIT_K(GPIOB_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						case OUTPUT_MODE_2:
							CLEAR_BIT_K(GPIOB_CRH,((pin_ptr->pin_index-8)*4));
							SET_BIT_K(GPIOB_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						case OUTPUT_MODE_50:
							SET_BIT_K(GPIOB_CRH,((pin_ptr->pin_index-8)*4));
							SET_BIT_K(GPIOB_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						default: retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTC_INDEX:
					switch(pin_ptr->mode){
						case INPUT_MODE:
							CLEAR_BIT_K(GPIOC_CRH,((pin_ptr->pin_index-8)*4));
							CLEAR_BIT_K(GPIOC_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						case OUTPUT_MODE_10:
							SET_BIT_K(GPIOC_CRH,((pin_ptr->pin_index-8)*4));
							CLEAR_BIT_K(GPIOC_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						case OUTPUT_MODE_2:
							CLEAR_BIT_K(GPIOC_CRH,((pin_ptr->pin_index-8)*4));
							SET_BIT_K(GPIOC_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						case OUTPUT_MODE_50:
							SET_BIT_K(GPIOC_CRH,((pin_ptr->pin_index-8)*4));
							SET_BIT_K(GPIOC_CRH,(((pin_ptr->pin_index-8)*4)+1));
						break;
						
						default: retval=E_NOT_OK;
					}
				break;
				
				default:retval=E_NOT_OK;
			}
		}
		else
		{
			retval=E_NOT_OK;
		}
	}
	return retval;
}

static Std_ReturnType_t gpio_set_pin_conf(const Pin_Config_t *pin_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==pin_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		if(pin_ptr->pin_index <=CRL_MAX_PIN_INDEX)
		{
			switch(pin_ptr->port_index)
			{
				case GPIO_PORTA_INDEX:
					if(pin_ptr->conf==ANALOG_MODE && pin_ptr->conf==OUTPUT_PP)
					{
						CLEAR_BIT_K(GPIOA_CRL,((pin_ptr->pin_index*4)+2));
						CLEAR_BIT_K(GPIOA_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else if(pin_ptr->conf==FLOATING_INPUT && pin_ptr->conf==OUTPUT_OD)
					{
						SET_BIT_K(GPIOA_CRL,((pin_ptr->pin_index*4)+2));
						CLEAR_BIT_K(GPIOA_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else if(pin_ptr->conf==INPUT_PP && pin_ptr->conf==AFO_PP)
					{
						CLEAR_BIT_K(GPIOA_CRL,((pin_ptr->pin_index*4)+2));
						SET_BIT_K(GPIOA_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else if(pin_ptr->conf==RESERVED && pin_ptr->conf==AFO_OD)
					{
						SET_BIT_K(GPIOA_CRL,((pin_ptr->pin_index*4)+2));
						SET_BIT_K(GPIOA_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else{
						retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTB_INDEX:
					if(pin_ptr->conf==ANALOG_MODE && pin_ptr->conf==OUTPUT_PP)
					{
						CLEAR_BIT_K(GPIOB_CRL,((pin_ptr->pin_index*4)+2));
						CLEAR_BIT_K(GPIOB_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else if(pin_ptr->conf==FLOATING_INPUT && pin_ptr->conf==OUTPUT_OD)
					{
						SET_BIT_K(GPIOB_CRL,((pin_ptr->pin_index*4)+2));
						CLEAR_BIT_K(GPIOB_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else if(pin_ptr->conf==INPUT_PP && pin_ptr->conf==AFO_PP)
					{
						CLEAR_BIT_K(GPIOB_CRL,((pin_ptr->pin_index*4)+2));
						SET_BIT_K(GPIOB_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else if(pin_ptr->conf==RESERVED && pin_ptr->conf==AFO_OD)
					{
						SET_BIT_K(GPIOB_CRL,((pin_ptr->pin_index*4)+2));
						SET_BIT_K(GPIOB_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else{
						retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTC_INDEX:
					if(pin_ptr->conf==ANALOG_MODE && pin_ptr->conf==OUTPUT_PP)
					{
						CLEAR_BIT_K(GPIOC_CRL,((pin_ptr->pin_index*4)+2));
						CLEAR_BIT_K(GPIOC_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else if(pin_ptr->conf==FLOATING_INPUT && pin_ptr->conf==OUTPUT_OD)
					{
						SET_BIT_K(GPIOC_CRL,((pin_ptr->pin_index*4)+2));
						CLEAR_BIT_K(GPIOC_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else if(pin_ptr->conf==INPUT_PP && pin_ptr->conf==AFO_PP)
					{
						CLEAR_BIT_K(GPIOC_CRL,((pin_ptr->pin_index*4)+2));
						SET_BIT_K(GPIOC_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else if(pin_ptr->conf==RESERVED && pin_ptr->conf==AFO_OD)
					{
						SET_BIT_K(GPIOC_CRL,((pin_ptr->pin_index*4)+2));
						SET_BIT_K(GPIOC_CRL,(((pin_ptr->pin_index*4)+1)+2));
					}
					else{
						retval=E_NOT_OK;
					}
				break;
				
				default:retval=E_NOT_OK;
			}
		}
		else if(pin_ptr->pin_index<=CRH_MAX_PIN_INDEX)
		{
			switch(pin_ptr->port_index)
			{
				case GPIO_PORTA_INDEX:
					if(pin_ptr->conf==ANALOG_MODE && pin_ptr->conf==OUTPUT_PP)
					{
						CLEAR_BIT_K(GPIOA_CRH,(((pin_ptr->pin_index-8)*4)+2));
						CLEAR_BIT_K(GPIOA_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else if(pin_ptr->conf==FLOATING_INPUT && pin_ptr->conf==OUTPUT_OD)
					{
						SET_BIT_K(GPIOA_CRH,(((pin_ptr->pin_index-8)*4)+2));
						CLEAR_BIT_K(GPIOA_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else if(pin_ptr->conf==INPUT_PP && pin_ptr->conf==AFO_PP)
					{
						CLEAR_BIT_K(GPIOA_CRH,(((pin_ptr->pin_index-8)*4)+2));
						SET_BIT_K(GPIOA_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else if(pin_ptr->conf==RESERVED && pin_ptr->conf==AFO_OD)
					{
						SET_BIT_K(GPIOA_CRH,(((pin_ptr->pin_index-8)*4)+2));
						SET_BIT_K(GPIOA_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else{
						retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTB_INDEX:
					if(pin_ptr->conf==ANALOG_MODE && pin_ptr->conf==OUTPUT_PP)
					{
						CLEAR_BIT_K(GPIOB_CRH,(((pin_ptr->pin_index-8)*4)+2));
						CLEAR_BIT_K(GPIOB_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else if(pin_ptr->conf==FLOATING_INPUT && pin_ptr->conf==OUTPUT_OD)
					{
						SET_BIT_K(GPIOB_CRH,(((pin_ptr->pin_index-8)*4)+2));
						CLEAR_BIT_K(GPIOB_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else if(pin_ptr->conf==INPUT_PP && pin_ptr->conf==AFO_PP)
					{
						CLEAR_BIT_K(GPIOB_CRH,(((pin_ptr->pin_index-8)*4)+2));
						SET_BIT_K(GPIOB_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else if(pin_ptr->conf==RESERVED && pin_ptr->conf==AFO_OD)
					{
						SET_BIT_K(GPIOB_CRH,(((pin_ptr->pin_index-8)*4)+2));
						SET_BIT_K(GPIOB_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else{
						retval=E_NOT_OK;
					}
				break;
				
				case GPIO_PORTC_INDEX:
					if(pin_ptr->conf==ANALOG_MODE && pin_ptr->conf==OUTPUT_PP)
					{
						CLEAR_BIT_K(GPIOC_CRH,(((pin_ptr->pin_index-8)*4)+2));
						CLEAR_BIT_K(GPIOC_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else if(pin_ptr->conf==FLOATING_INPUT && pin_ptr->conf==OUTPUT_OD)
					{
						SET_BIT_K(GPIOC_CRH,(((pin_ptr->pin_index-8)*4)+2));
						CLEAR_BIT_K(GPIOC_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else if(pin_ptr->conf==INPUT_PP && pin_ptr->conf==AFO_PP)
					{
						CLEAR_BIT_K(GPIOC_CRH,(((pin_ptr->pin_index-8)*4)+2));
						SET_BIT_K(GPIOC_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else if(pin_ptr->conf==RESERVED && pin_ptr->conf==AFO_OD)
					{
						SET_BIT_K(GPIOC_CRH,(((pin_ptr->pin_index-8)*4)+2));
						SET_BIT_K(GPIOC_CRH,((((pin_ptr->pin_index-8)*4)+1)+2));
					}
					else{
						retval=E_NOT_OK;
					}
				break;
				
				default:retval=E_NOT_OK;
			}
		}
		else
		{
			retval=E_NOT_OK;
		}
	}
	return retval;
}



static Std_ReturnType_t  gpio_port_initialize(Port_INDEX_t port)
{
	Std_ReturnType_t retval=E_OK;
	switch(port){
		case GPIO_PORTA_INDEX:
			GPIOA_ODR&=0x0;
		break;
			
		case GPIO_PORTB_INDEX:
			GPIOB_ODR&=0x0;
		break;
		
		case GPIO_PORTC_INDEX:
			GPIOC_ODR&=0x0;
		break;
		default:retval=E_NOT_OK ;
	}
	
	return retval;
}


/******************************************************STATIC FUNCTIONS PROTOTYPES END  **********************************************/




/******************************************************FUNCTION DOCUMENTATION EXAMPLE*******************************************************************************/

/*
@name: function_name
@param: function parameters
@description:function_description
@return:function output
*/


/******************************************************FUNCTION DOCUMENTATION EXAMPLE*******************************************************************************/



/******************************************************EDIT DETAILS SECTION START**********************************************************************************/
//EXAMPLE:
/**EDIT 1
@name: karim
@edit_date: 9/3/2024 ,2:39 pm
@details:changed function XXX , line XXX from XXX to XXX
@status: problem solved 
*/


/******************************************************EDIT DETAILS SECTION END  **********************************************************************************/





