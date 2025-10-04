/**
@file: MCAL_TIMER2.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "MCAL_TIMER2.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
static uint32_t prescaler_value=TIMER2_CLKIN_FREQ;
static uint32_t preload_value;
static uint8_t  timer2_resolution;//in us
static uint32_t timer2_max_count;//in us
#if TIMER2_MODE==TIMER_TIMER_MODE
static uint32_t delay;
#endif

#if TIMER2_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
static void(*TIM2_OF_InterruptHandler)(void)=NULL;
	#if TIMER2_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
	static void(*TIM2_CCP1_InterruptHandler)(void)=NULL;
	static void(*TIM2_CCP2_InterruptHandler)(void)=NULL;
	static void(*TIM2_CCP3_InterruptHandler)(void)=NULL;
	static void(*TIM2_CCP4_InterruptHandler)(void)=NULL;
	#endif
#endif

/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
#if TIMER2_MODE==TIMER_PWM_MODE
static Std_ReturnType_t pwm_pin_init(timer2_config_t *timer2_ptr);
#endif
static Std_ReturnType_t set_timer2_mode_preload_prescale(timer2_config_t *timer2_ptr);
static Std_ReturnType_t Enable_timer2_interrupt(void);
static Std_ReturnType_t Disable_timer2_interrupt(void);
static Std_ReturnType_t	timer2_handle_interrupts(timer2_config_t	*timer2_ptr);



//TIMER2 GENERAL MACROS
static void TIMER2_ENABLE(void);
static void TIMER2_DISABLE(void);
static void TIMER2_UEV_ENABLE(void);
static void TIMER2_UEV_DISABLE(void);
static void TIMER2_URS_ALL(void);
static void TIMER2_URS_OVERFLOW(void);
static void TIMER2_SET_UP_COUNTER(void);
static void TIMER2_SET_DOWN_COUNTER(void);
static void TIMER2_AUTO_RELOAD_ENABLE(void);
static void TIMER2_AUTO_RELOAD_DISABLE(void);
static void TIMER2_COUNTER_MODE_ENABLE(void);
static void TIMER2_TIMER_MODE_ENABLE(void);
static void TIMER2_SELECT_EDGE_ALIGNED_MODE(void);
static void TIMER2_GENERATE_UPDATE(void);


//TIMER2 CHANNEL_1 MACROS
static void TIMER2_CH1_SELECT_OUTPUT_MODE(void);
static void TIMER2_CCP_CH1_SELECT_INPUT_MODE(void);
static void TIMER2_COUNTER_CH1_RISING_EDGE(void);
static void TIMER2_CH1_CCP_DISABLE_PRELOAD(void);
static void TIMER2_CH1_CCP_ENABLE_PRELOAD(void);
static void TIMER2_CH1_CCP_DISABLE(void);
static void TIMER2_CH1_ENABLE_OUTPUT(void);

static void TIMER2_CH1_SELECT_PWM_MODE(void);


//TIMER2 CHANNEL_2 MACROS
static void TIMER2_CH2_SELECT_OUTPUT_MODE(void);
static void TIMER2_CH2_SELECT_INPUT_MODE(void);
static void TIMER2_COUNTER_CH2_RISING_EDGE(void);
static void TIMER2_CH2_CCP_DISABLE_PRELOAD(void);
static void TIMER2_CH2_CCP_ENABLE_PRELOAD(void);
static void TIMER2_CH2_CCP_DISABLE(void);
static void TIMER2_CH2_ENABLE_OUTPUT(void);
static void TIMER2_CH2_SELECT_PWM_MODE(void);




//TIMER2 CHANNEL_3 MACROS
static void TIMER2_CH3_SELECT_OUTPUT_MODE(void);
static void TIMER2_CH3_SELECT_INPUT_MODE(void);
static void TIMER2_COUNTER_CH3_RISING_EDGE(void);
static void TIMER2_CH3_CCP_DISABLE_PRELOAD(void);
static void TIMER2_CH3_CCP_ENABLE_PRELOAD(void);
static void TIMER2_CH3_CCP_DISABLE(void);
static void TIMER2_CH3_ENABLE_OUTPUT(void);
static void TIMER2_CH3_SELECT_PWM_MODE(void);

//TIMER2 CHANNEL_4 MACROS
static void TIMER2_CH4_SELECT_OUTPUT_MODE(void);
static void TIMER2_CH4_SELECT_INPUT_MODE(void);
static void TIMER2_COUNTER_CH4_RISING_EDGE(void);
static void TIMER2_CH4_CCP_DISABLE_PRELOAD(void);
static void TIMER2_CH4_CCP_ENABLE_PRELOAD(void);
static void TIMER2_CH4_CCP_DISABLE(void);
static void TIMER2_CH4_ENABLE_OUTPUT(void);
static void TIMER2_CH4_SELECT_PWM_MODE(void);
/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t timer2_init(timer2_config_t *timer2_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer2_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
        /******************enable timer2 clock*********/
        RCC_TIM2_CLK_ENABLE();
        /******************DISABLE timer2**************/
        TIMER2_DISABLE();
        /******************DISABLE timer2 int**************/
        //retval=Disable_timer2_interrupt();
				/**************initialize gpio pin***********/
				#if TIMER2_MODE==TIMER_PWM_MODE
				retval=pwm_pin_init(timer2_ptr);
				#endif
        /************set mode************************/
        retval=set_timer2_mode_preload_prescale(timer2_ptr);
				/*************set callback functions**********/
				retval=timer2_handle_interrupts(timer2_ptr);
        /***********Enable Interrupts****************/
        retval=Enable_timer2_interrupt();
			
				//setting the UG bit to update the registers before counting
				TIMER2_GENERATE_UPDATE();
        /***********Enable timer2********************/
        TIMER2_ENABLE();
        
    }
    return retval;
}
Std_ReturnType_t timer2_deinit(timer2_config_t *timer2_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer2_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
				TIMER2_DISABLE();
		}
    return retval;
}


