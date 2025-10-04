/**
@file: MCAL_TIMER4.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "MCAL_TIMER4.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
static uint32_t prescaler_value=TIMER4_CLKIN_FREQ;
static uint32_t preload_value;
static uint8_t  timer4_resolution;//in us
static uint32_t timer4_max_count;//in us
#if TIMER4_MODE==TIMER_TIMER_MODE
static uint32_t delay;
#endif

#if TIMER4_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
static void(*TIM4_OF_InterruptHandler)(void)=NULL;
	#if TIMER4_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
	static void(*TIM4_CCP1_InterruptHandler)(void)=NULL;
	static void(*TIM4_CCP2_InterruptHandler)(void)=NULL;
	static void(*TIM4_CCP3_InterruptHandler)(void)=NULL;
	static void(*TIM4_CCP4_InterruptHandler)(void)=NULL;
	#endif
#endif

/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
#if TIMER4_MODE==TIMER_PWM_MODE
static Std_ReturnType_t pwm_pin_init(timer4_config_t *timer4_ptr);
#endif
static Std_ReturnType_t set_timer4_mode_preload_prescale(timer4_config_t *timer4_ptr);
static Std_ReturnType_t Enable_timer4_interrupt(void);
static Std_ReturnType_t Disable_timer4_interrupt(void);
static Std_ReturnType_t	timer4_handle_interrupts(timer4_config_t	*timer4_ptr);



//TIMER4 GENERAL MACROS
static void TIMER4_ENABLE(void);
static void TIMER4_DISABLE(void);
static void TIMER4_UEV_ENABLE(void);
static void TIMER4_UEV_DISABLE(void);
static void TIMER4_URS_ALL(void);
static void TIMER4_URS_OVERFLOW(void);
static void TIMER4_SET_UP_COUNTER(void);
static void TIMER4_SET_DOWN_COUNTER(void);
static void TIMER4_AUTO_RELOAD_ENABLE(void);
static void TIMER4_AUTO_RELOAD_DISABLE(void);
static void TIMER4_COUNTER_MODE_ENABLE(void);
static void TIMER4_TIMER_MODE_ENABLE(void);
static void TIMER4_SELECT_EDGE_ALIGNED_MODE(void);
static void TIMER4_GENERATE_UPDATE(void);


//TIMER4 CHANNEL_1 MACROS
static void TIMER4_CH1_SELECT_OUTPUT_MODE(void);
static void TIMER4_CCP_CH1_SELECT_INPUT_MODE(void);
static void TIMER4_COUNTER_CH1_RISING_EDGE(void);
static void TIMER4_CH1_CCP_DISABLE_PRELOAD(void);
static void TIMER4_CH1_CCP_ENABLE_PRELOAD(void);
static void TIMER4_CH1_CCP_DISABLE(void);
static void TIMER4_CH1_ENABLE_OUTPUT(void);

static void TIMER4_CH1_SELECT_PWM_MODE(void);


//TIMER4 CHANNEL_2 MACROS
static void TIMER4_CH2_SELECT_OUTPUT_MODE(void);
static void TIMER4_CH2_SELECT_INPUT_MODE(void);
static void TIMER4_COUNTER_CH2_RISING_EDGE(void);
static void TIMER4_CH2_CCP_DISABLE_PRELOAD(void);
static void TIMER4_CH2_CCP_ENABLE_PRELOAD(void);
static void TIMER4_CH2_CCP_DISABLE(void);
static void TIMER4_CH2_ENABLE_OUTPUT(void);
static void TIMER4_CH2_SELECT_PWM_MODE(void);




//TIMER4 CHANNEL_3 MACROS
static void TIMER4_CH3_SELECT_OUTPUT_MODE(void);
static void TIMER4_CH3_SELECT_INPUT_MODE(void);
static void TIMER4_COUNTER_CH3_RISING_EDGE(void);
static void TIMER4_CH3_CCP_DISABLE_PRELOAD(void);
static void TIMER4_CH3_CCP_ENABLE_PRELOAD(void);
static void TIMER4_CH3_CCP_DISABLE(void);
static void TIMER4_CH3_ENABLE_OUTPUT(void);
static void TIMER4_CH3_SELECT_PWM_MODE(void);

//TIMER4 CHANNEL_4 MACROS
static void TIMER4_CH4_SELECT_OUTPUT_MODE(void);
static void TIMER4_CH4_SELECT_INPUT_MODE(void);
static void TIMER4_COUNTER_CH4_RISING_EDGE(void);
static void TIMER4_CH4_CCP_DISABLE_PRELOAD(void);
static void TIMER4_CH4_CCP_ENABLE_PRELOAD(void);
static void TIMER4_CH4_CCP_DISABLE(void);
static void TIMER4_CH4_ENABLE_OUTPUT(void);
static void TIMER4_CH4_SELECT_PWM_MODE(void);
/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t timer4_init(timer4_config_t *timer4_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer4_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
        /******************enable timer4 clock*********/
        RCC_TIM4_CLK_ENABLE();
        /******************DISABLE timer4**************/
        TIMER4_DISABLE();
        /******************DISABLE timer4 int**************/
        //retval=Disable_timer4_interrupt();
				/**************initialize gpio pin***********/
				#if TIMER4_MODE==TIMER_PWM_MODE
				retval=pwm_pin_init(timer4_ptr);
				#endif
        /************set mode************************/
        retval=set_timer4_mode_preload_prescale(timer4_ptr);
				/*************set callback functions**********/
				retval=timer4_handle_interrupts(timer4_ptr);
        /***********Enable Interrupts****************/
        retval=Enable_timer4_interrupt();
			
				//setting the UG bit to update the registers before counting
				TIMER4_GENERATE_UPDATE();
        /***********Enable timer4********************/
        TIMER4_ENABLE();
        
    }
    return retval;
}
Std_ReturnType_t timer4_deinit(timer4_config_t *timer4_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer4_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
				TIMER4_DISABLE();
		}
    return retval;
}


