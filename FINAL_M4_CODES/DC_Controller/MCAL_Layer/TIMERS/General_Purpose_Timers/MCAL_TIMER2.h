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

//TIMER2 GENERAL MACROS
#define TIMER2_ENABLE()		                    	(SET_BIT_K(TIM2_CR1,TIMX_CR1_CEN))
#define TIMER2_DISABLE()	                    	(CLEAR_BIT_K(TIM2_CR1,TIMX_CR1_CEN))
#define TIMER2_UEV_ENABLE()		                	(CLEAR_BIT_K(TIM2_CR1,TIMX_CR1_UDIS))
#define TIMER2_UEV_DISABLE()	                	(SET_BIT_K(TIM2_CR1,TIMX_CR1_UDIS))
#define TIMER2_URS_ALL()                        (CLEAR_BIT_K(TIM2_CR1,TIMX_CR1_URS))
#define TIMER2_URS_OVERFLOW()                   (SET_BIT_K(TIM2_CR1,TIMX_CR1_URS))
#define TIMER2_SET_UP_COUNTER()                 (CLEAR_BIT_K(TIM2_CR1,TIMX_CR1_DIR))
#define TIMER2_SET_DOWN_COUNTER()               (SET_BIT_K(TIM2_CR1,TIMX_CR1_DIR))
#define TIMER2_AUTO_RELOAD_ENABLE()             (SET_BIT_K(TIM2_CR1,TIMX_CR1_ARPE))
#define TIMER2_AUTO_RELOAD_DISABLE()            (CLEAR_BIT_K(TIM2_CR1,TIMX_CR1_ARPE))
#define TIMER2_COUNTER_MODE_ENABLE()            (TIM2_SMCR|=0x0007)
#define TIMER2_TIMER_MODE_ENABLE()              (TIM2_SMCR&=0X0000)
#define TIMER2_SELECT_EDGE_ALIGNED_MODE()       (TIM2_CR1&=0XFF9F)
#define TIMER2_GENERATE_UPDATE()                (SET_BIT_K(TIM2_EGR,TIMX_EGR_UG))


//TIMER2 CHANNEL_1 MACROS
#define TIMER2_CH1_SELECT_OUTPUT_MODE()      		(TIM2_CCMR1&=0XFFFC)
#define TIMER2_CCP_CH1_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM2_CCMR1,TIMX_CCMR1_CC1S_1);\
    SET_BIT_K(TIM2_CCMR1,TIMX_CCMR1_CC1S_0);\
})
#define TIMER2_COUNTER_CH1_RISING_EDGE()						(CLEAR_BIT_K(TIM2_CCER,CC1P);
#define TIMER2_CH1_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM2_CCMR1,TIMX_CCMR1_OC1PE))
#define TIMER2_CH1_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM2_CCMR1,TIMX_CCMR1_OC1PE))
#define TIMER2_CH1_CCP_DISABLE()                    (TIM2_CCMR1&=0XFF8F)
#define TIMER2_CH1_ENABLE_OUTPUT()                  (SET_BIT_K(TIM2_CCER,TIMX_CCER_CC1E))

#define TIMER2_CH1_SELECT_PWM_MODE()                ({\
    TIM2_CCMR1&=0xFF8F;\
    TIM2_CCMR1|=0x0060;\
})



//TIMER2 CHANNEL_2 MACROS
#define TIMER2_CH2_SELECT_OUTPUT_MODE()      (TIM2_CCMR1&=0XFCFF)
#define TIMER2_CH2_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM2_CCMR1,TIMX_CCMR1_CC2S_1);\
    SET_BIT_K(TIM2_CCMR1,TIMX_CCMR1_CC2S_0);\
})
#define TIMER2_COUNTER_CH2_RISING_EDGE()						(CLEAR_BIT_K(TIM2_CCER,CC2P);
#define TIMER2_CH2_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM2_CCMR1,TIMX_CCMR1_OC2PE))
#define TIMER2_CH2_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM2_CCMR1,TIMX_CCMR1_OC2PE))
#define TIMER2_CH2_CCP_DISABLE()                    (TIM2_CCMR1&=0X8FFF)
#define TIMER2_CH2_ENABLE_OUTPUT()                  (SET_BIT_K(TIM2_CCER,TIMX_CCER_CC2E))
#define TIMER2_CH2_SELECT_PWM_MODE()                ({\
    TIM2_CCMR1&=0x8FFF;\
    TIM2_CCMR1|=0x6000;\
})




//TIMER2 CHANNEL_3 MACROS
#define TIMER2_CH3_SELECT_OUTPUT_MODE()      (TIM2_CCMR2&=0XFFFC)
#define TIMER2_CH3_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM2_CCMR1,TIMX_CCMR1_CC3S_1);\
    SET_BIT_K(TIM2_CCMR1,TIMX_CCMR1_CC3S_0);\
})
#define TIMER2_COUNTER_CH3_RISING_EDGE()						(CLEAR_BIT_K(TIM2_CCER,CC3P);
#define TIMER2_CH3_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM2_CCMR2,TIMX_CCMR2_OC3PE))
#define TIMER2_CH3_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM2_CCMR2,TIMX_CCMR2_OC3PE))
#define TIMER2_CH3_CCP_DISABLE()                    (TIM2_CCMR2&=0XFF8F)
#define TIMER2_CH3_ENABLE_OUTPUT()                  (SET_BIT_K(TIM2_CCER,TIMX_CCER_CC3E))
#define TIMER2_CH3_SELECT_PWM_MODE()                ({\
    TIM2_CCMR2&=0xFF8F;\
    TIM2_CCMR2|=0x0060;\
})


//TIMER2 CHANNEL_4 MACROS
#define TIMER2_CH4_SELECT_OUTPUT_MODE()      (TIM2_CCMR2&=0XFCFF)
#define TIMER2_CH4_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM2_CCMR2,TIMX_CCMR2_CC4S_1);\
    SET_BIT_K(TIM2_CCMR2,TIMX_CCMR2_CC4S_0);\
})
#define TIMER2_COUNTER_CH4_RISING_EDGE()						(CLEAR_BIT_K(TIM2_CCER,CC4P);
#define TIMER2_CH4_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM2_CCMR2,TIMX_CCMR2_OC4PE))
#define TIMER2_CH4_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM2_CCMR2,TIMX_CCMR2_OC4PE))
#define TIMER2_CH4_CCP_DISABLE()                    (TIM2_CCMR2&=0X8FFF)
#define TIMER2_CH4_ENABLE_OUTPUT()                  (SET_BIT_K(TIM2_CCER,TIMX_CCER_CC4E))
#define TIMER2_CH4_SELECT_PWM_MODE()                ({\
    TIM2_CCMR2&=0x8FFF;\
    TIM2_CCMR2|=0x6000;\
})








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
Std_ReturnType_t timer2_pwm_write(timer2_config_t *timer2_ptr,uint32_t duty_cycle);
#endif

void	TIM2_IRQHandler(void);
/*******************FUCNCTIONS PROTOTYPES (APIS) SECTION END  ****/



#endif


