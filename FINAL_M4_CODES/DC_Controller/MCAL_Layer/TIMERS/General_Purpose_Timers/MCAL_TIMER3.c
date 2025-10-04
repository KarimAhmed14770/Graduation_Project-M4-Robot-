/**
@file: MCAL_TIMER3.c
@author: M4_TEAM
@creation_date: date
*/


/********************************INCLUDES SECTION START****************************************/
#include "MCAL_TIMER3.h"
/********************************INCLUDES SECTION END******************************************/



/********************************DATA TYPES DEFINITIONS SECTION START**************************/
static uint32_t prescaler_value=TIMER3_CLKIN_FREQ;
static uint32_t preload_value;
static uint8_t  timer3_resolution;//in us
static uint32_t timer3_max_count;//in us
#if TIMER3_MODE==TIMER_TIMER_MODE
static uint8_t delay;
#endif

#if TIMER3_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
static void(*TIM3_OF_InterruptHandler)(void)=NULL;
	#if TIMER3_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
	static void(*TIM3_CCP1_InterruptHandler)(void)=NULL;
	static void(*TIM3_CCP2_InterruptHandler)(void)=NULL;
	static void(*TIM3_CCP3_InterruptHandler)(void)=NULL;
	static void(*TIM3_CCP4_InterruptHandler)(void)=NULL;
	#endif
#endif

/********************************DATA TYPES DEFINITIONS SECTION END****************************/




/********************************HELPER FUNCTIONS PROTOTYPES SECTION START*********************/
#if TIMER3_MODE==TIMER_PWM_MODE
static Std_ReturnType_t pwm_pin_init(timer3_config_t *timer3_ptr);
#endif
static Std_ReturnType_t set_timer3_mode_preload_prescale(timer3_config_t *timer3_ptr);
static Std_ReturnType_t Enable_timer3_interrupt(void);
static Std_ReturnType_t Disable_timer3_interrupt(void);
static Std_ReturnType_t	timer3_handle_interrupts(timer3_config_t	*timer3_ptr);
/********************************HELPER FUNCTIONS PROTOTYPES SECTION END***********************/





/********************************FUNCTIONS DEFINITIONS(APIS) SECTION START*********************/
Std_ReturnType_t timer3_init(timer3_config_t *timer3_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer3_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
        /******************enable timer3 clock*********/
        RCC_TIM3_CLK_ENABLE();
        /******************DISABLE timer3**************/
        TIMER3_DISABLE();
        /******************DISABLE timer3 int**************/
        //retval=Disable_timer3_interrupt();
				/**************initialize gpio pin***********/
				#if TIMER3_MODE==TIMER_PWM_MODE
				retval=pwm_pin_init(timer3_ptr);
				#endif
        /************set mode************************/
        retval=set_timer3_mode_preload_prescale(timer3_ptr);
				/*************set callback functions**********/
				retval=timer3_handle_interrupts(timer3_ptr);
        /***********Enable Interrupts****************/
        retval=Enable_timer3_interrupt();
			
				//setting the UG bit to update the registers before counting
				TIMER3_GENERATE_UPDATE();
        /***********Enable timer3********************/
        TIMER3_ENABLE();
        
    }
    return retval;
}
Std_ReturnType_t timer3_deinit(timer3_config_t *timer3_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer3_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
				TIMER3_DISABLE();
		}
    return retval;
}


#if TIMER3_MODE==TIMER_TIMER_MODE

Std_ReturnType_t timer3_timeBase_write(timer3_config_t *timer3_ptr,uint32_t time_base){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer3_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
								timer3_resolution=1;
                timer3_max_count=TIMER3_COUNT_REG_MAX*timer3_resolution;
                while(timer3_max_count<time_base)
                {
                    timer3_resolution++;
                    prescaler_value=TIMER3_CLKIN_FREQ*timer3_resolution;
                    timer3_max_count=TIMER3_COUNT_REG_MAX*timer3_resolution;
                }
                preload_value=time_base/timer3_resolution;
                TIM3_PSC=prescaler_value;
                TIM3_ARR=preload_value;
								timer3_ptr->time_base_us=time_base;
	}			
	return retval;

	}

Std_ReturnType_t timer3_delay(timer3_config_t *timer3_ptr,uint32_t delay_time)
{
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer3_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
			uint32_t temp=timer3_ptr->time_base_us;
			timer3_timeBase_write(timer3_ptr,delay_time);
			while(!delay){}
			delay=0;
			timer3_timeBase_write(timer3_ptr,temp);
	}			
	return retval;
}
#endif

