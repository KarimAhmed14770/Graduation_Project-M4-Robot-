/**
@file: HAL_SERVO.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef HAL_SERVO
#define HAL_SERVO

/*******************INCLUDES SECTION START************************/
#include "../../MCAL_Layer//TIMERS/General_Purpose_Timers/MCAL_TIMERS.h"
/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/
#define SERVO_FREQUENCY (20000)
#define SERVO_MIN_POS		(500)
#define SERVO_MAX_POS		(2500)
/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/

/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/
typedef enum{
	#if TIMER1_MODE==TIMER_PWM_MODE
	TIMER1_PWM=1,
	#endif
	#if TIMER2_MODE==TIMER_PWM_MODE
	TIMER2_PWM=2,
	#endif
	#if TIMER3_MODE==TIMER_PWM_MODE
	TIMER3_PWM=3,
	#endif
	#if TIMER4_MODE==TIMER_PWM_MODE
	TIMER4_PWM=4
	#endif
}Pwm_timer_t;

typedef enum{
	
	PWM_CH1,
	PWM_CH2,	
	PWM_CH3,
	PWM_CH4
}pwm_channel_t;


typedef enum{
	NORMAL,
	REVERSE
}Servo_is_reverse_t;
typedef enum{
	#if TIMER1_MODE==TIMER_TIMER_MODE
	TIMER1_TIMING=1,
	#endif
	#if TIMER2_MODE==TIMER_TIMER_MODE
	TIMER2_TIMING=2,
	#endif
	#if TIMER3_MODE==TIMER_TIMER_MODE
	TIMER3_TIMING=3,
	#endif
	#if TIMER4_MODE==TIMER_TIMER_MODE
	TIMER4_TIMING=4
	#endif
}timing_timer_t;


typedef struct{
	Pwm_timer_t 			pwm_timer;
	pwm_channel_t 		pwm_channel;
	uint8_t 					starting_pos;
	double						curr_pos;
	uint16_t 					duty_cycle;
	uint8_t 					is_reverse;
	float							gear_ratio;
}Servo_t;
/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t 	servo_init(Servo_t *servo_ptr);
Std_ReturnType_t 	servo_go(Servo_t *servo_ptr,uint8_t angle);
Std_ReturnType_t	servo_hold(Servo_t *servo_ptr);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/




#endif

