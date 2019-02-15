/*
********************************************************************************
* COPYRIGHT(c) ЗАО «ЧИП и ДИП», 2018
* 
* Программное обеспечение предоставляется на условиях «как есть» (as is).
* При распространении указание автора обязательно.
********************************************************************************
*/


#include "ADC.h"


//------------------------------------------------------------------------------
void ADC_Init()
{
  ADC_GPIO->MODER |= (3 << (2 * ADC_PIN_1)) | (3 << (2 * ADC_PIN_2));
  //ADC_GPIO->PUPDR |= (2 << (2 * ADC_PIN_1)) | (3 << (2 * ADC_PIN_2));
  
  RCC->ADC_ENR |= ADC_CLK_EN;  
  ADC_USED->CR |= ADC_CR_ADCAL;
  while(((ADC_USED->CR) & ADC_CR_ADCAL) == ADC_CR_ADCAL);
  
  ADC_USED->CFGR2 = ADC_CFGR2_CKMODE_1;
  ADC_USED->SMPR = ADC_SMPR_SMP_1 | ADC_SMPR_SMP_0;
  ADC_USED->CHSELR = ADC_CHANNEL_1;
  ADC_USED->CR |= ADC_CR_ADEN;
  while(((ADC_USED->ISR) & ADC_ISR_ADRDY) != ADC_ISR_ADRDY);
}
//------------------------------------------------------------------------------
void ADC_GetData(uint16_t *DataBuff)
{      
  for (uint8_t chnl = 0; chnl < AUDIO_CHANNELS_STEREO; chnl++)
  {
    ADC_USED->CHSELR = ADCChanls[chnl];
    ADC_USED->CR |= ADC_CR_ADSTART;
    while((ADC_USED->ISR & ADC_ISR_EOC) != ADC_ISR_EOC);
    DataBuff[chnl] = ADC_USED->DR;
  }
}