#if TIMER4_MODE==TIMER_TIMER_MODE

Std_ReturnType_t timer4_timeBase_write(timer4_config_t *timer4_ptr,uint32_t time_base){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer4_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
								timer4_resolution=1;
                timer4_max_count=TIMER4_COUNT_REG_MAX*timer4_resolution;
                while(timer4_max_count<time_base)
                {
                    timer4_resolution++;
                    prescaler_value=TIMER4_CLKIN_FREQ*timer4_resolution;
                    timer4_max_count=TIMER4_COUNT_REG_MAX*timer4_resolution;
                }
                preload_value=time_base/timer4_resolution;
                TIM4->PSC=prescaler_value;
                TIM4->ARR=preload_value;
								timer4_ptr->time_base_us=time_base;
	}			
	return retval;

	}

Std_ReturnType_t timer4_delay(timer4_config_t *timer4_ptr,uint32_t delay_time)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer4_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
			uint32_t temp=timer4_ptr->time_base_us;
			timer4_timeBase_write(timer4_ptr,delay_time);
			delay=0;
			while(!delay){}
			timer4_timeBase_write(timer4_ptr,temp);
	}			
	return retval;

}
#endif

#if TIMER4_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer4_pwm_write(timer4_config_t *timer4_ptr,uint16_t duty_cycle){
			Std_ReturnType_t retval=E_OK;
			switch(timer4_ptr->channel){
				case TIMER4_CH1:
				TIM4->CCR1=duty_cycle;
				break;
				case TIMER4_CH2:
				TIM4->CCR2=duty_cycle;
				break;
				case TIMER4_CH3:
				TIM4->CCR3=duty_cycle;
				break;
				case TIMER4_CH4:
				TIM4->CCR4=duty_cycle;
				break;
				default:retval=E_NOT_OK;
			}
			return retval;
}
#endif
/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/

