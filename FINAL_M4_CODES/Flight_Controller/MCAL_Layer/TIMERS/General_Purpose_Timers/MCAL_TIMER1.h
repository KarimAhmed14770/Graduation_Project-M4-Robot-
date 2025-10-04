/**
@file: MCAL_TIMER1.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef MCAL_TIMER1_H
#define MCAL_TIMER1_H

/*******************INCLUDES SECTION START************************/
#include "../../GPIO/MCAL_GPIO.h"
#include "../MCAL_TIMERS_GEN_CFG.h"
#include "../../INTERRUPT/MCAL_INTERNAL_INTERRUPTS.h"
#include	"../../INTERRUPT/MCAL_NVIC.h"
/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/
#define TIMER1_CLKIN_FREQ       (72)
#define COUNTER_PRESCALER       (1)
#define TIMER1_COUNT_REG_MAX    (65535)
#define TIMER1_CH_PORT					(PORTA)
#define TIMER1_CH1_PIN					(PIN8)
#define TIMER1_CH2_PIN					(PIN9)
#define TIMER1_CH3_PIN					(PIN10)
#define TIMER1_CH4_PIN					(PIN11)

/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/









/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/

#if TIMER1_MODE==TIMER_PWM_MODE
typedef enum{
		TIMER1_CH1=1, //pin A0
		TIMER1_CH2,	//pinA1
		TIMER1_CH3,	//pinA2
		TIMER1_CH4		//pinA3
}timer1_pwm_channel_t;
#endif

typedef struct{
    #if TIMER1_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
			#if TIMER1_MODE==TIMER_TIMER_MODE
			void(*TIM1_OF_InterruptHandler)(void);
			#endif
        #if TIMER1_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            void(*TIM1_CCP1_InterruptHandler)(void);
            void(*TIM1_CCP2_InterruptHandler)(void);
            void(*TIM1_CCP3_InterruptHandler)(void);
            void(*TIM1_CCP4_InterruptHandler)(void);
        #endif
    #endif

    #if TIMER1_MODE==TIMER_TIMER_MODE
    uint32_t time_base_us;
    #endif

    #if TIMER1_MODE==TIMER_PWM_MODE
    uint32_t PWM_frequency;
    timer1_pwm_channel_t	channel;
		uint32_t duty_cycle;
    #endif
    

}timer1_config_t;
/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t timer1_init(timer1_config_t *timer1_ptr);
Std_ReturnType_t timer1_deinit(timer1_config_t *timer1_ptr);
#if TIMER1_MODE==TIMER_TIMER_MODE
Std_ReturnType_t timer1_timeBase_write(timer1_config_t *timer1_ptr,uint32_t time_base);
Std_ReturnType_t timer1_delay(timer1_config_t *timer1_ptr,uint32_t delay_time);
uint64_t timer1_millis();

#endif


#if TIMER1_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer1_pwm_write(timer1_config_t *timer1_ptr,uint32_t duty_cycle);
#endif

void	TIM1_UP_IRQHandler(void);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/



#endif