#if TIMER2_MODE==TIMER_TIMER_MODE

Std_ReturnType_t timer2_timeBase_write(timer2_config_t *timer2_ptr,uint32_t time_base){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
								timer2_resolution=1;
                timer2_max_count=TIMER2_COUNT_REG_MAX*timer2_resolution;
                while(timer2_max_count<time_base)
                {
                    timer2_resolution++;
                    prescaler_value=TIMER2_CLKIN_FREQ*timer2_resolution;
                    timer2_max_count=TIMER2_COUNT_REG_MAX*timer2_resolution;
                }
                preload_value=time_base/timer2_resolution;
                TIM2->PSC=prescaler_value;
                TIM2->ARR=preload_value;
								timer2_ptr->time_base_us=time_base;
	}			
	return retval;

	}

Std_ReturnType_t timer2_delay(timer2_config_t *timer2_ptr,uint32_t delay_time)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
			uint32_t temp=timer2_ptr->time_base_us;
			timer2_timeBase_write(timer2_ptr,delay_time);
			delay=0;
			while(!delay){}
			timer2_timeBase_write(timer2_ptr,temp);
	}			
	return retval;

}
#endif

#if TIMER2_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer2_pwm_write(timer2_config_t *timer2_ptr,uint16_t duty_cycle){
			Std_ReturnType_t retval=E_OK;
			switch(timer2_ptr->channel){
				case TIMER2_CH1:
				TIM2->CCR1=duty_cycle;
				break;
				case TIMER2_CH2:
				TIM2->CCR2=duty_cycle;
				break;
				case TIMER2_CH3:
				TIM2->CCR3=duty_cycle;
				break;
				case TIMER2_CH4:
				TIM2->CCR4=duty_cycle;
				break;
				default:retval=E_NOT_OK;
			}
			return retval;
}
#endif
/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/