#if TIMER4_MODE==TIMER_PWM_MODE
static Std_ReturnType_t pwm_pin_init(timer4_config_t *timer4_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer4_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		Pin_Config_t PWM_pin;
		PWM_pin.port_index=TIMER4_CH_PORT;
		PWM_pin.mode=OUTPUT_MODE_50;
		PWM_pin.conf=AFO_PP;
		switch(timer4_ptr->channel)
		{
			case TIMER4_CH1:
			PWM_pin.pin_index=TIMER4_CH1_PIN;
			break;
			case TIMER4_CH2:
			PWM_pin.pin_index=TIMER4_CH2_PIN;
			break;
			case TIMER4_CH3:
			PWM_pin.pin_index=TIMER4_CH3_PIN;
			break;
			case TIMER4_CH4:
			PWM_pin.pin_index=TIMER4_CH4_PIN;
			break;
			default:retval=E_NOT_OK;
		}
		retval=gpio_pin_init(&PWM_pin);
	}
	return retval;
}
#endif
static Std_ReturnType_t set_timer4_mode_preload_prescale(timer4_config_t *timer4_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer4_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
       #if TIMER4_MODE==TIMER_TIMER_MODE
								timer4_resolution=1;
                timer4_max_count=TIMER4_COUNT_REG_MAX*timer4_resolution;
                while(timer4_max_count<timer4_ptr->time_base_us)
                {
                    timer4_resolution++;
                    prescaler_value=TIMER4_CLKIN_FREQ*timer4_resolution;
                    timer4_max_count=TIMER4_COUNT_REG_MAX*timer4_resolution;
                }
                preload_value=timer4_ptr->time_base_us/timer4_resolution;
                TIM4->PSC=prescaler_value;
                TIM4->ARR=preload_value;
            
        #endif

        #if TIMER4_MODE==TIMER_PWM_MODE
                timer4_resolution=1;
                timer4_max_count=TIMER4_COUNT_REG_MAX*timer4_resolution;
                while(timer4_max_count<timer4_ptr->PWM_frequency)
                {
                    timer4_resolution++;
                    prescaler_value=TIMER4_CLKIN_FREQ*timer4_resolution;
                    timer4_max_count=TIMER4_COUNT_REG_MAX*timer4_resolution;
                }
                preload_value=timer4_ptr->PWM_frequency/timer4_resolution;
                TIM4->PSC=prescaler_value;
                TIM4->ARR=preload_value;
								
								/*************888enable autoreload register8******/
              TIMER4_AUTO_RELOAD_ENABLE();
							switch(timer4_ptr->channel)
							{
								case TIMER4_CH1 :
								/**************output_enable****/
								TIMER4_CH1_ENABLE_OUTPUT();
                /*****timer4 ch1 PWM select******/
                TIMER4_CH1_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER4_CH1_CCP_ENABLE_PRELOAD();
								TIM4->CCR1=timer4_ptr->duty_cycle;
								break;
								
								case TIMER4_CH2 :
								/**************output_enable****/
								TIMER4_CH2_ENABLE_OUTPUT();
                /*****timer4 ch2 PWM select******/
                TIMER4_CH2_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER4_CH2_CCP_ENABLE_PRELOAD();
								TIM4->CCR2=timer4_ptr->duty_cycle;
								break;
								case TIMER4_CH3 :
								/**************output_enable****/
								TIMER4_CH3_ENABLE_OUTPUT();
                /*****timer4 ch1 PWM select******/
                TIMER4_CH3_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER4_CH3_CCP_ENABLE_PRELOAD();
								TIM4->CCR3=timer4_ptr->duty_cycle;
								break;
							
								case TIMER4_CH4 :
								/**************output_enable****/
								TIMER4_CH4_ENABLE_OUTPUT();
                /*****timer4 ch1 PWM select******/
                TIMER4_CH4_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER4_CH4_CCP_ENABLE_PRELOAD();
								TIM4->CCR4=timer4_ptr->duty_cycle;
								break;
								default:retval=E_NOT_OK;
							}
							
							   return retval;

#endif
}
}

            
    


static Std_ReturnType_t Enable_timer4_interrupt(void){
    Std_ReturnType_t retval=E_OK;
    #if TIMER4_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER4_UPDATE_INTERRUPT_ENABLE();
        #if TIMER4_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER4_CC1_INTERRUPT_ENABLE();
            TIMER4_CC2_INTERRUPT_ENABLE();
            TIMER4_CC3_INTERRUPT_ENABLE();
            TIMER4_CC4_INTERRUPT_ENABLE();
        #endif
        SET_BIT(NVIC->ISER[0],NVIC_ISER_SETENA_30);
    #endif

    return retval;
}

static Std_ReturnType_t Disable_timer4_interrupt(void){
	
    Std_ReturnType_t retval=E_OK;
    #if TIMER4_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
        #if TIMER4_MODE==TIMER_TIMER_MODE
            TIMER4_UPDATE_INTERRUPT_DISABLE();
        #endif
        #if TIMER4_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER4_CC1_INTERRUPT_DISABLE();
            TIMER4_CC2_INTERRUPT_DISABLE();
            TIMER4_CC3_INTERRUPT_DISABLE();
            TIMER4_CC4_INTERRUPT_DISABLE();
        #endif
        SET_BIT(NVIC->ICER[0],NVIC_ISER_SETENA_30);
    #endif

    return retval;
}

