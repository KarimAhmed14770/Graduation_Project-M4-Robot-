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

//TIMER4 GENERAL MACROS
#define TIMER4_ENABLE()		                    	(SET_BIT_K(TIM4_CR1,TIMX_CR1_CEN))
#define TIMER4_DISABLE()	                    	(CLEAR_BIT_K(TIM4_CR1,TIMX_CR1_CEN))
#define TIMER4_UEV_ENABLE()		                	(CLEAR_BIT_K(TIM4_CR1,TIMX_CR1_UDIS))
#define TIMER4_UEV_DISABLE()	                	(SET_BIT_K(TIM4_CR1,TIMX_CR1_UDIS))
#define TIMER4_URS_ALL()                        (CLEAR_BIT_K(TIM4_CR1,TIMX_CR1_URS))
#define TIMER4_URS_OVERFLOW()                   (SET_BIT_K(TIM4_CR1,TIMX_CR1_URS))
#define TIMER4_SET_UP_COUNTER()                 (CLEAR_BIT_K(TIM4_CR1,TIMX_CR1_DIR))
#define TIMER4_SET_DOWN_COUNTER()               (SET_BIT_K(TIM4_CR1,TIMX_CR1_DIR))
#define TIMER4_AUTO_RELOAD_ENABLE()             (SET_BIT_K(TIM4_CR1,TIMX_CR1_ARPE))
#define TIMER4_AUTO_RELOAD_DISABLE()            (CLEAR_BIT_K(TIM4_CR1,TIMX_CR1_ARPE))
#define TIMER4_COUNTER_MODE_ENABLE()            (TIM4_SMCR|=0x0007)
#define TIMER4_TIMER_MODE_ENABLE()              (TIM4_SMCR&=0X0000)
#define TIMER4_SELECT_EDGE_ALIGNED_MODE()       (TIM4_CR1&=0XFF9F)
#define TIMER4_GENERATE_UPDATE()                (SET_BIT_K(TIM4_EGR,TIMX_EGR_UG))


//TIMER4 CHANNEL_1 MACROS
#define TIMER4_CH1_SELECT_OUTPUT_MODE()      		(TIM4_CCMR1&=0XFFFC)
#define TIMER4_CCP_CH1_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM4_CCMR1,TIMX_CCMR1_CC1S_1);\
    SET_BIT_K(TIM4_CCMR1,TIMX_CCMR1_CC1S_0);\
})
#define TIMER4_COUNTER_CH1_RISING_EDGE()						(CLEAR_BIT_K(TIM4_CCER,CC1P);
#define TIMER4_CH1_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM4_CCMR1,TIMX_CCMR1_OC1PE))
#define TIMER4_CH1_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM4_CCMR1,TIMX_CCMR1_OC1PE))
#define TIMER4_CH1_CCP_DISABLE()                    (TIM4_CCMR1&=0XFF8F)
#define TIMER4_CH1_ENABLE_OUTPUT()                  (SET_BIT_K(TIM4_CCER,TIMX_CCER_CC1E))
#define TIMER4_CH1_SELECT_PWM_MODE()                ({\
    TIM4_CCMR1&=0xFF8F;\
    TIM4_CCMR1|=0x0060;\
})



//TIMER4 CHANNEL_2 MACROS
#define TIMER4_CH2_SELECT_OUTPUT_MODE()      (TIM4_CCMR1&=0XFCFF)
#define TIMER4_CH2_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM4_CCMR1,TIMX_CCMR1_CC2S_1);\
    SET_BIT_K(TIM4_CCMR1,TIMX_CCMR1_CC2S_0);\
})
#define TIMER4_COUNTER_CH2_RISING_EDGE()						(CLEAR_BIT_K(TIM4_CCER,CC2P);
#define TIMER4_CH2_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM4_CCMR1,TIMX_CCMR1_OC2PE))
#define TIMER4_CH2_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM4_CCMR1,TIMX_CCMR1_OC2PE))
#define TIMER4_CH2_CCP_DISABLE()                    (TIM4_CCMR1&=0X8FFF)
#define TIMER4_CH2_ENABLE_OUTPUT()                  (SET_BIT_K(TIM4_CCER,TIMX_CCER_CC2E))
#define TIMER4_CH2_SELECT_PWM_MODE()                ({\
    TIM4_CCMR1&=0x8FFF;\
    TIM4_CCMR1|=0x6000;\
})




//TIMER4 CHANNEL_3 MACROS
#define TIMER4_CH3_SELECT_OUTPUT_MODE()      (TIM4_CCMR2&=0XFFFC)
#define TIMER4_CH3_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM4_CCMR1,TIMX_CCMR1_CC3S_1);\
    SET_BIT_K(TIM4_CCMR1,TIMX_CCMR1_CC3S_0);\
})
#define TIMER4_COUNTER_CH3_RISING_EDGE()						(CLEAR_BIT_K(TIM4_CCER,CC3P);
#define TIMER4_CH3_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM4_CCMR2,TIMX_CCMR2_OC3PE))
#define TIMER4_CH3_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM4_CCMR2,TIMX_CCMR2_OC3PE))
#define TIMER4_CH3_CCP_DISABLE()                    (TIM4_CCMR2&=0XFF8F)
#define TIMER4_CH3_ENABLE_OUTPUT()                  (SET_BIT_K(TIM4_CCER,TIMX_CCER_CC3E))
#define TIMER4_CH3_SELECT_PWM_MODE()                ({\
    TIM4_CCMR2&=0xFF8F;\
    TIM4_CCMR2|=0x0060;\
})


//TIMER4 CHANNEL_4 MACROS
#define TIMER4_CH4_SELECT_OUTPUT_MODE()      (TIM4_CCMR2&=0XFCFF)
#define TIMER4_CH4_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM4_CCMR2,TIMX_CCMR2_CC4S_1);\
    SET_BIT_K(TIM4_CCMR2,TIMX_CCMR2_CC4S_0);\
})
#define TIMER4_COUNTER_CH4_RISING_EDGE()						(CLEAR_BIT_K(TIM4_CCER,CC4P);
#define TIMER4_CH4_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM4_CCMR2,TIMX_CCMR2_OC4PE))
#define TIMER4_CH4_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM4_CCMR2,TIMX_CCMR2_OC4PE))
#define TIMER4_CH4_CCP_DISABLE()                    (TIM4_CCMR2&=0X8FFF)
#define TIMER4_CH4_ENABLE_OUTPUT()                  (SET_BIT_K(TIM4_CCER,TIMX_CCER_CC4E))
#define TIMER4_CH4_SELECT_PWM_MODE()                ({\
    TIM4_CCMR2&=0x8FFF;\
    TIM4_CCMR2|=0x6000;\
})



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
Std_ReturnType_t timer4_pwm_write(timer4_config_t *timer4_ptr,uint32_t duty_cycle);
#endif

void	TIM4_IRQHandler(void);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/



#endif