#if TIMER2_MODE==TIMER_PWM_MODE
static Std_ReturnType_t pwm_pin_init(timer2_config_t *timer2_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		Pin_Config_t PWM_pin;
		PWM_pin.port_index=TIMER2_CH_PORT;
		PWM_pin.mode=OUTPUT_MODE_50;
		PWM_pin.conf=AFO_PP;
		switch(timer2_ptr->channel)
		{
			case TIMER2_CH1:
			PWM_pin.pin_index=TIMER2_CH1_PIN;
			break;
			case TIMER2_CH2:
			PWM_pin.pin_index=TIMER2_CH2_PIN;
			break;
			case TIMER2_CH3:
			PWM_pin.pin_index=TIMER2_CH3_PIN;
			break;
			case TIMER2_CH4:
			PWM_pin.pin_index=TIMER2_CH4_PIN;
			break;
			default:retval=E_NOT_OK;
		}
		retval=gpio_pin_init(&PWM_pin);
	}
	return retval;
}
#endif
static Std_ReturnType_t set_timer2_mode_preload_prescale(timer2_config_t *timer2_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer2_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
       #if TIMER2_MODE==TIMER_TIMER_MODE
								timer2_resolution=1;
                timer2_max_count=TIMER2_COUNT_REG_MAX*timer2_resolution;
                while(timer2_max_count<timer2_ptr->time_base_us)
                {
                    timer2_resolution++;
                    prescaler_value=TIMER2_CLKIN_FREQ*timer2_resolution;
                    timer2_max_count=TIMER2_COUNT_REG_MAX*timer2_resolution;
                }
                preload_value=timer2_ptr->time_base_us/timer2_resolution;
                TIM2->PSC=prescaler_value;
                TIM2->ARR=preload_value;
            
        #endif

        #if TIMER2_MODE==TIMER_PWM_MODE
                timer2_resolution=1;
                timer2_max_count=TIMER2_COUNT_REG_MAX*timer2_resolution;
                while(timer2_max_count<timer2_ptr->PWM_frequency)
                {
                    timer2_resolution++;
                    prescaler_value=TIMER2_CLKIN_FREQ*timer2_resolution;
                    timer2_max_count=TIMER2_COUNT_REG_MAX*timer2_resolution;
                }
                preload_value=timer2_ptr->PWM_frequency/timer2_resolution;
                TIM2->PSC=prescaler_value;
                TIM2->ARR=preload_value;
								
								/*************888enable autoreload register8******/
              TIMER2_AUTO_RELOAD_ENABLE();
							switch(timer2_ptr->channel)
							{
								case TIMER2_CH1 :
								/**************output_enable****/
								TIMER2_CH1_ENABLE_OUTPUT();
                /*****timer2 ch1 PWM select******/
                TIMER2_CH1_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER2_CH1_CCP_ENABLE_PRELOAD();
								TIM2->CCR1=timer2_ptr->duty_cycle;
								break;
								
								case TIMER2_CH2 :
								/**************output_enable****/
								TIMER2_CH2_ENABLE_OUTPUT();
                /*****timer2 ch2 PWM select******/
                TIMER2_CH2_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER2_CH2_CCP_ENABLE_PRELOAD();
								TIM2->CCR2=timer2_ptr->duty_cycle;
								break;
								case TIMER2_CH3 :
								/**************output_enable****/
								TIMER2_CH3_ENABLE_OUTPUT();
                /*****timer2 ch1 PWM select******/
                TIMER2_CH3_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER2_CH3_CCP_ENABLE_PRELOAD();
								TIM2->CCR3=timer2_ptr->duty_cycle;
								break;
							
								case TIMER2_CH4 :
								/**************output_enable****/
								TIMER2_CH4_ENABLE_OUTPUT();
                /*****timer2 ch1 PWM select******/
                TIMER2_CH4_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER2_CH4_CCP_ENABLE_PRELOAD();
								TIM2->CCR4=timer2_ptr->duty_cycle;
								break;
								default:retval=E_NOT_OK;
							}
							
							   return retval;

#endif
}
}

            
    


static Std_ReturnType_t Enable_timer2_interrupt(void){
    Std_ReturnType_t retval=E_OK;
    #if TIMER2_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER2_UPDATE_INTERRUPT_ENABLE();
        #if TIMER2_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER2_CC1_INTERRUPT_ENABLE();
            TIMER2_CC2_INTERRUPT_ENABLE();
            TIMER2_CC3_INTERRUPT_ENABLE();
            TIMER2_CC4_INTERRUPT_ENABLE();
        #endif
        SET_BIT(NVIC->ISER[0],NVIC_ISER_SETENA_28);
    #endif

    return retval;
}

static Std_ReturnType_t Disable_timer2_interrupt(void){
	
    Std_ReturnType_t retval=E_OK;
    #if TIMER2_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
        #if TIMER2_MODE==TIMER_TIMER_MODE
            TIMER2_UPDATE_INTERRUPT_DISABLE();
        #endif
        #if TIMER2_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER2_CC1_INTERRUPT_DISABLE();
            TIMER2_CC2_INTERRUPT_DISABLE();
            TIMER2_CC3_INTERRUPT_DISABLE();
            TIMER2_CC4_INTERRUPT_DISABLE();
        #endif
        SET_BIT(NVIC->ICER[0],NVIC_ISER_SETENA_28);
    #endif

    return retval;
}

