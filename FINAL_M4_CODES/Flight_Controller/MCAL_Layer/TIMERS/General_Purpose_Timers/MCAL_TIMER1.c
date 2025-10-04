/**
@file: MCAL_TIMER1.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "MCAL_TIMER1.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
static uint16_t prescaler_value=TIMER1_CLKIN_FREQ;
static uint16_t preload_value;
static uint8_t  timer1_resolution;//in us
static uint32_t timer1_max_count;//in us
#if TIMER1_MODE==TIMER_TIMER_MODE
static uint32_t delay;
extern timer1_config_t tim1;
uint8_t timer1_millis_counter;
uint64_t _miLLis__;
void timer1_isr(void);
#endif

#if TIMER1_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
static void(*TIM1_OF_InterruptHandler)(void)=NULL;
	#if TIMER1_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
	static void(*TIM1_CCP1_InterruptHandler)(void)=NULL;
	static void(*TIM1_CCP2_InterruptHandler)(void)=NULL;
	static void(*TIM1_CCP3_InterruptHandler)(void)=NULL;
	static void(*TIM1_CCP4_InterruptHandler)(void)=NULL;
	#endif
#endif

/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
#if TIMER1_MODE==TIMER_PWM_MODE
static Std_ReturnType_t pwm_pin_init(timer1_config_t *timer1_ptr);
#endif
static Std_ReturnType_t set_timer1_mode_preload_prescale(timer1_config_t *timer1_ptr);
static Std_ReturnType_t Enable_timer1_interrupt(void);
static Std_ReturnType_t Disable_timer1_interrupt(void);
static Std_ReturnType_t	timer1_handle_interrupts(timer1_config_t	*timer1_ptr);


//TIMER1 GENERAL MACROS
void TIMER1_ENABLE(void);
void TIMER1_DISABLE(void);
void TIMER1_UEV_ENABLE(void);
void TIMER1_UEV_DISABLE(void);
	
void TIMER1_URS_ALL(void);
void TIMER1_URS_OVERFLOW(void);
void TIMER1_SET_UP_COUNTER(void);
void TIMER1_SET_DOWN_COUNTER(void);
	
void TIMER1_AUTO_RELOAD_ENABLE(void);
	
void TIMER1_AUTO_RELOAD_DISABLE(void);
	
void TIMER1_COUNTER_MODE_ENABLE(void);
	
void TIMER1_TIMER_MODE_ENABLE(void);
	
void TIMER1_SELECT_EDGE_ALIGNED_MODE(void);
void TIMER1_GENERATE_UPDATE(void);
//TIMER1 CHANNEL_1 MACROS
void TIMER1_CH1_SELECT_OUTPUT_MODE(void);
void TIMER1_CCP_CH1_SELECT_INPUT_MODE(void);

void TIMER1_COUNTER_CH1_RISING_EDGE(void);
	
void TIMER1_CH1_CCP_DISABLE_PRELOAD(void);
void TIMER1_CH1_CCP_ENABLE_PRELOAD(void);
	
void TIMER1_CH1_CCP_DISABLE(void);
	
void TIMER1_CH1_ENABLE_OUTPUT(void);
void TIMER1_CH1_SELECT_PWM_MODE(void);


//TIMER1 CHANNEL_2 MACROS
void TIMER1_CH2_SELECT_OUTPUT_MODE(void);
	
void TIMER1_CH2_SELECT_INPUT_MODE(void);
void TIMER1_COUNTER_CH2_RISING_EDGE(void);
void TIMER1_CH2_CCP_DISABLE_PRELOAD(void);
	
void TIMER1_CH2_CCP_ENABLE_PRELOAD(void);
void TIMER1_CH2_CCP_DISABLE(void);
	
void TIMER1_CH2_ENABLE_OUTPUT(void);
void TIMER1_CH2_SELECT_PWM_MODE(void);



//TIMER1 CHANNEL_3 MACROS
void TIMER1_CH3_SELECT_OUTPUT_MODE(void);
void TIMER1_CH3_SELECT_INPUT_MODE(void);
void TIMER1_COUNTER_CH3_RISING_EDGE(void);
void TIMER1_CH3_CCP_DISABLE_PRELOAD(void);
	
void TIMER1_CH3_CCP_ENABLE_PRELOAD(void);
void TIMER1_CH3_CCP_DISABLE(void);
	
void TIMER1_CH3_ENABLE_OUTPUT(void);
void TIMER1_CH3_SELECT_PWM_MODE(void);


//TIMER1 CHANNEL_4 MACROS
void TIMER1_CH4_SELECT_OUTPUT_MODE(void);
void TIMER1_CH4_SELECT_INPUT_MODE(void);
void TIMER1_COUNTER_CH4_RISING_EDGE(void);
	
void TIMER1_CH4_CCP_DISABLE_PRELOAD(void);
	
void TIMER1_CH4_CCP_ENABLE_PRELOAD(void);
void TIMER1_CH4_CCP_DISABLE(void);
void TIMER1_CH4_ENABLE_OUTPUT(void);
void TIMER1_CH4_SELECT_PWM_MODE(void);

/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t timer1_init(timer1_config_t *timer1_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer1_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
        /******************enable timer1 clock*********/
        RCC_TIM1_CLK_ENABLE();
        /******************DISABLE timer1**************/
        TIMER1_DISABLE();
        /******************DISABLE timer1 int**************/
        //retval=Disable_timer1_interrupt(void);
				/**************initialize gpio pin***********/
				#if TIMER1_MODE==TIMER_PWM_MODE
				retval=pwm_pin_init(timer1_ptr);
				#endif
        /************set mode************************/
        retval=set_timer1_mode_preload_prescale(timer1_ptr);
				/*************set callback functions**********/
				retval=timer1_handle_interrupts(timer1_ptr);
        /***********Enable Interrupts****************/
        retval=Enable_timer1_interrupt();
			
				//setting the UG bit to update the registers before counting
				TIMER1_GENERATE_UPDATE();
        /***********Enable timer1********************/
        TIMER1_ENABLE();
        
    }
    return retval;
}
Std_ReturnType_t timer1_deinit(timer1_config_t *timer1_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer1_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
				TIMER1_DISABLE();
		}
    return retval;
}


