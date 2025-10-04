/******************************************************INCLUDES SECTION START*******************************************************/


#include "MCAL_ADC.h"

/******************************************************INCLUDES SECTION END*******************************************************/



#define ADC_MAX_VALUE_12BIT 4095
#define ADC_MAX_VALUE_10BIT 1023
#define ADC_MAX_VALUE_8BIT  255
#define ADC_MAX_VALUE_6BIT  63

static ADC_TypeDef *get_adc_instance(ADC_Index_t adc_index) {
    switch (adc_index) {
        case ADC1_INDEX:
            return ADC1;
        case ADC2_INDEX:
            return ADC2;
        default:
            return NULL;
    }
}

static uint32_t get_adc_max_value(ADC_Resolution_t resolution) {
    switch (resolution) {
        case ADC_RESOLUTION_12BIT: return ADC_MAX_VALUE_12BIT;
        case ADC_RESOLUTION_10BIT: return ADC_MAX_VALUE_10BIT;
        case ADC_RESOLUTION_8BIT:  return ADC_MAX_VALUE_8BIT;
        case ADC_RESOLUTION_6BIT:  return ADC_MAX_VALUE_6BIT;
        default: return ADC_MAX_VALUE_12BIT;
    }
}

Std_ReturnType_t adc_init(const ADC_Config_t *config) {
    if (!config) {
        return E_NOT_OK;
    }

    ADC_TypeDef *adc = get_adc_instance(config->adc_index);
    if (!adc) {
        return E_NOT_OK;
    }

    // Enable ADC clock
    if (config->adc_index == ADC1_INDEX) {
        RCC->APB2ENR |= (1 << 9);  // Enable ADC1 clock
    } else if (config->adc_index == ADC2_INDEX) {
        RCC->APB2ENR |= (1 << 10);  // Enable ADC2 clock
    }

    // Set ADC resolution and alignment
    adc->CR1 &= ~(0x3 << 24);
    switch (config->resolution) {
        case ADC_RESOLUTION_6BIT:
            adc->CR1 |= (0x3 << 24);
            break;
        case ADC_RESOLUTION_8BIT:
            adc->CR1 |= (0x2 << 24);
            break;
        case ADC_RESOLUTION_10BIT:
            adc->CR1 |= (0x1 << 24);
            break;
        case ADC_RESOLUTION_12BIT:
            break;
        default:
            return E_NOT_OK;
    }

    // Configure sampling time for the channel
    if (config->channel <= ADC_CHANNEL_9) {
        adc->SMPR2 |= (config->sampling_time << (config->channel * 3));
    } else {
        adc->SMPR1 |= (config->sampling_time << ((config->channel - 10) * 3));
    }

    // Continuous conversion mode
    adc->CR2 |= (1 << 1);  // CONT

    // Enable ADC
    adc->CR2 |= (1 << 0);  // ADON

    return E_OK;
}

Std_ReturnType_t adc_start_conversion(ADC_Index_t adc_index, ADC_Channel_t channel) {
    if (channel > ADC_CHANNEL_15) {
        return E_NOT_OK;
    }

    ADC_TypeDef *adc = get_adc_instance(adc_index);
    if (!adc) {
        return E_NOT_OK;
    }

    // Select the channel
    adc->SQR3 = (channel & 0x1F);

    // Start the conversion
    adc->CR2 |= (1 << 30);  // SWSTART

    return E_OK;
}

Std_ReturnType_t adc_read_value(ADC_Index_t adc_index, ADC_Channel_t channel, uint16_t *value) {
    if (!value || channel > ADC_CHANNEL_15) {
        return E_NOT_OK;
    }

    ADC_TypeDef *adc = get_adc_instance(adc_index);
    if (!adc) {
        return E_NOT_OK;
    }

    // Wait for conversion to complete
    while (!(adc->SR & (1 << 1)));  // EOC

    // Read the value
    *value = adc->DR;

    return E_OK;
}

void adc_irq_handler(ADC_Index_t adc_index) {
    ADC_TypeDef *adc = get_adc_instance(adc_index);
    if (!adc) {
        return;
    }

    // Handle ADC interrupt
    if (adc->SR & (1 << 1)) {  // Check End of Conversion (EOC)
        uint16_t adc_value = adc->DR;  // Read ADC data register

        // Clear EOC flag
        adc->SR &= ~(1 << 1);

        // Process the ADC value or store it as needed
    }
}