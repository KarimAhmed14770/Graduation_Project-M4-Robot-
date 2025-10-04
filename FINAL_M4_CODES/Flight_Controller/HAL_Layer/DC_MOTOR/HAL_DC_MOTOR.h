/**
@file: HAL_DC_MOTOR.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef HAL_DC_MOTOR_H
#define HAL_DC_MOTOR_H

/*******************INCLUDES SECTION START************************/
#include	"../../MCAL_Layer/TIMERS/General_Purpose_Timers/MCAL_TIMERS.h"
#include 	"../../Softwares/M4_Softwares.h"
/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/
#define DC_FREQUENCY						(20000)
#define REVERSE_DC_DIRECTIONS		(0)
#define ZERO_SPEED							(127)
#define MAX_SPEED								(255)
/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/

/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/
#if TIMER1_MODE==TIMER_PWM_MODE || TIMER2_MODE==TIMER_PWM_MODE|| TIMER3_MODE==TIMER_PWM_MODE|| TIMER4_MODE==TIMER_PWM_MODE

typedef enum{
	#if TIMER1_MODE==TIMER_PWM_MODE
	DC_PWM_TIMER1=1,
	#endif
	#if TIMER2_MODE==TIMER_PWM_MODE
	DC_PWM_TIMER2=2,
	#endif
	#if TIMER3_MODE==TIMER_PWM_MODE
	DC_PWM_TIMER3=3,
	#endif
	#if TIMER4_MODE==TIMER_PWM_MODE
	DC_PWM_TIMER4=4,
	#endif
}Pwm_DC_timer_t;
#endif

typedef enum{
	DC_CH1=1,
	DC_CH2,
	DC_CH3,
	DC_CH4
}Dc_pwm_channel_t;

typedef struct{
	Pin_Config_t IN1_pin;
	Pin_Config_t IN2_pin;
	#if TIMER1_MODE==TIMER_PWM_MODE || TIMER2_MODE==TIMER_PWM_MODE|| TIMER3_MODE==TIMER_PWM_MODE|| TIMER4_MODE==TIMER_PWM_MODE
	Pwm_DC_timer_t 	pwm_timer;
	#endif
	Dc_pwm_channel_t pwm_channel;
}Dc_motor_t;
/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t Dc_motor_init(Dc_motor_t *dc_motor_ptr);
Std_ReturnType_t Dc_motor_move(Dc_motor_t *dc_motor_ptr,uint32_t speed);
Std_ReturnType_t Dc_motor_stop(Dc_motor_t *dc_motor_ptr);


/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/




#endif