#if TIMER3_MODE==TIMER_PWM_MODE
Std_ReturnType_t timer3_pwm_write(timer3_config_t *timer3_ptr,uint32_t duty_cycle){
			Std_ReturnType_t retval=E_OK;
			switch(timer3_ptr->channel){
				case TIMER3_CH1:
				TIM3_CCR1=duty_cycle;
				break;
				case TIMER3_CH2:
				TIM3_CCR2=duty_cycle;
				break;
				case TIMER3_CH3:
				TIM3_CCR3=duty_cycle;
				break;
				case TIMER3_CH4:
				TIM3_CCR4=duty_cycle;
				break;
				default:retval=E_NOT_OK;
			}
			return retval;
}
#endif
/********************************FUNCTIONS DEFINITIONS (APIS) SECTION END**********************/




/********************************HELPER FUNCTIONS DEFINITIONS SECTION START********************/

#if TIMER3_MODE==TIMER_PWM_MODE
static Std_ReturnType_t pwm_pin_init(timer3_config_t *timer3_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer3_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		Pin_Config_t PWM_pin;
		
		PWM_pin.mode=OUTPUT_MODE_50;
		PWM_pin.conf=AFO_PP;
		switch(timer3_ptr->channel)
		{
			case TIMER3_CH1:
			PWM_pin.port_index=TIMER3_CH1_2_PORT;
			PWM_pin.pin_index=TIMER3_CH1_PIN;
			break;
			case TIMER3_CH2:
			PWM_pin.port_index=TIMER3_CH1_2_PORT;
			PWM_pin.pin_index=TIMER3_CH2_PIN;
			break;
			case TIMER3_CH3:
			PWM_pin.port_index=TIMER3_CH3_4_PORT;
			PWM_pin.pin_index=TIMER3_CH3_PIN;
			break;
			case TIMER3_CH4:
			PWM_pin.port_index=TIMER3_CH3_4_PORT;
			PWM_pin.pin_index=TIMER3_CH4_PIN;
			break;
			default:retval=E_NOT_OK;
		}
		retval=gpio_pin_init(&PWM_pin);
	}
	return retval;
}
#endif
static Std_ReturnType_t set_timer3_mode_preload_prescale(timer3_config_t *timer3_ptr){
    Std_ReturnType_t retval=E_OK;
    if(NULL==timer3_ptr)
    {
        retval=E_NOT_OK;
    }
    else
    {
       #if TIMER3_MODE==TIMER_TIMER_MODE
								timer3_resolution=1;
                timer3_max_count=TIMER3_COUNT_REG_MAX*timer3_resolution;
                while(timer3_max_count<timer3_ptr->time_base_us)
                {
                    timer3_resolution++;
                    prescaler_value=TIMER3_CLKIN_FREQ*timer3_resolution;
                    timer3_max_count=TIMER3_COUNT_REG_MAX*timer3_resolution;
                }
                preload_value=timer3_ptr->time_base_us/timer3_resolution;
                TIM3_PSC=prescaler_value;
                TIM3_ARR=preload_value;
            
        #endif

        #if TIMER3_MODE==TIMER_PWM_MODE
                timer3_resolution=1;
                timer3_max_count=TIMER3_COUNT_REG_MAX*timer3_resolution;
                while(timer3_max_count<timer3_ptr->PWM_frequency)
                {
                    timer3_resolution++;
                    prescaler_value=TIMER3_CLKIN_FREQ*timer3_resolution;
                    timer3_max_count=TIMER3_COUNT_REG_MAX*timer3_resolution;
                }
                preload_value=timer3_ptr->PWM_frequency/timer3_resolution;
                TIM3_PSC=prescaler_value;
                TIM3_ARR=preload_value;
								
								/*************888enable autoreload register8******/
              TIMER3_AUTO_RELOAD_ENABLE();
							switch(timer3_ptr->channel)
							{
								case TIMER3_CH1 :
								/**************output_enable****/
								TIMER3_CH1_ENABLE_OUTPUT();
                /*****timer3 ch1 PWM select******/
                TIMER3_CH1_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER3_CH1_CCP_ENABLE_PRELOAD();
								TIM3_CCR1=timer3_ptr->duty_cycle;
								break;
								
								case TIMER3_CH2 :
								/**************output_enable****/
								TIMER3_CH2_ENABLE_OUTPUT();
                /*****timer3 ch2 PWM select******/
                TIMER3_CH2_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER3_CH2_CCP_ENABLE_PRELOAD();
								TIM3_CCR2=timer3_ptr->duty_cycle;
								break;
								case TIMER3_CH3 :
								/**************output_enable****/
								TIMER3_CH3_ENABLE_OUTPUT();
                /*****timer3 ch1 PWM select******/
                TIMER3_CH3_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER3_CH3_CCP_ENABLE_PRELOAD();
								TIM3_CCR3=timer3_ptr->duty_cycle;
								break;
							
								case TIMER3_CH4 :
								/**************output_enable****/
								TIMER3_CH4_ENABLE_OUTPUT();
                /*****timer3 ch1 PWM select******/
                TIMER3_CH4_SELECT_PWM_MODE();	
								/*************enable preload******/
								TIMER3_CH4_CCP_ENABLE_PRELOAD();
								TIM3_CCR4=timer3_ptr->duty_cycle;
								break;
								default:retval=E_NOT_OK;
							}
							
							   return retval;

#endif
}
}

            
    


