/**
@file: MCAL_TIMERS.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "MCAL_TIMERS.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
#if TIMER1_MODE==TIMER_PWM_MODE
extern timer1_config_t timer1_ch1={
	.channel=TIMER1_CH1,
};
extern timer1_config_t timer1_ch2={
	.channel=TIMER1_CH2,

};
extern timer1_config_t timer1_ch3={
	.channel=TIMER1_CH3,

};
extern timer1_config_t timer1_ch4={
	.channel=TIMER1_CH4,

};
#endif

#if TIMER2_MODE==TIMER_PWM_MODE
extern timer2_config_t timer2_ch1={
	.channel=TIMER2_CH1,

};
extern timer2_config_t timer2_ch2={
	.channel=TIMER2_CH2,
	
};
extern timer2_config_t timer2_ch3={
	.channel=TIMER2_CH3,

};
extern timer2_config_t timer2_ch4={
	.channel=TIMER2_CH4,

};
#endif

#if TIMER3_MODE==TIMER_PWM_MODE
extern timer3_config_t timer3_ch1={
	.channel=TIMER3_CH1,

};
extern timer3_config_t timer3_ch2={
	.channel=TIMER3_CH2,

};
extern timer3_config_t timer3_ch3={
	.channel=TIMER3_CH3,

};
extern timer3_config_t timer3_ch4={
	.channel=TIMER3_CH4,

};
#endif

#if TIMER4_MODE==TIMER_PWM_MODE
extern timer4_config_t timer4_ch1={
	.channel=TIMER4_CH1,

};
extern timer4_config_t timer4_ch2={
	.channel=TIMER4_CH2,

};
extern timer4_config_t timer4_ch3={
	.channel=TIMER4_CH3,

};
extern timer4_config_t timer4_ch4={
	.channel=TIMER4_CH4,

};
#endif


#if TIMER1_MODE==TIMER_TIMER_MODE
extern timer1_config_t tim1={
	.TIM1_OF_InterruptHandler=NULL,
	.time_base_us=1000

};

#endif

#if TIMER2_MODE==TIMER_TIMER_MODE
extern timer2_config_t tim2={
	.TIM2_OF_InterruptHandler=NULL,
	.time_base_us=1000

};

#endif

#if TIMER3_MODE==TIMER_TIMER_MODE
extern timer3_config_t tim3={
	.TIM3_OF_InterruptHandler=NULL,
	.time_base_us=1000

};

#endif

#if TIMER4_MODE==TIMER_TIMER_MODE
extern timer4_config_t tim4={
	.TIM4_OF_InterruptHandler=NULL,
	.time_base_us=1000

};

#endif
/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/

/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/

/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/

/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/