#if TIMER1_MODE==TIMER_TIMER_MODE

Std_ReturnType_t timer1_timeBase_write(timer1_config_t *timer1_ptr,uint32_t time_base){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer1_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
								timer1_resolution=1;
                timer1_max_count=TIMER1_COUNT_REG_MAX*timer1_resolution;
                while(timer1_max_count<time_base)
                {
                    timer1_resolution++;
                    prescaler_value=TIMER1_CLKIN_FREQ*timer1_resolution;
                    timer1_max_count=TIMER1_COUNT_REG_MAX*timer1_resolution;
                }
                preload_value=time_base/timer1_resolution;
                TIM1->PSC=prescaler_value;
                TIM1->ARR=preload_value;
								timer1_ptr->time_base_us=time_base;
	}			
	return retval;

	}

Std_ReturnType_t timer1_delay(timer1_config_t *timer1_ptr,uint32_t delay_time)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer1_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
			uint32_t temp=timer1_ptr->time_base_us;
			timer1_timeBase_write(timer1_ptr,delay_time);
			while(!delay){}
			delay=0;
			timer1_timeBase_write(timer1_ptr,temp);
	}			
	return retval;

}
#endif

uint64_t timer1_millis()
{
	
	if(timer1_millis_counter==0)
	{
		tim1.TIM1_OF_InterruptHandler=timer1_isr;
		timer1_millis_counter++;
		timer1_init(&tim1);
	}
	return _miLLis__;
}
#if TIMER1_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer1_pwm_write(timer1_config_t *timer1_ptr,uint32_t duty_cycle){
			Std_ReturnType_t retval=E_OK;
			switch(timer1_ptr->channel){
				case TIMER1_CH1:
				TIM1_CCR1=duty_cycle;
				break;
				case TIMER1_CH2:
				TIM1_CCR2=duty_cycle;
				break;
				case TIMER1_CH3:
				TIM1_CCR3=duty_cycle;
				break;
				case TIMER1_CH4:
				TIM1_CCR4=duty_cycle;
				break;
				default:retval=E_NOT_OK;
			}
			return retval;
}
#endif
/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/

