/*
********************************************************************************
* COPYRIGHT(c) ЗАО «ЧИП и ДИП», 2018
* 
* Программное обеспечение предоставляется на условиях «как есть» (as is).
* При распространении указание автора обязательно.
********************************************************************************
*/


#include "NeoLEDOut.h"


static uint32_t *NeoBuffer;
static void (*OutComplete)(void);
static uint8_t DMABuffer[DMA_BUFFER_SIZE];
static uint16_t CurLEDIndex = 0;
static uint8_t LEDsCountUsed = 0;


void NeoOutInit(uint32_t *DataBuffer, uint8_t LEDsCount, void (*OutCompleteCallback)(void))
{   
  NeoBuffer = DataBuffer;
  LEDsCountUsed = LEDsCount;
  OutComplete = OutCompleteCallback;
  RCC->AHBENR |= RCC_AHBENR_DMAEN;
  DOUT_DMA_CHANNEL->CPAR = (uint32_t) (&(DOUT_TIMER->CCR1));
  DOUT_DMA_CHANNEL->CMAR = (uint32_t)(&DMABuffer[0]);
  DOUT_DMA_CHANNEL->CCR = DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_DIR | DMA_CCR_PSIZE_0 | DMA_CCR_HTIE | DMA_CCR_TCIE | DMA_CCR_PL;
  NVIC_EnableIRQ(DOUT_DMA_IRQ);
  NVIC_SetPriority(DOUT_DMA_IRQ, 0);
  
  DOUT_GPIO->MODER |=  (2 << (2 * DOUT_PIN));
  DOUT_GPIO->AFR[0] |= (DOUT_TIM_AF << (4 * DOUT_PIN));
  DOUT_GPIO->OSPEEDR |= (3 << (2 * DOUT_PIN));
  DOUT_GPIO->PUPDR |= (1 << (2 * DOUT_PIN));
    
  RCC->DOUT_TIMER_ENR |= DOUT_TIMER_CLK_EN;
  DOUT_TIMER->PSC = DOUT_TIMER_PSC;
  DOUT_TIMER->ARR = DOUT_TIMER_ARR;  
  DOUT_TIMER->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE;
  DOUT_TIMER->BDTR = TIM_BDTR_MOE;
  DOUT_TIMER->CCER = TIM_CCER_CC1E;
  DOUT_TIMER->DIER = TIM_DIER_CC1DE;
  DOUT_TIMER->SR = 0;
}
//------------------------------------------------------------------------------
void NeoStartDataOut()
{
  CurLEDIndex = 0;
  PrepareData(0);
  CurLEDIndex += DMA_BUFFER_HALF_LEDS;
  PrepareData(DMA_BUFFER_HALF_SIZE);
  
  DOUT_DMA_CHANNEL->CNDTR = DMA_BUFFER_SIZE;
  DOUT_TIMER->CCR1 = 0;
  DOUT_DMA_CHANNEL->CCR |= DMA_CCR_EN;
  DOUT_TIMER->CR1 |= TIM_CR1_CEN;
}
//------------------------------------------------------------------------------
void DMA1_Channel1_IRQHandler(void)
{
  if ((DMA1->ISR & DMA_ISR_HTIF1) == DMA_ISR_HTIF1)
  {
    if (CurLEDIndex < (LEDsCountUsed - DMA_BUFFER_HALF_LEDS))
    {
      CurLEDIndex += DMA_BUFFER_HALF_LEDS;
      PrepareData(0);
    }
    else
    {
      CurLEDIndex += DMA_BUFFER_HALF_LEDS;
      PrepareReset(0);
    }
  }
  else if ((DMA1->ISR & DMA_ISR_TCIF1) == DMA_ISR_TCIF1)
  {
    if (CurLEDIndex < LEDsCountUsed)
    {
      CurLEDIndex += DMA_BUFFER_HALF_LEDS;
      PrepareData(DMA_BUFFER_HALF_SIZE);
    }
    else if (CurLEDIndex == LEDsCountUsed)
    {
      CurLEDIndex += DMA_BUFFER_HALF_LEDS;
      PrepareReset(DMA_BUFFER_HALF_SIZE);
    }
    else
    {
      DOUT_TIMER->CR1 &= ~TIM_CR1_CEN;
      DOUT_TIMER->SR = 0;
      DOUT_DMA_CHANNEL->CCR &= ~DMA_CCR_EN;
  
      OutComplete();
    }
  }
  
  DMA1->IFCR |= DMA_IFCR_CGIF1;
}
//------------------------------------------------------------------------------
void PrepareData(uint8_t DMABufStart)
{
  for (uint16_t led = 0; led < DMA_BUFFER_HALF_LEDS; led++)
  {
    for (uint8_t LedBit = 0; LedBit < LED_DATA_SIZE; LedBit++)
    {
      uint16_t DMABitIndex = (DMABufStart + led + 1) * LED_DATA_SIZE - 1 - LedBit;
      
      if ((NeoBuffer[CurLEDIndex + led] & (1 << LedBit)) == (1 << LedBit))
        DMABuffer[DMABitIndex] = LED_T1H_OUT;
      else
        DMABuffer[DMABitIndex] = LED_T0H_OUT;
    }
  }
}
//------------------------------------------------------------------------------
void PrepareReset(uint8_t DMABufStart)
{
  for (uint16_t led = 0; led < DMA_BUFFER_HALF_LEDS; led++)
  {
    for (uint8_t LedBit = 0; LedBit < LED_DATA_SIZE; LedBit++)
    {
      uint16_t DMABitIndex = (DMABufStart + led) * LED_DATA_SIZE + LedBit;
      
      DMABuffer[DMABitIndex] = 0;
    }
  }
}


