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
static uint8_t delay;
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
                TIM4_PSC=prescaler_value;
                TIM4_ARR=preload_value;
								timer4_ptr->time_base_us=time_base;
	}			
	return retval;

	}

Std_ReturnType_t timer4_delay(timer4_config_t *timer2_ptr,uint32_t delay_time)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer2_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
			uint32_t temp=timer2_ptr->time_base_us;
			timer4_timeBase_write(timer2_ptr,delay_time);
			while(!delay){}
			delay=0;
			timer4_timeBase_write(timer2_ptr,temp);
	}			
	return retval;
}
#endif

#if TIMER4_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer4_pwm_write(timer4_config_t *timer4_ptr,uint32_t duty_cycle){
			Std_ReturnType_t retval=E_OK;
			switch(timer4_ptr->channel){
				case TIMER4_CH1:
				TIM4_CCR1=duty_cycle;
				break;
				case TIMER4_CH2:
				TIM4_CCR2=duty_cycle;
				break;
				case TIMER4_CH3:
				TIM4_CCR3=duty_cycle;
				break;
				case TIMER4_CH4:
				TIM4_CCR4=duty_cycle;
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
                TIM4_PSC=prescaler_value;
                TIM4_ARR=preload_value;
            
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
                TIM4_PSC=prescaler_value;
                TIM4_ARR=preload_value;
								
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
								TIM4_CCR1=timer4_ptr->duty_cycle;
								break;
								
								case TIMER4_CH2 :
								/**************output_enable****/
								TIMER4_CH2_ENABLE_OUTPUT();
                /*****timer4 ch2 PWM select******/
                TIMER4_CH2_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER4_CH2_CCP_ENABLE_PRELOAD();
								TIM4_CCR2=timer4_ptr->duty_cycle;
								break;
								case TIMER4_CH3 :
								/**************output_enable****/
								TIMER4_CH3_ENABLE_OUTPUT();
                /*****timer4 ch1 PWM select******/
                TIMER4_CH3_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER4_CH3_CCP_ENABLE_PRELOAD();
								TIM4_CCR3=timer4_ptr->duty_cycle;
								break;
							
								case TIMER4_CH4 :
								/**************output_enable****/
								TIMER4_CH4_ENABLE_OUTPUT();
                /*****timer4 ch1 PWM select******/
                TIMER4_CH4_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER4_CH4_CCP_ENABLE_PRELOAD();
								TIM4_CCR4=timer4_ptr->duty_cycle;
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
        SET_BIT_K(NVIC_ISER0,TIMER4_NVIC);
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
        SET_BIT_K(NVIC_ICER0,TIMER4_NVIC);
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

void	TIM4_IRQHandler(void){
	SET_BIT_K(NVIC_ICPR0,TIMER4_NVIC);	
	if(READ_BIT_K(TIM4_SR,TIMX_SR_UIF))
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


/******************************************************EDIT DETAILS SECTION END  **********************************************************************************/




/**/