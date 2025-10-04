/**
@file: HAL_DC_MOTOR.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "HAL_DC_MOTOR.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
#if TIMER1_MODE==TIMER_PWM_MODE
extern timer1_config_t timer1_ch1;
extern timer1_config_t timer1_ch2;
extern timer1_config_t timer1_ch3;
extern timer1_config_t timer1_ch4;
#endif

#if TIMER2_MODE==TIMER_PWM_MODE
extern timer2_config_t timer2_ch1;
extern timer2_config_t timer2_ch2;
extern timer2_config_t timer2_ch3;
extern timer2_config_t timer2_ch4;
#endif
#if TIMER3_MODE==TIMER_PWM_MODE
extern timer3_config_t timer3_ch1;
extern timer3_config_t timer3_ch2;
extern timer3_config_t timer3_ch3;
extern timer3_config_t timer3_ch4;
#endif

#if TIMER4_MODE==TIMER_PWM_MODE
extern timer4_config_t timer4_ch1;
extern timer4_config_t timer4_ch2;
extern timer4_config_t timer4_ch3;
extern timer4_config_t timer4_ch4;
#endif

/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
static Std_ReturnType_t DC_pwm_pin_init(Dc_motor_t *dc_motor_ptr);
static Std_ReturnType_t DC_move_forward(Dc_motor_t *dc_motor_ptr);
static Std_ReturnType_t DC_move_Backward(Dc_motor_t *dc_motor_ptr);
static Std_ReturnType_t DC_set_speed(Dc_motor_t *dc_motor_ptr,uint32_t speed);
/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t Dc_motor_init(Dc_motor_t *dc_motor_ptr)
{
	Std_ReturnType_t	retval=E_OK;
	if(NULL==dc_motor_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		/***************8GPIO pins initialization**************/
		dc_motor_ptr->IN1_pin.mode=OUTPUT_MODE_50;
		dc_motor_ptr->IN1_pin.conf=OUTPUT_PP;
		dc_motor_ptr->IN2_pin.mode=OUTPUT_MODE_50;
		dc_motor_ptr->IN2_pin.conf=OUTPUT_PP;
		retval=gpio_pin_init(&dc_motor_ptr->IN1_pin);
		retval=gpio_pin_init(&dc_motor_ptr->IN2_pin);
		retval=DC_pwm_pin_init(dc_motor_ptr);
	}
	return retval;
}
Std_ReturnType_t Dc_motor_move(Dc_motor_t *dc_motor_ptr,uint32_t speed) //speed variable is the input range (0-255) or(0-1023)
{
	Std_ReturnType_t	retval=E_OK;
	if(NULL==dc_motor_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		//input should be mapped to (0 20000) before writing the pwm value
		if(speed>MAX_SPEED)
		{
			speed=MAX_SPEED;//1023
		}
		
		else if(speed>ZERO_SPEED)//511
		{
			speed-=ZERO_SPEED;
			speed=map_to_range(0,512,0,20000,speed);
			retval=DC_move_forward(dc_motor_ptr);
			retval=DC_set_speed(dc_motor_ptr,speed);
		}
		else if(speed<ZERO_SPEED)
		{
			speed=ZERO_SPEED-speed;
			speed=map_to_range(0,512,0,20000,speed);
			retval=DC_move_Backward(dc_motor_ptr);
			retval=DC_set_speed(dc_motor_ptr,speed);
				
		}
		else{
						retval=Dc_motor_stop(dc_motor_ptr);
			}
		
	}
	return retval;
}
Std_ReturnType_t Dc_motor_stop(Dc_motor_t *dc_motor_ptr)
{
	Std_ReturnType_t	retval=E_OK;
	if(NULL==dc_motor_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		retval=gpio_pin_write(&dc_motor_ptr->IN1_pin,LOW);
		retval=gpio_pin_write(&dc_motor_ptr->IN2_pin,LOW);
	return retval;
}
}
/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/
static Std_ReturnType_t DC_pwm_pin_init(Dc_motor_t *dc_motor_ptr){
	Std_ReturnType_t	retval=E_OK;
	if(NULL==dc_motor_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(dc_motor_ptr->pwm_timer)
		{
			#if TIMER1_MODE==TIMER_PWM_MODE
			case DC_PWM_TIMER1:
				switch (dc_motor_ptr->pwm_channel)
				{
					case CH1:
						timer1_ch1.PWM_frequency=DC_FREQUENCY;
						retval=timer1_init(&timer1_ch1);
					break;
					
					case CH2:
						timer1_ch2.PWM_frequency=DC_FREQUENCY;
						retval=timer1_init(&timer1_ch2);
					break;
					
					case CH3:
						timer1_ch3.PWM_frequency=DC_FREQUENCY;
						retval=timer1_init(&timer1_ch3);
					break;
					
					case CH4:
						timer1_ch4.PWM_frequency=DC_FREQUENCY;
						retval=timer1_init(&timer1_ch4);
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
				
			#if TIMER2_MODE==TIMER_PWM_MODE	
			case DC_PWM_TIMER2:
				switch (dc_motor_ptr->pwm_channel)
				{
					case CH1:
						timer2_ch1.PWM_frequency=DC_FREQUENCY;
						retval=timer2_init(&timer2_ch1);
					break;
					
					case CH2:
						timer2_ch2.PWM_frequency=DC_FREQUENCY;
						retval=timer2_init(&timer2_ch2);
					break;
					
					case CH3:
						timer2_ch3.PWM_frequency=DC_FREQUENCY;
						retval=timer2_init(&timer2_ch3);
					break;
					
					case CH4:
						timer2_ch4.PWM_frequency=DC_FREQUENCY;
						retval=timer2_init(&timer2_ch4);
					break;
					default:retval=E_NOT_OK;
				}
			break;
				#endif
			
			#if TIMER3_MODE==TIMER_PWM_MODE
			case DC_PWM_TIMER3:
				switch (dc_motor_ptr->pwm_channel)
				{
					case CH1:
						timer3_ch1.PWM_frequency=DC_FREQUENCY;
						retval=timer3_init(&timer3_ch1);						
					break;
					
					case CH2:
						timer3_ch2.PWM_frequency=DC_FREQUENCY;
						retval=timer3_init(&timer3_ch2);
					break;
					
					case CH3:
						timer3_ch3.PWM_frequency=DC_FREQUENCY;
						retval=timer3_init(&timer3_ch3);
					break;
					
					case CH4:
						timer3_ch4.PWM_frequency=DC_FREQUENCY;
						retval=timer3_init(&timer3_ch4);						
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			
			#if TIMER4_MODE==TIMER_PWM_MODE

			case DC_PWM_TIMER4:
				switch (dc_motor_ptr->pwm_channel)
				{
					case CH1:
						timer4_ch1.PWM_frequency=DC_FREQUENCY;
						retval=timer4_init(&timer4_ch1);
					break;
					
					case CH2:
						timer4_ch2.PWM_frequency=DC_FREQUENCY;
						retval=timer4_init(&timer4_ch2);						
					break;
					
					case CH3:
						timer4_ch3.PWM_frequency=DC_FREQUENCY;
						retval=timer4_init(&timer4_ch3);
					break;
					
					case CH4:
						timer4_ch4.PWM_frequency=DC_FREQUENCY;
						retval=timer4_init(&timer4_ch4);						
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			
			default:retval=E_NOT_OK;
		}
	}
	return retval;
}

static Std_ReturnType_t DC_move_forward(Dc_motor_t *dc_motor_ptr)
{
	Std_ReturnType_t	retval=E_OK;
	if(NULL==dc_motor_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		#if REVERSE_DC_DIRECTIONS
		retval=gpio_pin_write(&dc_motor_ptr->IN1_pin,LOW);
		retval=gpio_pin_write(&dc_motor_ptr->IN2_pin,HIGH);
		#else
		retval=gpio_pin_write(&dc_motor_ptr->IN1_pin,HIGH);
		retval=gpio_pin_write(&dc_motor_ptr->IN2_pin,LOW);
		#endif
	}
	return retval;
}
static Std_ReturnType_t DC_move_Backward(Dc_motor_t *dc_motor_ptr)
{
	Std_ReturnType_t	retval=E_OK;
	if(NULL==dc_motor_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		#if REVERSE_DC_DIRECTIONS
		retval=gpio_pin_write(&dc_motor_ptr->IN1_pin,HIGH);
		retval=gpio_pin_write(&dc_motor_ptr->IN2_pin,LOW);
		#else
		retval=gpio_pin_write(&dc_motor_ptr->IN1_pin,LOW);
		retval=gpio_pin_write(&dc_motor_ptr->IN2_pin,HIGH);
		#endif
	}
	return retval;
}
static Std_ReturnType_t DC_set_speed(Dc_motor_t *dc_motor_ptr,uint32_t speed)
{
	Std_ReturnType_t	retval=E_OK;
	if(NULL==dc_motor_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		switch(dc_motor_ptr->pwm_timer)
		{
			#if TIMER1_MODE==TIMER_PWM_MODE
			case DC_PWM_TIMER1:
				switch (dc_motor_ptr->pwm_channel)
				{
					case CH1:
						retval=timer1_pwm_write(&timer1_ch1,speed);
					break;
					
					case CH2:
						retval=timer1_pwm_write(&timer1_ch2,speed);
					break;
					
					case CH3:
						retval=timer1_pwm_write(&timer1_ch3,speed);
					break;
					
					case CH4:
						retval=timer1_pwm_write(&timer1_ch4,speed);
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
				
			#if TIMER2_MODE==TIMER_PWM_MODE	
			case DC_PWM_TIMER2:
				switch (dc_motor_ptr->pwm_channel)
				{
					case CH1:
						retval=timer2_pwm_write(&timer2_ch1,speed);
					break;
					
					case CH2:
						retval=timer2_pwm_write(&timer2_ch2,speed);
					break;
					
					case CH3:
						retval=timer2_pwm_write(&timer2_ch3,speed);
					break;
					
					case CH4:
						retval=timer2_pwm_write(&timer2_ch4,speed);
					break;
					default:retval=E_NOT_OK;
				}
			break;
				#endif
			
			#if TIMER3_MODE==TIMER_PWM_MODE
			case DC_PWM_TIMER3:
				switch (dc_motor_ptr->pwm_channel)
				{
					case CH1:
						retval=timer3_pwm_write(&timer3_ch1,speed);						
					break;
					
					case CH2:
						retval=timer3_pwm_write(&timer3_ch2,speed);
					break;
					
					case CH3:
						retval=timer3_pwm_write(&timer3_ch3,speed);
					break;
					
					case CH4:
						retval=timer3_pwm_write(&timer3_ch4,speed);						
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			
			#if TIMER4_MODE==TIMER_PWM_MODE

			case DC_PWM_TIMER4:
				switch (dc_motor_ptr->pwm_channel)
				{
					case CH1:
						retval=timer4_pwm_write(&timer4_ch1,speed);
					break;
					
					case CH2:
						retval=timer4_pwm_write(&timer4_ch2,speed);						
					break;
					
					case CH3:
						retval=timer4_pwm_write(&timer4_ch3,speed);
					break;
					
					case CH4:
						retval=timer4_pwm_write(&timer4_ch4,speed);						
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			
			default:retval=E_NOT_OK;
		}
	}
	return retval;
}
/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/