static Std_ReturnType_t	timer4_handle_interrupts(timer4_config_t	*timer4_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer4_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		#if TIMER4_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
						#if TIMER4_MODE==TIMER_TIMER_MODE
						TIM4_OF_InterruptHandler=timer4_ptr->TIM4_OF_InterruptHandler;
						#endif
				#if TIMER4_CCP_INTERRUPT_FEATURE==TIMER_PWM_MODE
						TIM4_CCP1_InterruptHandler=timer4_ptr->TIM4_CCP1_InterruptHandler;
						TIM4_CCP2_InterruptHandler=timer4_ptr->TIM4_CCP2_InterruptHandler;
						TIM4_CCP3_InterruptHandler=timer4_ptr->TIM4_CCP3_InterruptHandler;
						TIM4_CCP4_InterruptHandler=timer4_ptr->TIM4_CCP4_InterruptHandler;
				#endif
		#endif
	}
	return retval;
}

#if TIMER4_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
void	TIM4_IRQHandler(void){
	SET_BIT(NVIC->ICPR[0],NVIC_ISER_SETENA_30);	
	if(READ_BIT(TIM4->SR,TIM_SR_UIF))
		{
			TIMER4_CLEAR_FLAG();
			#if TIMER4_MODE==TIMER_TIMER_MODE
			delay=1;
			#endif
			if(TIM4_OF_InterruptHandler){
					
					TIM4_OF_InterruptHandler();
			}
		}
}
#endif

//TIMER4 GENERAL MACROS
static void TIMER4_ENABLE(void)
{	
	SET_BIT(TIM4->CR1,TIM_CR1_CEN);
}
static void TIMER4_DISABLE(void)	
{	
	CLEAR_BIT(TIM4->CR1,TIM_CR1_CEN);
}
static void TIMER4_UEV_ENABLE(void)	
{	
	CLEAR_BIT(TIM4->CR1,TIM_CR1_UDIS);
}
static void TIMER4_UEV_DISABLE(void)	  
{	
	SET_BIT(TIM4->CR1,TIM_CR1_UDIS);
}
static void TIMER4_URS_ALL(void){
	CLEAR_BIT(TIM4->CR1,TIM_CR1_URS);
}
static void TIMER4_URS_OVERFLOW(void){
	SET_BIT(TIM4->CR1,TIM_CR1_URS);
}
static void TIMER4_SET_UP_COUNTER(void){
	CLEAR_BIT(TIM4->CR1,TIM_CR1_DIR);
}
static void TIMER4_SET_DOWN_COUNTER(void){
	SET_BIT(TIM4->CR1,TIM_CR1_DIR);
}
static void TIMER4_AUTO_RELOAD_ENABLE(void){
	SET_BIT(TIM4->CR1,TIM_CR1_ARPE);
}
static void TIMER4_AUTO_RELOAD_DISABLE(void){
	CLEAR_BIT(TIM4->CR1,TIM_CR1_ARPE);
}
static void TIMER4_COUNTER_MODE_ENABLE(void){
	TIM4->SMCR|=0x0007;
}
static void TIMER4_TIMER_MODE_ENABLE(void){
	TIM4->SMCR&=0X0000;
}
static void TIMER4_SELECT_EDGE_ALIGNED_MODE(void){
	TIM4->CR1&=0XFF9F;
}

static void TIMER4_GENERATE_UPDATE(void){
	SET_BIT(TIM4->EGR,TIM_EGR_UG);
}

//TIMER4 CHANNEL_1 MACROS
static void TIMER4_CH1_SELECT_OUTPUT_MODE(void){
	TIM4->CCMR1&=0XFFFC;
}
static void TIMER4_CCP_CH1_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM4->CCMR1,TIM_CCMR1_CC1S_1);
    SET_BIT(TIM4->CCMR1,TIM_CCMR1_CC1S_0);
}
static void TIMER4_COUNTER_CH1_RISING_EDGE(void){
	CLEAR_BIT(TIM4->CCER,TIM_CCER_CC1P);
}
static void TIMER4_CH1_CCP_DISABLE_PRELOAD(void){
	CLEAR_BIT(TIM4->CCMR1,TIM_CCMR1_OC1PE);
}
static void TIMER4_CH1_CCP_ENABLE_PRELOAD(void){
	SET_BIT(TIM4->CCMR1,TIM_CCMR1_OC1PE);
}
static void TIMER4_CH1_CCP_DISABLE(void){
	TIM4->CCMR1&=0XFF8F;
}
static void TIMER4_CH1_ENABLE_OUTPUT(void){
	SET_BIT(TIM4->CCER,TIM_CCER_CC1E);
}
static void TIMER4_CH1_SELECT_PWM_MODE(void){
    TIM4->CCMR1&=0xFF8F;
    TIM4->CCMR1|=0x0060;
}



