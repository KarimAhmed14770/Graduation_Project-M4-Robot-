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

//TIMER1 GENERAL MACROS
#define TIMER1_ENABLE()		                    	(SET_BIT_K(TIM1_CR1,TIMX_CR1_CEN))
#define TIMER1_DISABLE()	                    	(CLEAR_BIT_K(TIM1_CR1,TIMX_CR1_CEN))
#define TIMER1_UEV_ENABLE()		                	(CLEAR_BIT_K(TIM1_CR1,TIMX_CR1_UDIS))
#define TIMER1_UEV_DISABLE()	                	(SET_BIT_K(TIM1_CR1,TIMX_CR1_UDIS))
#define TIMER1_URS_ALL()                        (CLEAR_BIT_K(TIM1_CR1,TIMX_CR1_URS))
#define TIMER1_URS_OVERFLOW()                   (SET_BIT_K(TIM1_CR1,TIMX_CR1_URS))
#define TIMER1_SET_UP_COUNTER()                 (CLEAR_BIT_K(TIM1_CR1,TIMX_CR1_DIR))
#define TIMER1_SET_DOWN_COUNTER()               (SET_BIT_K(TIM1_CR1,TIMX_CR1_DIR))
#define TIMER1_AUTO_RELOAD_ENABLE()             (SET_BIT_K(TIM1_CR1,TIMX_CR1_ARPE))
#define TIMER1_AUTO_RELOAD_DISABLE()            (CLEAR_BIT_K(TIM1_CR1,TIMX_CR1_ARPE))
#define TIMER1_COUNTER_MODE_ENABLE()            (TIM1_SMCR|=0x0007)
#define TIMER1_TIMER_MODE_ENABLE()              (TIM1_SMCR&=0X0000)
#define TIMER1_SELECT_EDGE_ALIGNED_MODE()       (TIM1_CR1&=0XFF9F)
#define TIMER1_GENERATE_UPDATE()                (SET_BIT_K(TIM1_EGR,TIMX_EGR_UG))


//TIMER1 CHANNEL_1 MACROS
#define TIMER1_CH1_SELECT_OUTPUT_MODE()      		(TIM1_CCMR1&=0XFFFC)
#define TIMER1_CCP_CH1_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM1_CCMR1,TIMX_CCMR1_CC1S_1);\
    SET_BIT_K(TIM1_CCMR1,TIMX_CCMR1_CC1S_0);\
})
#define TIMER1_COUNTER_CH1_RISING_EDGE()						(CLEAR_BIT_K(TIM1_CCER,CC1P);
#define TIMER1_CH1_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM1_CCMR1,TIMX_CCMR1_OC1PE))
#define TIMER1_CH1_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM1_CCMR1,TIMX_CCMR1_OC1PE))
#define TIMER1_CH1_CCP_DISABLE()                    (TIM1_CCMR1&=0XFF8F)
#define TIMER1_CH1_ENABLE_OUTPUT()  								({\
  SET_BIT_K(TIM1_BDTR,TIMX_BDTR_MOE);\
	SET_BIT_K(TIM1_CCER,TIMX_CCER_CC1E);\
})

#define TIMER1_CH1_SELECT_PWM_MODE()                ({\
    TIM1_CCMR1&=0xFF8F;\
    TIM1_CCMR1|=0x0060;\
})



//TIMER1 CHANNEL_2 MACROS
#define TIMER1_CH2_SELECT_OUTPUT_MODE()      (TIM1_CCMR1&=0XFCFF)
#define TIMER1_CH2_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM1_CCMR1,TIMX_CCMR1_CC2S_1);\
    SET_BIT_K(TIM1_CCMR1,TIMX_CCMR1_CC2S_0);\
})
#define TIMER1_COUNTER_CH2_RISING_EDGE()						(CLEAR_BIT_K(TIM1_CCER,CC2P);
#define TIMER1_CH2_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM1_CCMR1,TIMX_CCMR1_OC2PE))
#define TIMER1_CH2_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM1_CCMR1,TIMX_CCMR1_OC2PE))
#define TIMER1_CH2_CCP_DISABLE()                    (TIM1_CCMR1&=0X8FFF)
#define TIMER1_CH2_ENABLE_OUTPUT()  								({\
  SET_BIT_K(TIM1_BDTR,TIMX_BDTR_MOE);\
	SET_BIT_K(TIM1_CCER,TIMX_CCER_CC2E);\
})
#define TIMER1_CH2_SELECT_PWM_MODE()                ({\
    TIM1_CCMR1&=0x8FFF;\
    TIM1_CCMR1|=0x6000;\
})




//TIMER1 CHANNEL_3 MACROS
#define TIMER1_CH3_SELECT_OUTPUT_MODE()      (TIM1_CCMR2&=0XFFFC)
#define TIMER1_CH3_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM1_CCMR1,TIMX_CCMR1_CC3S_1);\
    SET_BIT_K(TIM1_CCMR1,TIMX_CCMR1_CC3S_0);\
})
#define TIMER1_COUNTER_CH3_RISING_EDGE()						(CLEAR_BIT_K(TIM1_CCER,CC3P);
#define TIMER1_CH3_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM1_CCMR2,TIMX_CCMR2_OC3PE))
#define TIMER1_CH3_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM1_CCMR2,TIMX_CCMR2_OC3PE))
#define TIMER1_CH3_CCP_DISABLE()                    (TIM1_CCMR2&=0XFF8F)
#define TIMER1_CH3_ENABLE_OUTPUT()  								({\
  SET_BIT_K(TIM1_BDTR,TIMX_BDTR_MOE);\
	SET_BIT_K(TIM1_CCER,TIMX_CCER_CC3E);\
})
#define TIMER1_CH3_SELECT_PWM_MODE()                ({\
    TIM1_CCMR2&=0xFF8F;\
    TIM1_CCMR2|=0x0060;\
})


//TIMER1 CHANNEL_4 MACROS
#define TIMER1_CH4_SELECT_OUTPUT_MODE()      (TIM1_CCMR2&=0XFCFF)
#define TIMER1_CH4_SELECT_INPUT_MODE()       ({\
    CLEAR_BIT_K(TIM1_CCMR2,TIMX_CCMR2_CC4S_1);\
    SET_BIT_K(TIM1_CCMR2,TIMX_CCMR2_CC4S_0);\
})
#define TIMER1_COUNTER_CH4_RISING_EDGE()						(CLEAR_BIT_K(TIM1_CCER,CC4P);
#define TIMER1_CH4_CCP_DISABLE_PRELOAD()            (CLEAR_BIT_K(TIM1_CCMR2,TIMX_CCMR2_OC4PE))
#define TIMER1_CH4_CCP_ENABLE_PRELOAD()             (SET_BIT_K(TIM1_CCMR2,TIMX_CCMR2_OC4PE))
#define TIMER1_CH4_CCP_DISABLE()                    (TIM1_CCMR2&=0X8FFF)
#define TIMER1_CH4_ENABLE_OUTPUT()  								({\
  SET_BIT_K(TIM1_BDTR,TIMX_BDTR_MOE);\
	SET_BIT_K(TIM1_CCER,TIMX_CCER_CC4E);\
})
#define TIMER1_CH4_SELECT_PWM_MODE()                ({\
    TIM1_CCMR2&=0x8FFF;\
    TIM1_CCMR2|=0x6000;\
})








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



