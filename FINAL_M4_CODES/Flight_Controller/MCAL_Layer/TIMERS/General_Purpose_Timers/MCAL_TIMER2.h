/**
@file: MCAL_TIMER2.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef MCAL_TIMER2_H
#define MCAL_TIMER2_H

/*******************INCLUDES SECTION START************************/
#include "../../GPIO/MCAL_GPIO.h"
#include "../MCAL_TIMERS_GEN_CFG.h"
#include "../../INTERRUPT/MCAL_INTERNAL_INTERRUPTS.h"
#include	"../../INTERRUPT/MCAL_NVIC.h"
/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/
#define TIMER2_CLKIN_FREQ       (72)
#define COUNTER_PRESCALER       (1)
#define TIMER2_COUNT_REG_MAX    (65535)
#define TIMER2_CH_PORT					(PORTA)
#define TIMER2_CH1_PIN					(PIN0)
#define TIMER2_CH2_PIN					(PIN1)
#define TIMER2_CH3_PIN					(PIN2)
#define TIMER2_CH4_PIN					(PIN3)

/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/









/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/

#if TIMER2_MODE==TIMER_PWM_MODE
typedef enum{
		TIMER2_CH1=1, //pin A0
		TIMER2_CH2,	//pinA1
		TIMER2_CH3,	//pinA2
		TIMER2_CH4		//pinA3
}timer2_pwm_channel_t;
#endif

typedef struct{
    #if TIMER2_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
			#if TIMER2_MODE==TIMER_TIMER_MODE
			void(*TIM2_OF_InterruptHandler)(void);
			#endif
        #if TIMER2_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            void(*TIM2_CCP1_InterruptHandler)(void);
            void(*TIM2_CCP2_InterruptHandler)(void);
            void(*TIM2_CCP3_InterruptHandler)(void);
            void(*TIM2_CCP4_InterruptHandler)(void);
        #endif
    #endif

    #if TIMER2_MODE==TIMER_TIMER_MODE
    uint32_t time_base_us;
    #endif

    #if TIMER2_MODE==TIMER_PWM_MODE
    uint32_t PWM_frequency;
    timer2_pwm_channel_t	channel;
		uint32_t duty_cycle;
    #endif
    

}timer2_config_t;
/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t timer2_init(timer2_config_t *timer2_ptr);
Std_ReturnType_t timer2_deinit(timer2_config_t *timer2_ptr);
#if TIMER2_MODE==TIMER_TIMER_MODE
Std_ReturnType_t timer2_timeBase_write(timer2_config_t *timer2_ptr,uint32_t time_base);
Std_ReturnType_t timer2_delay(timer2_config_t *timer2_ptr,uint32_t delay_time);

#endif


#if TIMER2_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer2_pwm_write(timer2_config_t *timer2_ptr,uint16_t duty_cycle);
#endif

void	TIM2_IRQHandler(void);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/



#endif