static Std_ReturnType_t	timer2_handle_interrupts(timer2_config_t	*timer2_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		#if TIMER2_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
						#if TIMER2_MODE==TIMER_TIMER_MODE
						TIM2_OF_InterruptHandler=timer2_ptr->TIM2_OF_InterruptHandler;
						#endif
				#if TIMER2_CCP_INTERRUPT_FEATURE==TIMER_PWM_MODE
						TIM2_CCP1_InterruptHandler=timer2_ptr->TIM2_CCP1_InterruptHandler;
						TIM2_CCP2_InterruptHandler=timer2_ptr->TIM2_CCP2_InterruptHandler;
						TIM2_CCP3_InterruptHandler=timer2_ptr->TIM2_CCP3_InterruptHandler;
						TIM2_CCP4_InterruptHandler=timer2_ptr->TIM2_CCP4_InterruptHandler;
				#endif
		#endif
	}
	return retval;
}

#if TIMER2_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
void	TIM2_IRQHandler(void){
	SET_BIT(NVIC->ICPR[0],NVIC_ISER_SETENA_28);	
	if(READ_BIT(TIM2->SR,TIM_SR_UIF))
		{
			TIMER2_CLEAR_FLAG();
			#if TIMER2_MODE==TIMER_TIMER_MODE
			delay=1;
			#endif
			if(TIM2_OF_InterruptHandler){
					
					TIM2_OF_InterruptHandler();
			}
		}
}
#endif

//TIMER2 GENERAL MACROS
static void TIMER2_ENABLE(void)
{	
	SET_BIT(TIM2->CR1,TIM_CR1_CEN);
}
static void TIMER2_DISABLE(void)	
{	
	CLEAR_BIT(TIM2->CR1,TIM_CR1_CEN);
}
static void TIMER2_UEV_ENABLE(void)	
{	
	CLEAR_BIT(TIM2->CR1,TIM_CR1_UDIS);
}
static void TIMER2_UEV_DISABLE(void)	  
{	
	SET_BIT(TIM2->CR1,TIM_CR1_UDIS);
}
static void TIMER2_URS_ALL(void){
	CLEAR_BIT(TIM2->CR1,TIM_CR1_URS);
}
static void TIMER2_URS_OVERFLOW(void){
	SET_BIT(TIM2->CR1,TIM_CR1_URS);
}
static void TIMER2_SET_UP_COUNTER(void){
	CLEAR_BIT(TIM2->CR1,TIM_CR1_DIR);
}
static void TIMER2_SET_DOWN_COUNTER(void){
	SET_BIT(TIM2->CR1,TIM_CR1_DIR);
}
static void TIMER2_AUTO_RELOAD_ENABLE(void){
	SET_BIT(TIM2->CR1,TIM_CR1_ARPE);
}
static void TIMER2_AUTO_RELOAD_DISABLE(void){
	CLEAR_BIT(TIM2->CR1,TIM_CR1_ARPE);
}
static void TIMER2_COUNTER_MODE_ENABLE(void){
	TIM2->SMCR|=0x0007;
}
static void TIMER2_TIMER_MODE_ENABLE(void){
	TIM2->SMCR&=0X0000;
}
static void TIMER2_SELECT_EDGE_ALIGNED_MODE(void){
	TIM2->CR1&=0XFF9F;
}

static void TIMER2_GENERATE_UPDATE(void){
	SET_BIT(TIM2->EGR,TIM_EGR_UG);
}

//TIMER2 CHANNEL_1 MACROS
static void TIMER2_CH1_SELECT_OUTPUT_MODE(void){
	TIM2->CCMR1&=0XFFFC;
}
static void TIMER2_CCP_CH1_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM2->CCMR1,TIM_CCMR1_CC1S_1);
    SET_BIT(TIM2->CCMR1,TIM_CCMR1_CC1S_0);
}
static void TIMER2_COUNTER_CH1_RISING_EDGE(void){
	CLEAR_BIT(TIM2->CCER,TIM_CCER_CC1P);
}
static void TIMER2_CH1_CCP_DISABLE_PRELOAD(void){
	CLEAR_BIT(TIM2->CCMR1,TIM_CCMR1_OC1PE);
}
static void TIMER2_CH1_CCP_ENABLE_PRELOAD(void){
	SET_BIT(TIM2->CCMR1,TIM_CCMR1_OC1PE);
}
static void TIMER2_CH1_CCP_DISABLE(void){
	TIM2->CCMR1&=0XFF8F;
}
static void TIMER2_CH1_ENABLE_OUTPUT(void){
	SET_BIT(TIM2->CCER,TIM_CCER_CC1E);
}
static void TIMER2_CH1_SELECT_PWM_MODE(void){
    TIM2->CCMR1&=0xFF8F;
    TIM2->CCMR1|=0x0060;
}