#if TIMER1_MODE==TIMER_PWM_MODE
static Std_ReturnType_t pwm_pin_init(timer1_config_t *timer1_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer1_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		Pin_Config_t PWM_pin;
		PWM_pin.port_index=TIMER1_CH_PORT;
		PWM_pin.mode=OUTPUT_MODE_50;
		PWM_pin.conf=AFO_PP;
		switch(timer1_ptr->channel)
		{
			case TIMER1_CH1:
			PWM_pin.pin_index=TIMER1_CH1_PIN;
			break;
			case TIMER1_CH2:
			PWM_pin.pin_index=TIMER1_CH2_PIN;
			break;
			case TIMER1_CH3:
			PWM_pin.pin_index=TIMER1_CH3_PIN;
			break;
			case TIMER1_CH4:
			PWM_pin.pin_index=TIMER1_CH4_PIN;
			break;
			default:retval=E_NOT_OK;
		}
		retval=gpio_pin_init(&PWM_pin);
	}
	return retval;
}
#endif
static Std_ReturnType_t set_timer1_mode_preload_prescale(timer1_config_t *timer1_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer1_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
       #if TIMER1_MODE==TIMER_TIMER_MODE
								timer1_resolution=1;
                timer1_max_count=TIMER1_COUNT_REG_MAX*timer1_resolution;
                while(timer1_max_count<timer1_ptr->time_base_us)
                {
                    timer1_resolution++;
                    prescaler_value=TIMER1_CLKIN_FREQ*timer1_resolution;
                    timer1_max_count=TIMER1_COUNT_REG_MAX*timer1_resolution;
                }
                preload_value=timer1_ptr->time_base_us/timer1_resolution;
                TIM1->PSC=prescaler_value;
                TIM1->ARR=preload_value;
            
        #endif

        #if TIMER1_MODE==TIMER_PWM_MODE
                timer1_resolution=1;
                timer1_max_count=TIMER1_COUNT_REG_MAX*timer1_resolution;
                while(timer1_max_count<timer1_ptr->PWM_frequency)
                {
                    timer1_resolution++;
                    prescaler_value=TIMER1_CLKIN_FREQ*timer1_resolution;
                    timer1_max_count=TIMER1_COUNT_REG_MAX*timer1_resolution;
                }
                preload_value=timer1_ptr->PWM_frequency/timer1_resolution;
                TIM1_PSC=prescaler_value;
                TIM1_ARR=preload_value;
								
								/*************888enable autoreload register8******/
              TIMER1_AUTO_RELOAD_ENABLE(void);
							switch(timer1_ptr->channel)
							{
								case TIMER1_CH1 :
								/**************output_enable****/
								TIMER1_CH1_ENABLE_OUTPUT(void);
                /*****timer1 ch1 PWM select******/
                TIMER1_CH1_SELECT_PWM_MODE(void);	
								/*************enable preload******/
								TIMER1_CH1_CCP_ENABLE_PRELOAD(void);
								TIM1_CCR1=timer1_ptr->duty_cycle;
								break;
								
								case TIMER1_CH2 :
								/**************output_enable****/
								TIMER1_CH2_ENABLE_OUTPUT(void);
                /*****timer1 ch2 PWM select******/
                TIMER1_CH2_SELECT_PWM_MODE(void);	
								/*************enable preload******/
								TIMER1_CH2_CCP_ENABLE_PRELOAD(void);
								TIM1_CCR2=timer1_ptr->duty_cycle;
								break;
								case TIMER1_CH3 :
								/**************output_enable****/
								TIMER1_CH3_ENABLE_OUTPUT(void);
                /*****timer1 ch1 PWM select******/
                TIMER1_CH3_SELECT_PWM_MODE(void);	
								/*************enable preload******/
								TIMER1_CH3_CCP_ENABLE_PRELOAD(void);
								TIM1_CCR3=timer1_ptr->duty_cycle;
								break;
							
								case TIMER1_CH4 :
								/**************output_enable****/
								TIMER1_CH4_ENABLE_OUTPUT(void);
                /*****timer1 ch1 PWM select******/
                TIMER1_CH4_SELECT_PWM_MODE(void);	
								/*************enable preload******/
								TIMER1_CH4_CCP_ENABLE_PRELOAD(void);
								TIM1_CCR4=timer1_ptr->duty_cycle;
								break;
								default:retval=E_NOT_OK;
							}
							
							   return retval;

#endif
}
}

            
    


