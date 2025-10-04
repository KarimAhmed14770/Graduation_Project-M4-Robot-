/**
@file: HAL_SERVO.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "HAL_SERVO.h"
#include "../../Softwares/M4_Softwares.h"
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


#if TIMER1_MODE==TIMER_TIMER_MODE
extern timer1_config_t tim1;
void tim1_isr(void);


#endif

#if TIMER2_MODE==TIMER_TIMER_MODE
extern timer2_config_t tim2;
void tim2_isr(void);
#endif

#if TIMER3_MODE==TIMER_TIMER_MODE
extern timer3_config_t tim3;
//void tim3_isr(void);


#endif

#if TIMER4_MODE==TIMER_TIMER_MODE
extern timer4_config_t tim4;
//void tim4_isr(void);
#endif

/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
static Std_ReturnType_t servo_adjust_polarity(Servo_t *servo_ptr);
static Std_ReturnType_t servo_adjust_duty_cycle(Servo_t *servo_ptr);
/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t 	servo_init(Servo_t *servo_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==servo_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		servo_adjust_polarity(servo_ptr);
		servo_adjust_duty_cycle(servo_ptr);
		switch(servo_ptr->pwm_timer)
		{
			#if TIMER1_MODE==TIMER_PWM_MODE
			case TIMER1_PWM:
				switch(servo_ptr->pwm_channel)
				{
					case PWM_CH1:
						timer1_ch1.PWM_frequency=SERVO_FREQUENCY;
						timer1_ch1.duty_cycle=servo_ptr->duty_cycle;
						retval=timer1_init(&timer1_ch1);
						retval=timer1_pwm_write(&timer2_ch1,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH2:
						timer1_ch2.PWM_frequency=SERVO_FREQUENCY;
						timer1_ch2.duty_cycle=servo_ptr->duty_cycle;
						retval=timer1_init(&timer1_ch2);
						retval=timer1_pwm_write(&timer2_ch2,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH3:
						timer1_ch3.PWM_frequency=SERVO_FREQUENCY;
						timer1_ch3.duty_cycle=servo_ptr->duty_cycle;
						retval=timer1_init(&timer1_ch3);
						retval=timer1_pwm_write(&timer2_ch3,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH4:
						timer1_ch4.PWM_frequency=SERVO_FREQUENCY;
						timer1_ch4.duty_cycle=servo_ptr->duty_cycle;
						retval=timer1_init(&timer1_ch4);
						retval=timer1_pwm_write(&timer2_ch4,servo_ptr->duty_cycle);
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			
			#if TIMER2_MODE==TIMER_PWM_MODE
			case TIMER2_PWM:
					switch(servo_ptr->pwm_channel)
				{
					case PWM_CH1:
						timer2_ch1.PWM_frequency=SERVO_FREQUENCY;
						timer2_ch1.duty_cycle=servo_ptr->duty_cycle;
						retval=timer2_init(&timer2_ch1);
						retval=timer2_pwm_write(&timer2_ch1,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH2:
						timer2_ch2.PWM_frequency=SERVO_FREQUENCY;
						timer2_ch2.duty_cycle=servo_ptr->duty_cycle;
						retval=timer2_init(&timer2_ch2);
						retval=timer2_pwm_write(&timer2_ch2,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH3:
						timer2_ch3.PWM_frequency=SERVO_FREQUENCY;
						timer2_ch3.duty_cycle=servo_ptr->duty_cycle;
						retval=timer2_init(&timer2_ch3);
						retval=timer2_pwm_write(&timer2_ch3,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH4:
						timer2_ch4.PWM_frequency=SERVO_FREQUENCY;
						timer2_ch4.duty_cycle=servo_ptr->duty_cycle;
						retval=timer2_init(&timer2_ch4);
						retval=timer2_pwm_write(&timer2_ch4,servo_ptr->duty_cycle);
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			#if TIMER3_MODE==TIMER_PWM_MODE
			case TIMER3_PWM:
					switch(servo_ptr->pwm_channel)
				{
					case PWM_CH1:
						timer3_ch1.PWM_frequency=SERVO_FREQUENCY;
						timer3_ch1.duty_cycle=servo_ptr->duty_cycle;	
						retval=timer3_init(&timer3_ch1);
						retval=timer3_pwm_write(&timer3_ch1,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH2:
						timer3_ch2.PWM_frequency=SERVO_FREQUENCY;
						timer3_ch2.duty_cycle=servo_ptr->duty_cycle;
						retval=timer3_init(&timer3_ch2);
						retval=timer3_pwm_write(&timer3_ch2,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH3:
						timer3_ch3.PWM_frequency=SERVO_FREQUENCY;
						timer3_ch3.duty_cycle=servo_ptr->duty_cycle;
						retval=timer3_init(&timer3_ch3);
						retval=timer3_pwm_write(&timer3_ch3,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH4:
						timer3_ch4.PWM_frequency=SERVO_FREQUENCY;
						timer3_ch4.duty_cycle=servo_ptr->duty_cycle;
						retval=timer3_init(&timer3_ch4);
						retval=timer3_pwm_write(&timer3_ch4,servo_ptr->duty_cycle);
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			
			#if TIMER4_MODE==TIMER_PWM_MODE
			case TIMER4_PWM:
					switch(servo_ptr->pwm_channel)
				{
					case PWM_CH1:
						timer4_ch1.PWM_frequency=SERVO_FREQUENCY;
						timer4_ch1.duty_cycle=servo_ptr->duty_cycle;
						retval=timer4_init(&timer4_ch1);
						retval=timer4_pwm_write(&timer4_ch1,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH2:
						timer4_ch2.PWM_frequency=SERVO_FREQUENCY;
						timer4_ch2.duty_cycle=servo_ptr->duty_cycle;
						retval=timer4_init(&timer4_ch2);
						retval=timer4_pwm_write(&timer4_ch2,servo_ptr->duty_cycle);

					break;
					
					case PWM_CH3:
						timer4_ch3.PWM_frequency=SERVO_FREQUENCY;
						timer4_ch3.duty_cycle=servo_ptr->duty_cycle;
						retval=timer4_init(&timer4_ch3);
						retval=timer4_pwm_write(&timer4_ch3,servo_ptr->duty_cycle);

					break;
					
					case PWM_CH4:
						timer4_ch4.PWM_frequency=SERVO_FREQUENCY;
						timer4_ch4.duty_cycle=servo_ptr->duty_cycle;
						retval=timer4_init(&timer4_ch4);
						retval=timer4_pwm_write(&timer4_ch4,servo_ptr->duty_cycle);

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


Std_ReturnType_t 	servo_go(Servo_t *servo_ptr,double angle){
	Std_ReturnType_t retval=E_OK;
	if(NULL==servo_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		servo_ptr->curr_pos=angle;
		servo_adjust_polarity(servo_ptr);
		servo_adjust_duty_cycle(servo_ptr);

		switch(servo_ptr->pwm_timer)
		{
			#if TIMER1_MODE==TIMER_PWM_MODE
			case TIMER1_PWM:
				switch(servo_ptr->pwm_channel)
				{
					
					case PWM_CH1:
						timer1_pwm_write(&timer1_ch1,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH2:
						timer1_pwm_write(&timer1_ch2,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH3:
						timer1_pwm_write(&timer1_ch3,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH4:
						timer1_pwm_write(&timer1_ch4,servo_ptr->duty_cycle);
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			
			#if TIMER2_MODE==TIMER_PWM_MODE
			case TIMER2_PWM:
					switch(servo_ptr->pwm_channel)
				{
					case PWM_CH1:
						timer2_pwm_write(&timer2_ch1,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH2:
						timer2_pwm_write(&timer2_ch2,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH3:
						timer2_pwm_write(&timer2_ch3,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH4:

						timer2_pwm_write(&timer2_ch4,servo_ptr->duty_cycle);
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			#if TIMER3_MODE==TIMER_PWM_MODE
			case TIMER3_PWM:
					switch(servo_ptr->pwm_channel)
				{
				servo_ptr->curr_pos=duty_cycle;
					case PWM_CH1:
						timer3_pwm_write(&timer3_ch1,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH2:
						timer3_pwm_write(&timer3_ch2,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH3:
						timer3_pwm_write(&timer3_ch3,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH4:
						timer3_pwm_write(&timer3_ch4,servo_ptr->duty_cycle);
					break;
					default:retval=E_NOT_OK;
				}
			break;
			#endif
			
			#if TIMER4_MODE==TIMER_PWM_MODE
			case TIMER4_PWM:
					switch(servo_ptr->pwm_channel)
				{
					case PWM_CH1:
						timer4_pwm_write(&timer4_ch1,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH2:
						timer4_pwm_write(&timer4_ch2,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH3:
						timer4_pwm_write(&timer4_ch3,servo_ptr->duty_cycle);
					break;
					
					case PWM_CH4:
						timer4_pwm_write(&timer4_ch4,servo_ptr->duty_cycle);
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

Std_ReturnType_t	servo_hold(Servo_t *servo_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==servo_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		
	}
	return retval;
}

/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/
static Std_ReturnType_t servo_adjust_polarity(Servo_t *servo_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==servo_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		if(servo_ptr->is_reverse)
		{
			servo_ptr->curr_pos=map_to_range(0,180,180,0,servo_ptr->curr_pos);
		}
	}
	
	return retval;
}
static Std_ReturnType_t servo_adjust_duty_cycle(Servo_t *servo_ptr)
{
	Std_ReturnType_t retval=E_OK;
	if( NULL==servo_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		servo_ptr->duty_cycle=map_to_range(0,180,500,2500,servo_ptr->curr_pos);
	}
}

/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/