//TIMER2 CHANNEL_2 MACROS
static void TIMER2_CH2_SELECT_OUTPUT_MODE(void){
	TIM2->CCMR1&=0XFCFF;
}
static void TIMER2_CH2_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM2->CCMR1,TIM_CCMR1_CC2S_1);
    SET_BIT(TIM2->CCMR1,TIM_CCMR1_CC2S_0);
}

static void TIMER2_COUNTER_CH2_RISING_EDGE(void){
	CLEAR_BIT(TIM2->CCER,TIM_CCER_CC1P);
}
static void TIMER2_CH2_CCP_DISABLE_PRELOAD(void){
	CLEAR_BIT(TIM2->CCMR1,TIM_CCMR1_OC2PE);
}
static void TIMER2_CH2_CCP_ENABLE_PRELOAD(void){
	SET_BIT(TIM2->CCMR1,TIM_CCMR1_OC2PE);
}
static void TIMER2_CH2_CCP_DISABLE(void){
	TIM2->CCMR1&=0X8FFF;
}
static void TIMER2_CH2_ENABLE_OUTPUT(void){
	SET_BIT(TIM2->CCER,TIM_CCER_CC2E);
}
static void TIMER2_CH2_SELECT_PWM_MODE(void){
    TIM2->CCMR1&=0x8FFF;
    TIM2->CCMR1|=0x6000;
}




//TIMER2 CHANNEL_3 MACROS
static void TIMER2_CH3_SELECT_OUTPUT_MODE(void){
		TIM2->CCMR2&=0XFFFC;
}
static void TIMER2_CH3_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM2->CCMR1,TIM_CCMR2_CC3S_1);
    SET_BIT(TIM2->CCMR1,TIM_CCMR2_CC3S_0);
}
static void TIMER2_COUNTER_CH3_RISING_EDGE(void){
	CLEAR_BIT(TIM2->CCER,TIM_CCER_CC3P);
}
static void TIMER2_CH3_CCP_DISABLE_PRELOAD(void){
	CLEAR_BIT(TIM2->CCMR2,TIM_CCMR2_OC3PE);
}
static void TIMER2_CH3_CCP_ENABLE_PRELOAD(void){
	SET_BIT(TIM2->CCMR2,TIM_CCMR2_OC3PE);
}
static void TIMER2_CH3_CCP_DISABLE(void){
	TIM2->CCMR2&=0XFF8F;
}
static void TIMER2_CH3_ENABLE_OUTPUT(void){
	SET_BIT(TIM2->CCER,TIM_CCER_CC3E);
}
static void TIMER2_CH3_SELECT_PWM_MODE(void){
    TIM2->CCMR2&=0xFF8F;
    TIM2->CCMR2|=0x0060;
}


//TIMER2 CHANNEL_4 MACROS
static void TIMER2_CH4_SELECT_OUTPUT_MODE(void){
	TIM2->CCMR2&=0XFCFF;
}
static void TIMER2_CH4_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM2->CCMR2,TIM_CCMR2_CC4S_1);
    SET_BIT(TIM2->CCMR2,TIM_CCMR2_CC4S_0);
}
static void TIMER2_COUNTER_CH4_RISING_EDGE(void){
	CLEAR_BIT(TIM2->CCER,TIM_CCER_CC4P);
}
static void TIMER2_CH4_CCP_DISABLE_PRELOAD(void){
	CLEAR_BIT(TIM2->CCMR2,TIM_CCMR2_OC4PE);
}
static void TIMER2_CH4_CCP_ENABLE_PRELOAD(void){
	SET_BIT(TIM2->CCMR2,TIM_CCMR2_OC4PE);
}
static void TIMER2_CH4_CCP_DISABLE(void){
	TIM2->CCMR2&=0X8FFF;
}
static void TIMER2_CH4_ENABLE_OUTPUT(void){
	SET_BIT(TIM2->CCER,TIM_CCER_CC4E);
}
static void TIMER2_CH4_SELECT_PWM_MODE(void){
    TIM2->CCMR2&=0x8FFF;
    TIM2->CCMR2|=0x6000;
}


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

