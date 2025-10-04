/*******************Flight Controller Code***********************/

#include "MCAL_Layer/UART/MCAL_UART.h"
#include "main.h"
#include "MCAL_Layer/TIMERS/General_Purpose_Timers/MCAL_TIMERS.h"

#include "MCAL_Layer/GPIO/MCAL_GPIO.h"	

Pin_Config_t led13={
	.port_index=PORTC,
	.pin_index=PIN13,
	.mode=OUTPUT_MODE_50,
	.conf=OUTPUT_PP
	
};

extern timer4_config_t tim4;
void tim_init(void);

int main()
{
	gpio_pin_init(&led13);
	tim_init();
	timer4_init(&tim4);
	
	while(1)
	{
		gpio_pin_write(&led13,HIGH);
		timer4_delay(&tim4,1000000);
		gpio_pin_write(&led13,LOW);
		timer4_delay(&tim4,1000000);
		
	}
}


void tim_init(void)
{
	tim4.TIM4_OF_InterruptHandler=NULL;
	tim4.time_base_us=1000;
}