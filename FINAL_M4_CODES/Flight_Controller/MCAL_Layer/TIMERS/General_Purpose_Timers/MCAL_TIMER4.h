/**
@file: MCAL_TIMER4.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef MCAL_TIMER4_H
#define MCAL_TIMER4_H

/*******************INCLUDES SECTION START************************/
#include "../../GPIO/MCAL_GPIO.h"
#include "../MCAL_TIMERS_GEN_CFG.h"
#include "../../INTERRUPT/MCAL_INTERNAL_INTERRUPTS.h"
#include	"../../INTERRUPT/MCAL_NVIC.h"
/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/
#define TIMER4_CLKIN_FREQ       (72)
#define COUNTER_PRESCALER       (1)
#define TIMER4_COUNT_REG_MAX    (65535)
#define TIMER4_CH_PORT					(PORTB)
#define TIMER4_CH1_PIN					(PIN6)
#define TIMER4_CH2_PIN					(PIN7)
#define TIMER4_CH3_PIN					(PIN8)
#define TIMER4_CH4_PIN					(PIN9)

/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/









/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/

#if TIMER4_MODE==TIMER_PWM_MODE
typedef enum{
		TIMER4_CH1=1, //pin A0
		TIMER4_CH2,	//pinA1
		TIMER4_CH3,	//pinA2
		TIMER4_CH4		//pinA3
}timer4_pwm_channel_t;
#endif

typedef struct{
    #if TIMER4_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
			#if TIMER4_MODE==TIMER_TIMER_MODE
			void(*TIM4_OF_InterruptHandler)(void);
			#endif
        #if TIMER4_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            void(*TIM4_CCP1_InterruptHandler)(void);
            void(*TIM4_CCP2_InterruptHandler)(void);
            void(*TIM4_CCP3_InterruptHandler)(void);
            void(*TIM4_CCP4_InterruptHandler)(void);
        #endif
    #endif

    #if TIMER4_MODE==TIMER_TIMER_MODE
    uint32_t time_base_us;
    #endif

    #if TIMER4_MODE==TIMER_PWM_MODE
    uint32_t PWM_frequency;
    timer4_pwm_channel_t	channel;
		uint32_t duty_cycle;
    #endif
    

}timer4_config_t;
/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t timer4_init(timer4_config_t *timer4_ptr);
Std_ReturnType_t timer4_deinit(timer4_config_t *timer4_ptr);
#if TIMER4_MODE==TIMER_TIMER_MODE
Std_ReturnType_t timer4_timeBase_write(timer4_config_t *timer4_ptr,uint32_t time_base);
Std_ReturnType_t timer4_delay(timer4_config_t *timer4_ptr,uint32_t delay_time);

#endif


#if TIMER4_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer4_pwm_write(timer4_config_t *timer4_ptr,uint16_t duty_cycle);
#endif

void	TIM4_IRQHandler(void);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/



#endif


