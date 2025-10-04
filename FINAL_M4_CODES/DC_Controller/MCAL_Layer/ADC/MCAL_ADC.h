#ifndef MCAL_ADC_H
#define MCAL_ADC_H


#include "../GPIO/MCAL_GPIO.h"
//#include "stm32f10x.h" 
//#include "../RCC_CLOCK/MCAL_RCC.h"



typedef enum {
    ADC1_INDEX = 0,
    ADC2_INDEX
} ADC_Index_t;

typedef enum {
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
    ADC_CHANNEL_8,
    ADC_CHANNEL_9,
    ADC_CHANNEL_10,
    ADC_CHANNEL_11,
    ADC_CHANNEL_12,
    ADC_CHANNEL_13,
    ADC_CHANNEL_14,
    ADC_CHANNEL_15
} ADC_Channel_t;

typedef enum {
    ADC_RESOLUTION_6BIT,
    ADC_RESOLUTION_8BIT,
    ADC_RESOLUTION_10BIT,
    ADC_RESOLUTION_12BIT
} ADC_Resolution_t;

typedef struct {
    ADC_Index_t adc_index;
    ADC_Channel_t channel;
    ADC_Resolution_t resolution;
    uint32_t sampling_time;
} ADC_Config_t;



Std_ReturnType_t adc_init(const ADC_Config_t *config);
Std_ReturnType_t adc_start_conversion(ADC_Index_t adc_index, ADC_Channel_t channel);
Std_ReturnType_t adc_read_value(ADC_Index_t adc_index, ADC_Channel_t channel, uint16_t *value);
void adc_irq_handler(ADC_Index_t adc_index);




#endif