static Std_ReturnType_t Enable_timer1_interrupt(){
    Std_ReturnType_t retval=E_OK;
    #if TIMER1_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER1_UPDATE_INTERRUPT_ENABLE();
        #if TIMER1_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER1_CC1_INTERRUPT_ENABLE(void);
            TIMER1_CC2_INTERRUPT_ENABLE(void);
            TIMER1_CC3_INTERRUPT_ENABLE(void);
            TIMER1_CC4_INTERRUPT_ENABLE(void);
        #endif
        SET_BIT(NVIC->ISER[0],NVIC_ISER_SETENA_25);
    #endif

    return retval;
}

static Std_ReturnType_t Disable_timer1_interrupt(){
	
    Std_ReturnType_t retval=E_OK;
    #if TIMER1_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
        #if TIMER1_MODE==TIMER_TIMER_MODE
            TIMER1_UPDATE_INTERRUPT_DISABLE();
        #endif
        #if TIMER1_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER1_CC1_INTERRUPT_DISABLE(void);
            TIMER1_CC2_INTERRUPT_DISABLE(void);
            TIMER1_CC3_INTERRUPT_DISABLE(void);
            TIMER1_CC4_INTERRUPT_DISABLE(void);
        #endif
        SET_BIT(NVIC->ICER[0],NVIC_ISER_SETENA_25);
    #endif

    return retval;
}

static Std_ReturnType_t	timer1_handle_interrupts(timer1_config_t	*timer1_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer1_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		#if TIMER1_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
						#if TIMER1_MODE==TIMER_TIMER_MODE
						TIM1_OF_InterruptHandler=timer1_ptr->TIM1_OF_InterruptHandler;
						#endif
				#if TIMER1_CCP_INTERRUPT_FEATURE==TIMER_PWM_MODE
						TIM1_CCP1_InterruptHandler=timer1_ptr->TIM1_CCP1_InterruptHandler;
						TIM1_CCP2_InterruptHandler=timer1_ptr->TIM1_CCP2_InterruptHandler;
						TIM1_CCP3_InterruptHandler=timer1_ptr->TIM1_CCP3_InterruptHandler;
						TIM1_CCP4_InterruptHandler=timer1_ptr->TIM1_CCP4_InterruptHandler;
				#endif
		#endif
	}
	return retval;
}

#if TIMER1_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
void	TIM1_UP_IRQHandler(void){
	SET_BIT(NVIC->ICPR[0],NVIC_ISER_SETENA_25);	
	if(READ_BIT(TIM1->SR,TIM_SR_UIF))
		{
			TIMER1_CLEAR_FLAG();
			#if TIMER1_MODE==TIMER_TIMER_MODE
			delay=1;
			#endif
			if(TIM1_OF_InterruptHandler){
					TIM1_OF_InterruptHandler();
			}
		}
}



void timer1_isr(void){
	_miLLis__++;
}

void TIMER1_ENABLE(void){
	(SET_BIT(TIM1->CR1,TIM_CR1_CEN));
}
void TIMER1_DISABLE(void){
	(CLEAR_BIT(TIM1->CR1,TIM_CR1_CEN));
}
void TIMER1_UEV_ENABLE(void){
	(CLEAR_BIT(TIM1->CR1,TIM_CR1_UDIS));
}
void TIMER1_UEV_DISABLE(void){
	(SET_BIT(TIM1->CR1,TIM_CR1_UDIS));
}
	
