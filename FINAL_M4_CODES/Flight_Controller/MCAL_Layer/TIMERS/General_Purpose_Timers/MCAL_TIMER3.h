/**
@file: MCAL_TIMER3.h
@author: M4_TEAM 
@creation_date: date
*/

#ifndef MCAL_TIMER3_H
#define MCAL_TIMER3_H

/*******************INCLUDES SECTION START************************/
#include "../../GPIO/MCAL_GPIO.h"
#include "../MCAL_TIMERS_GEN_CFG.h"
#include "../../INTERRUPT/MCAL_INTERNAL_INTERRUPTS.h"
#include	"../../INTERRUPT/MCAL_NVIC.h"
/*******************INCLUDES SECTION END  ************************/




/*******************MACROS DEFINITION SECTION START***************/
#define TIMER3_CLKIN_FREQ       (72)
#define COUNTER_PRESCALER       (1)
#define TIMER3_COUNT_REG_MAX    (65535)
#define TIMER3_CH1_2_PORT				(PORTA)
#define TIMER3_CH3_4_PORT				(PORTB)
#define TIMER3_CH1_PIN					(PIN6)
#define TIMER3_CH2_PIN					(PIN7)
#define TIMER3_CH3_PIN					(PIN0)
#define TIMER3_CH4_PIN					(PIN1)

/*******************MACROS DEFINITION SECTION END  ***************/




/*******************MACROS LIKE FUNCTIONS SECTION START***********/









/*******************MACROS LIKE FUNCTIONS SECTION END  ***********/




/*******************DATA TYPES DECLARATION SECTION START**********/

#if TIMER3_MODE==TIMER_PWM_MODE
typedef enum{
		TIMER3_CH1=1, //pin A0
		TIMER3_CH2,	//pinA1
		TIMER3_CH3,	//pinA2
		TIMER3_CH4		//pinA3
}timer3_pwm_channel_t;
#endif

typedef struct{
    #if TIMER3_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
			#if TIMER3_MODE==TIMER_TIMER_MODE
			void(*TIM3_OF_InterruptHandler)(void);
			#endif
        #if TIMER3_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            void(*TIM3_CCP1_InterruptHandler)(void);
            void(*TIM3_CCP2_InterruptHandler)(void);
            void(*TIM3_CCP3_InterruptHandler)(void);
            void(*TIM3_CCP4_InterruptHandler)(void);
        #endif
    #endif

    #if TIMER3_MODE==TIMER_TIMER_MODE
    uint32_t time_base_us;
    #endif

    #if TIMER3_MODE==TIMER_PWM_MODE
    uint32_t PWM_frequency;
    timer3_pwm_channel_t	channel;
		uint32_t duty_cycle;
    #endif
    

}timer3_config_t;
/*******************DATA TYPES DECLARATION SECTION END  **********/




/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION START****/
Std_ReturnType_t timer3_init(timer3_config_t *timer3_ptr);
Std_ReturnType_t timer3_deinit(timer3_config_t *timer3_ptr);
#if TIMER3_MODE==TIMER_TIMER_MODE
Std_ReturnType_t timer3_timeBase_write(timer3_config_t *timer3_ptr,uint32_t time_base);
Std_ReturnType_t timer3_delay(timer3_config_t *timer3_ptr,uint32_t delay_time);

#endif


#if TIMER3_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer3_pwm_write(timer3_config_t *timer3_ptr,uint16_t duty_cycle);
#endif

void	TIM3_IRQHandler(void);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/



#endif