static Std_ReturnType_t Enable_timer3_interrupt(void){
    Std_ReturnType_t retval=E_OK;
    #if TIMER3_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
						#if TIMER3_MODE==TIMER_TIMER_MODE
            TIMER3_UPDATE_INTERRUPT_ENABLE();
						#endif
        #if TIMER3_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER3_CC1_INTERRUPT_ENABLE();
            TIMER3_CC2_INTERRUPT_ENABLE();
            TIMER3_CC3_INTERRUPT_ENABLE();
            TIMER3_CC4_INTERRUPT_ENABLE();
        #endif
        SET_BIT_K(NVIC_ISER0,TIMER3_NVIC);
    #endif

    return retval;
}

static Std_ReturnType_t Disable_timer3_interrupt(void){
	
    Std_ReturnType_t retval=E_OK;
    #if TIMER3_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
        #if TIMER3_MODE==TIMER_TIMER_MODE
            TIMER3_UPDATE_INTERRUPT_DISABLE();
        #endif
        #if TIMER3_CCP_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
            TIMER3_CC1_INTERRUPT_DISABLE();
            TIMER3_CC2_INTERRUPT_DISABLE();
            TIMER3_CC3_INTERRUPT_DISABLE();
            TIMER3_CC4_INTERRUPT_DISABLE();
        #endif
        SET_BIT_K(NVIC_ICER0,TIMER3_NVIC);
    #endif

    return retval;
}

static Std_ReturnType_t	timer3_handle_interrupts(timer3_config_t	*timer3_ptr){
	Std_ReturnType_t retval=E_OK;
	if(NULL==timer3_ptr)
	{
		retval=E_NOT_OK;
	}
	else
	{
		#if TIMER3_UPDATE_INTERRUPT_FEATURE==TIMER_FEATURE_ENABLE
						#if TIMER3_MODE==TIMER_TIMER_MODE
						TIM3_OF_InterruptHandler=timer3_ptr->TIM3_OF_InterruptHandler;
		#endif
				#if TIMER3_CCP_INTERRUPT_FEATURE==TIMER_PWM_MODE
						TIM3_CCP1_InterruptHandler=timer3_ptr->TIM3_CCP1_InterruptHandler;
						TIM3_CCP2_InterruptHandler=timer3_ptr->TIM3_CCP2_InterruptHandler;
						TIM3_CCP3_InterruptHandler=timer3_ptr->TIM3_CCP3_InterruptHandler;
						TIM3_CCP4_InterruptHandler=timer3_ptr->TIM3_CCP4_InterruptHandler;
				#endif
		#endif
	}
	return retval;
}

void	TIM3_IRQHandler(void){
	SET_BIT_K(NVIC_ICPR0,TIMER3_NVIC);	
	if(READ_BIT_K(TIM3_SR,TIMX_SR_UIF))
		{
			TIMER3_CLEAR_FLAG();
			#if TIMER3_MODE==TIMER_TIMER_MODE
			delay=1;
			#endif
			if(TIM3_OF_InterruptHandler){
					TIM3_OF_InterruptHandler();
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