//TIMER4 CHANNEL_2 MACROS
static void TIMER4_CH2_SELECT_OUTPUT_MODE(void){
	TIM4->CCMR1&=0XFCFF;
}
static void TIMER4_CH2_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM4->CCMR1,TIM_CCMR1_CC2S_1);
    SET_BIT(TIM4->CCMR1,TIM_CCMR1_CC2S_0);
}

static void TIMER4_COUNTER_CH2_RISING_EDGE(void){
	CLEAR_BIT(TIM4->CCER,TIM_CCER_CC1P);
}
static void TIMER4_CH2_CCP_DISABLE_PRELOAD(void){
	CLEAR_BIT(TIM4->CCMR1,TIM_CCMR1_OC2PE);
}
static void TIMER4_CH2_CCP_ENABLE_PRELOAD(void){
	SET_BIT(TIM4->CCMR1,TIM_CCMR1_OC2PE);
}
static void TIMER4_CH2_CCP_DISABLE(void){
	TIM4->CCMR1&=0X8FFF;
}
static void TIMER4_CH2_ENABLE_OUTPUT(void){
	SET_BIT(TIM4->CCER,TIM_CCER_CC2E);
}
static void TIMER4_CH2_SELECT_PWM_MODE(void){
    TIM4->CCMR1&=0x8FFF;
    TIM4->CCMR1|=0x6000;
}




//TIMER4 CHANNEL_3 MACROS
static void TIMER4_CH3_SELECT_OUTPUT_MODE(void){
		TIM4->CCMR2&=0XFFFC;
}
static void TIMER4_CH3_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM4->CCMR1,TIM_CCMR2_CC3S_1);
    SET_BIT(TIM4->CCMR1,TIM_CCMR2_CC3S_0);
}
static void TIMER4_COUNTER_CH3_RISING_EDGE(void){
	CLEAR_BIT(TIM4->CCER,TIM_CCER_CC3P);
}
static void TIMER4_CH3_CCP_DISABLE_PRELOAD(void){
	CLEAR_BIT(TIM4->CCMR2,TIM_CCMR2_OC3PE);
}
static void TIMER4_CH3_CCP_ENABLE_PRELOAD(void){
	SET_BIT(TIM4->CCMR2,TIM_CCMR2_OC3PE);
}
static void TIMER4_CH3_CCP_DISABLE(void){
	TIM4->CCMR2&=0XFF8F;
}
static void TIMER4_CH3_ENABLE_OUTPUT(void){
	SET_BIT(TIM4->CCER,TIM_CCER_CC3E);
}
static void TIMER4_CH3_SELECT_PWM_MODE(void){
    TIM4->CCMR2&=0xFF8F;
    TIM4->CCMR2|=0x0060;
}


//TIMER4 CHANNEL_4 MACROS
static void TIMER4_CH4_SELECT_OUTPUT_MODE(void){
	TIM4->CCMR2&=0XFCFF;
}
static void TIMER4_CH4_SELECT_INPUT_MODE(void){
    CLEAR_BIT(TIM4->CCMR2,TIM_CCMR2_CC4S_1);
    SET_BIT(TIM4->CCMR2,TIM_CCMR2_CC4S_0);
}
static void TIMER4_COUNTER_CH4_RISING_EDGE(void){
	CLEAR_BIT(TIM4->CCER,TIM_CCER_CC4P);
}
static void TIMER4_CH4_CCP_DISABLE_PRELOAD(void){
	CLEAR_BIT(TIM4->CCMR2,TIM_CCMR2_OC4PE);
}
static void TIMER4_CH4_CCP_ENABLE_PRELOAD(void){
	SET_BIT(TIM4->CCMR2,TIM_CCMR2_OC4PE);
}
static void TIMER4_CH4_CCP_DISABLE(void){
	TIM4->CCMR2&=0X8FFF;
}
static void TIMER4_CH4_ENABLE_OUTPUT(void){
	SET_BIT(TIM4->CCER,TIM_CCER_CC4E);
}
static void TIMER4_CH4_SELECT_PWM_MODE(void){
    TIM4->CCMR2&=0x8FFF;
    TIM4->CCMR2|=0x6000;
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