void TIMER1_URS_ALL(void){
	(CLEAR_BIT(TIM1->CR1,TIM_CR1_URS));
}
void TIMER1_URS_OVERFLOW(void){
	(SET_BIT(TIM1->CR1,TIM_CR1_URS));
}
	
void TIMER1_SET_UP_COUNTER(void){
	(CLEAR_BIT(TIM1->CR1,TIM_CR1_DIR));
}
	
void TIMER1_SET_DOWN_COUNTER(void){
	(SET_BIT(TIM1->CR1,TIM_CR1_DIR));
}
	
void TIMER1_AUTO_RELOAD_ENABLE(void){
	(SET_BIT(TIM1->CR1,TIM_CR1_ARPE));
}
	
void TIMER1_AUTO_RELOAD_DISABLE(void){
	(CLEAR_BIT(TIM1->CR1,TIM_CR1_ARPE));
}
	
void TIMER1_COUNTER_MODE_ENABLE(void){
	(TIM1->SMCR|=0x0007);
}
	
void TIMER1_TIMER_MODE_ENABLE(void){
	(TIM1->SMCR&=0X0000);
}
	
void TIMER1_SELECT_EDGE_ALIGNED_MODE(void){
	(TIM1->CR1&=0XFF9F);
}
	
void TIMER1_GENERATE_UPDATE(void){
	(SET_BIT(TIM1->EGR,TIM_EGR_UG));
}

//TIMER1 CHANNEL_1 MACROS
void TIMER1_CH1_SELECT_OUTPUT_MODE(void){
	(TIM1->CCMR1&=0XFFFC);
}
void TIMER1_CCP_CH1_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM1->CCMR1,TIM_CCMR1_CC1S_1);
    SET_BIT(TIM1->CCMR1,TIM_CCMR1_CC1S_0);
}

void TIMER1_COUNTER_CH1_RISING_EDGE(void){
	CLEAR_BIT(TIM1->CCER,TIM_CCER_CC1P);
}
	
void TIMER1_CH1_CCP_DISABLE_PRELOAD(void){
	CLEAR_BIT(TIM1->CCMR1,TIM_CCMR1_OC1PE);
}
	
void TIMER1_CH1_CCP_ENABLE_PRELOAD(void){
	SET_BIT(TIM1->CCMR1,TIM_CCMR1_OC1PE);
}
	
void TIMER1_CH1_CCP_DISABLE(void){
	(TIM1->CCMR1&=0XFF8F);
}
	
void TIMER1_CH1_ENABLE_OUTPUT(void){
  SET_BIT(TIM1->BDTR,TIM_BDTR_MOE);
	SET_BIT(TIM1->CCER,TIM_CCER_CC1E);
}

void TIMER1_CH1_SELECT_PWM_MODE(void){
    TIM1->CCMR1&=0xFF8F;
    TIM1->CCMR1|=0x0060;
}



//TIMER1 CHANNEL_2 MACROS
void TIMER1_CH2_SELECT_OUTPUT_MODE(void){
	(TIM1->CCMR1&=0XFCFF);
}
	
void TIMER1_CH2_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM1->CCMR1,TIM_CCMR1_CC2S_1);
    SET_BIT(TIM1->CCMR1,TIM_CCMR1_CC2S_0);
}
void TIMER1_COUNTER_CH2_RISING_EDGE(void){
	CLEAR_BIT(TIM1->CCER,TIM_CCER_CC2P);
}
	
void TIMER1_CH2_CCP_DISABLE_PRELOAD(void){
	(CLEAR_BIT(TIM1->CCMR1,TIM_CCMR1_OC2PE));
}
	
void TIMER1_CH2_CCP_ENABLE_PRELOAD(void){
	(SET_BIT(TIM1->CCMR1,TIM_CCMR1_OC2PE));
}
	
