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
                TIM2_PSC=prescaler_value;
                TIM2_ARR=preload_value;
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
			while(!delay){}
			delay=0;
			timer2_timeBase_write(timer2_ptr,temp);
	}			
	return retval;

}
#endif

#if TIMER2_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer2_pwm_write(timer2_config_t *timer2_ptr,uint32_t duty_cycle){
			Std_ReturnType_t retval=E_OK;
			switch(timer2_ptr->channel){
				case TIMER2_CH1:
				TIM2_CCR1=duty_cycle;
				break;
				case TIMER2_CH2:
				TIM2_CCR2=duty_cycle;
				break;
				case TIMER2_CH3:
				TIM2_CCR3=duty_cycle;
				break;
				case TIMER2_CH4:
				TIM2_CCR4=duty_cycle;
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
                TIM2_PSC=prescaler_value;
                TIM2_ARR=preload_value;
            
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
                TIM2_PSC=prescaler_value;
                TIM2_ARR=preload_value;
								
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
								TIM2_CCR1=timer2_ptr->duty_cycle;
								break;
								
								case TIMER2_CH2 :
								/**************output_enable****/
								TIMER2_CH2_ENABLE_OUTPUT();
                /*****timer2 ch2 PWM select******/
                TIMER2_CH2_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER2_CH2_CCP_ENABLE_PRELOAD();
								TIM2_CCR2=timer2_ptr->duty_cycle;
								break;
								case TIMER2_CH3 :
								/**************output_enable****/
								TIMER2_CH3_ENABLE_OUTPUT();
                /*****timer2 ch1 PWM select******/
                TIMER2_CH3_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER2_CH3_CCP_ENABLE_PRELOAD();
								TIM2_CCR3=timer2_ptr->duty_cycle;
								break;
							
								case TIMER2_CH4 :
								/**************output_enable****/
								TIMER2_CH4_ENABLE_OUTPUT();
                /*****timer2 ch1 PWM select******/
                TIMER2_CH4_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER2_CH4_CCP_ENABLE_PRELOAD();
								TIM2_CCR4=timer2_ptr->duty_cycle;
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
        SET_BIT_K(NVIC_ISER0,TIMER2_NVIC);
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
        SET_BIT_K(NVIC_ICER0,TIMER2_NVIC);
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
	SET_BIT_K(NVIC_ICPR0,TIMER2_NVIC);	
	if(READ_BIT_K(TIM2_SR,TIMX_SR_UIF))
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

