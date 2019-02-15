/*
********************************************************************************
* COPYRIGHT(c) ЗАО «ЧИП и ДИП», 2018
* 
* Программное обеспечение предоставляется на условиях «как есть» (as is).
* При распространении указание автора обязательно.
********************************************************************************
*/


#include "NeoMusic_board.h"
#include "NeoLEDOut.h"
#include "ADC.h"
#include  <math.h>

static volatile uint8_t State = 0;
static uint32_t ColorBuffer[LEDS_COUNT_MAX];
static AudioChnlType AudioChnls[AUDIO_CHANNELS_STEREO];
static FallingLEDEffectType Effects[AUDIO_CHANNELS_STEREO];
static uint8_t LEDsCount = 0;
static uint8_t LEDsMiddleZoneStart = 0;
static uint8_t AudioChnlsCount = 2;
static uint8_t LEDsPreChnl = 0;
static uint8_t dBMultiplyer = 1;


int main()
{
  Module_Init();
  
  for(;;)
  { 
    if ((Effects[CHNL_L].ActiveLEDs == AudioChnls[CHNL_L].Value) && (Effects[CHNL_R].ActiveLEDs == AudioChnls[CHNL_R].Value))
    {
      for (uint8_t chnl = 0; chnl < AudioChnlsCount; chnl++)
        AudioChnls[chnl].Value = 0;
      
      for (uint16_t sample = 0; sample < ADC_SAMPLES; sample++)
      {
        uint16_t ADCNewData[AUDIO_CHANNELS_STEREO];
        ADC_GetData(ADCNewData);
        
        for (uint8_t chnl = 0; chnl < AudioChnlsCount; chnl++)
        {
          if (ADCNewData[chnl] > AudioChnls[chnl].Noise)
          {
            ADCNewData[chnl] -= AudioChnls[chnl].Noise;
          
            if (ADCNewData[chnl] > AudioChnls[chnl].Value)
              AudioChnls[chnl].Value = ADCNewData[chnl];
          }
        }
      }
      
      for (uint8_t chnl = 0; chnl < AudioChnlsCount; chnl++)
      {
        AudioChnls[chnl].Filter.Data[SMA_SAMPLES] = AudioChnls[chnl].Value / SMA_SAMPLES;
        AudioChnls[chnl].Filter.Result = AudioChnls[chnl].Filter.Result - AudioChnls[chnl].Filter.Data[0] + AudioChnls[chnl].Filter.Data[SMA_SAMPLES];
        
        for(uint8_t i = 0; i < SMA_SAMPLES; i++)
          AudioChnls[chnl].Filter.Data[i] = AudioChnls[chnl].Filter.Data[i + 1];
        
        AudioChnls[chnl].Value = AudioChnls[chnl].Filter.Result;
        
        uint8_t VolLevel = 0;
        for (uint8_t i = 0; i < dB_VLAUES_COUNT; i++)
        {
          if (AudioChnls[chnl].Value < ADCTodBValues[dB_START_INDEX + i])
            break;
            
          VolLevel++;
        }
        
        if (LEDsPreChnl == LEDS_HALF_COUNT)
          VolLevel /= 2;
        else if (LEDsPreChnl != LEDS_BASE_COUNT)
          VolLevel *= dBMultiplyer;
        
        AudioChnls[chnl].Value = VolLevel;
        if (AudioChnls[chnl].Value > (LEDsPreChnl - 1))
          AudioChnls[chnl].Value = (LEDsPreChnl - 1);
               
        if (Effects[chnl].ActiveLEDs <= AudioChnls[chnl].Value)
          Effects[chnl].Direction = UP;
        else
          Effects[chnl].Direction = DOWN;
      }
    }
    
    for (uint8_t chnl = 0; chnl < AudioChnlsCount; chnl++)
      ChangeEffectFrame(&Effects[chnl], &AudioChnls[chnl]);
    
    if (AudioChnlsCount == AUDIO_CHANNELS_STEREO)
      RightChnlTopToBottom();

    LedsUpdate();    
  }
}
//------------------------------------------------------------------------------
void ChangeEffectFrame(FallingLEDEffectType *Effect, AudioChnlType *Chnl)
{
  if ((Effect->Direction == UP) && (Effect->ActiveLEDs < Chnl->Value))
    Effect->ActiveLEDs++;    
  else if ((Effect->Direction == DOWN) && (Effect->ActiveLEDs > Chnl->Value))
    Effect->ActiveLEDs--;
  
  if (Effect->PeakLEDIndex >= Effect->ActiveLEDs)
  {
    Effect->Counter++;
      
    if ((Effect->PeakPhase == PAUSE) && (Effect->Counter == PAUSE_VALUE))
    {
      Effect->PeakPhase = FALLING;        
      Effect->Counter = FALLING_VALUE - 1;
    }
      
    else if ((Effect->PeakPhase == FALLING) && (Effect->Counter == FALLING_VALUE))
    { 
      if (Effect->PeakLEDIndex > Effect->ActiveLEDs)
        Effect->PeakLEDIndex--;
      Effect->Counter = 0;
    }
  }
  else if (Effect->Direction == UP)
  {
    Effect->PeakLEDIndex = Effect->ActiveLEDs;
    if (Effect->PeakPhase == FALLING)
    {
      Effect->PeakPhase = PAUSE;
      Effect->Counter = 0;
    }
  }
  
  for (uint16_t led = 0; led < LEDsPreChnl; led++)
  {
    if (led < Effect->ActiveLEDs)
    {
      if (led == 0)
        ColorBuffer[led + Chnl->LEDsOffset] = START_LED_COLOR;
      else if (led == 1)
        ColorBuffer[led + Chnl->LEDsOffset] = START_SECOND_LED_COLOR;
      else if (led < LEDsMiddleZoneStart)
        ColorBuffer[led + Chnl->LEDsOffset] = LOW_ZONE_LED_COLOR; 
      else
        ColorBuffer[led + Chnl->LEDsOffset] = MIDDLE_ZONE_LED_COLOR; 
    }
    else
      ColorBuffer[led + Chnl->LEDsOffset] = BACKGROUND_LED_COLOR;
  }
    
  ColorBuffer[Effect->PeakLEDIndex + Chnl->LEDsOffset] = PEAK_LED_COLOR;
}
//------------------------------------------------------------------------------
void RightChnlTopToBottom()
{
  for (uint16_t led = 0; led < (LEDsPreChnl / 2); led++)
  {
    uint32_t SwapColor = ColorBuffer[led];
    ColorBuffer[led] = ColorBuffer[LEDsPreChnl - 1 - led];
    ColorBuffer[LEDsPreChnl - 1 - led] = SwapColor;
  }
}
//------------------------------------------------------------------------------
void Module_Init()
{
  //HSI, PLL, 48 MHz
  FLASH->ACR = FLASH_ACR_PRFTBE | (uint32_t)FLASH_ACR_LATENCY;  
  RCC->CFGR |= (uint32_t)(RCC_CFGR_PLLSRC_HSI_DIV2 | RCC_CFGR_PLLMUL12);
  RCC->CR |= RCC_CR_PLLON;
  while((RCC->CR & RCC_CR_PLLRDY) == 0);
  RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
  RCC->CFGR |= (uint32_t)RCC_CFGR_SW_PLL;
  while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) != (uint32_t)RCC_CFGR_SWS_PLL);
  
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOFEN;
  
  AUDIO_CHNL_COUNT_GPIO->PUPDR |= (1 << (2 * AUDIO_CHNL_COUNT_PIN));
  LEDsCount = GPIOF->IDR & LED_COUNT_MASK;
  RCC->AHBENR &= ~RCC_AHBENR_GPIOFEN;
  
  switch(LEDsCount)
  {
    case LEDS_HALF:
      LEDsCount = LEDS_HALF_COUNT;
    break;
    
    case LEDS_BASE:
      LEDsCount = LEDS_BASE_COUNT;
    break;
    
    case LEDS_BASEx2:
      LEDsCount = LEDS_BASEx2_COUNT;
    break;
    
    case LEDS_BASEx4:
      LEDsCount = LEDS_BASEx4_COUNT;
    break;
  }
  
  if ((AUDIO_CHNL_COUNT_GPIO->IDR & (1 << AUDIO_CHNL_COUNT_PIN)) != (1 << AUDIO_CHNL_COUNT_PIN))
    AudioChnlsCount = 1;
  
  AUDIO_CHNL_COUNT_GPIO->PUPDR &= ~(1 << (2 * AUDIO_CHNL_COUNT_PIN));
    
  LEDsPreChnl = LEDsCount / AudioChnlsCount;
  LEDsMiddleZoneStart = LEDsPreChnl - LEDsPreChnl / 3 - 1;
  
  if (LEDsPreChnl == LEDS_BASEx2_COUNT)
    dBMultiplyer = 2;
  else if (LEDsPreChnl == LEDS_BASEx4_COUNT)
    dBMultiplyer = 4;
  
  for (uint16_t led = 0; led < LEDS_COUNT_MAX; led++)
    ColorBuffer[led] = 0;
  
  for (uint8_t chnl = 0; chnl < AUDIO_CHANNELS_STEREO; chnl++)
  {
    AudioChnls[chnl].Noise = 12;
    AudioChnls[chnl].Value = 0;    
    AudioChnls[chnl].LEDsOffset = 0;
    Effects[chnl].ActiveLEDs = 0;
    Effects[chnl].PeakLEDIndex = 1;
    Effects[chnl].Direction = UP;
    Effects[chnl].PeakPhase = FALLING;
    Effects[chnl].Counter = FALLING_VALUE - 1;
    AudioChnls[chnl].Filter.Result = 0;    
  }
  
  ADC_Init();
  
  for (uint8_t i = 0; i < SMA_SAMPLES; i++)
  {
    uint16_t ADCNewData[AUDIO_CHANNELS_STEREO];
    ADC_GetData(ADCNewData);
    
    for (uint8_t chnl = 0; chnl < AudioChnlsCount; chnl++)
    {
      AudioChnls[chnl].Filter.Data[i] = ADCNewData[chnl] / SMA_SAMPLES;
      AudioChnls[chnl].Filter.Result += AudioChnls[chnl].Filter.Data[i];
    }
  }
  
  if (AudioChnlsCount == AUDIO_CHANNELS_STEREO)
    AudioChnls[CHNL_L].LEDsOffset = LEDsCount / 2;
    
  RCC->LEDS_UPDATE_TIMER_ENR |= LEDS_UPDATE_TIMER_CLK_EN;
  LEDS_UPDATE_TIMER->PSC = LEDS_UPDATE_TIMER_PSC;
  LEDS_UPDATE_TIMER->ARR = ((LEDS_UPDATE_TIMER_ARR + 1) / (LEDsPreChnl / 4)) - 1;
  LEDS_UPDATE_TIMER->DIER = TIM_DIER_UIE;
  NVIC_EnableIRQ(LEDS_UPDATE_TIMER_IRQ);
  NVIC_SetPriority(LEDS_UPDATE_TIMER_IRQ, LEDS_UPDATE_PRIORITY);
  
  NeoOutInit(ColorBuffer, LEDsCount, &LedsOutComplete);
  
  if (AudioChnlsCount != AUDIO_CHANNELS_STEREO)
  {
    PWM_GPIO->AFR[1] |= (PWM_2_TIM_AF << (4 * (PWM_2_PIN - 8))) | (PWM_3_TIM_AF << (4 * (PWM_3_PIN - 8)));
    PWM_GPIO->AFR[0] |= (PWM_1_TIM_AF << (4 * PWM_1_PIN));
    PWM_GPIO->MODER |= ((2 << (2 * PWM_1_PIN)) | (2 << (2 * PWM_2_PIN)) | (2 << (2 * PWM_3_PIN)));
    RCC->APB2ENR |= PWM_2_CLK_EN;
    RCC->APB1ENR |= PWM_1_CLK_EN;
  
    PWM_1_TIMER->PSC = PWM_PSC;
    PWM_1_TIMER->ARR = PWM_ARR;
    PWM_1_TIMER->CCR1 = 0;
    PWM_1_TIMER->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
    PWM_1_TIMER->CCER = TIM_CCER_CC1E;
  
    PWM_2_TIMER->PSC = PWM_PSC;
    PWM_2_TIMER->ARR = PWM_ARR;
    PWM_2_TIMER->CCR2 = 0;
    PWM_2_TIMER->CCR3 = 0;
    PWM_2_TIMER->CCMR1 = TIM_CCMR1_OC2M_2 | TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2PE;
    PWM_2_TIMER->CCMR2 = TIM_CCMR2_OC3M_2 | TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3PE;
    PWM_2_TIMER->BDTR = TIM_BDTR_MOE;
    PWM_2_TIMER->CCER = TIM_CCER_CC2E | TIM_CCER_CC3E;
  
    PWM_1_TIMER->EGR = TIM_EGR_UG;
    PWM_2_TIMER->EGR = TIM_EGR_UG;
    PWM_1_TIMER->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
    PWM_2_TIMER->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;
    
    PWM_1_TIMER->CCR1 = 0x8F; //B
    PWM_2_TIMER->CCR2 = 0x10; //G
    PWM_3_TIMER->CCR3 = 0;    //R
  }
  
  
  LEDS_UPDATE_TIMER->CR1 |= TIM_CR1_CEN;
}
//------------------------------------------------------------------------------
void LEDS_UPDATE_TIMER_ISR(void)
{
  LEDS_UPDATE_TIMER->SR &= ~TIM_SR_UIF;
  LEDS_UPDATE_TIMER->CR1 &= ~TIM_CR1_CEN;
  State |= STATE_LEDS_UPDATE;
}
//------------------------------------------------------------------------------
void LedsOutComplete()
{
  State &= ~STATE_LEDS_OUT_GOING;
}
//------------------------------------------------------------------------------
void LedsUpdate()
{
  while((State & STATE_LEDS_UPDATE) != STATE_LEDS_UPDATE);
  State |= STATE_LEDS_OUT_GOING;
  NeoStartDataOut();
  while(((State & STATE_LEDS_OUT_GOING) == STATE_LEDS_OUT_GOING));
  State &= ~STATE_LEDS_UPDATE;
  LEDS_UPDATE_TIMER->CR1 |= TIM_CR1_CEN;
}