void TIMER1_CH2_CCP_DISABLE(void){
	(TIM1->CCMR1&=0X8FFF);
}
void TIMER1_CH2_ENABLE_OUTPUT(void){
  SET_BIT(TIM1->BDTR,TIM_BDTR_MOE);
	SET_BIT(TIM1->CCER,TIM_CCER_CC2E);
}
void TIMER1_CH2_SELECT_PWM_MODE(void){
    TIM1->CCMR1&=0x8FFF;
    TIM1->CCMR1|=0x6000;
}




//TIMER1 CHANNEL_3 MACROS
void TIMER1_CH3_SELECT_OUTPUT_MODE(void){
	(TIM1->CCMR2&=0XFFFC);
}
void TIMER1_CH3_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM1->CCMR2,TIM_CCMR2_CC3S_1);
    SET_BIT(TIM1->CCMR2,TIM_CCMR2_CC3S_0);
}
void TIMER1_COUNTER_CH3_RISING_EDGE(void){
	CLEAR_BIT(TIM1->CCER,TIM_CCER_CC3P);
}

void TIMER1_CH3_CCP_DISABLE_PRELOAD(void){
	(CLEAR_BIT(TIM1->CCMR2,TIM_CCMR2_OC3PE));
}
	
void TIMER1_CH3_CCP_ENABLE_PRELOAD(void){
	(SET_BIT(TIM1->CCMR2,TIM_CCMR2_OC3PE));
}
	
void TIMER1_CH3_CCP_DISABLE(void){
	TIM1->CCMR2&=0XFF8F;
}
	
void TIMER1_CH3_ENABLE_OUTPUT(void){
  SET_BIT(TIM1->BDTR,TIM_BDTR_MOE);
	SET_BIT(TIM1->CCER,TIM_CCER_CC3E);
}
void TIMER1_CH3_SELECT_PWM_MODE(void){
    TIM1->CCMR2&=0xFF8F;\
    TIM1->CCMR2|=0x0060;\
}


//TIMER1 CHANNEL_4 MACROS
void TIMER1_CH4_SELECT_OUTPUT_MODE(void){
	TIM1->CCMR2&=0XFCFF;
}
void TIMER1_CH4_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM1->CCMR2,TIM_CCMR2_CC4S_1);
    SET_BIT(TIM1->CCMR2,TIM_CCMR2_CC4S_0);
}
void TIMER1_COUNTER_CH4_RISING_EDGE(void){
	CLEAR_BIT(TIM1->CCER,TIM_CCER_CC4P);
}
	
void TIMER1_CH4_CCP_DISABLE_PRELOAD(void){
	(CLEAR_BIT(TIM1->CCMR2,TIM_CCMR2_OC4PE));
}
	
void TIMER1_CH4_CCP_ENABLE_PRELOAD(void){
	(SET_BIT(TIM1->CCMR2,TIM_CCMR2_OC4PE));
}
	
void TIMER1_CH4_CCP_DISABLE(void){
	TIM1->CCMR2&=0X8FFF;
}
void TIMER1_CH4_ENABLE_OUTPUT(void){
  SET_BIT(TIM1->BDTR,TIM_BDTR_MOE);
	SET_BIT(TIM1->CCER,TIM_CCER_CC4E);
}
void TIMER1_CH4_SELECT_PWM_MODE(void){
    TIM1->CCMR2&=0x8FFF;
    TIM1->CCMR2|=0x6000;
}

#endif
/********************************HELPER FUNCTIONS DEFINITIONS SECTION END**********************/





/********************************FUNCTION DOCUMENTATION EXAMPLE*******************/

/*
@name: function_name
@param: function parameters
@sequence:
@description:function_description
@return:function output
*/


/********************************FUNCTION DOCUMENTATION EXAMPLE*******************/



/********************************EDIT DETAILS SECTION START***********************/
//EXAMPLE:
/**EDIT 1
@name: karim
@edit_date: 9/3/2024 ,2:39 pm
@details:changed function XXX , line XXX from XXX to XXX
@status: problem solved 
*/

