/*
********************************************************************************
* COPYRIGHT(c) ЗАО «ЧИП и ДИП», 2018
* 
* Программное обеспечение предоставляется на условиях «как есть» (as is).
* При распространении указание автора обязательно.
********************************************************************************
*/




#ifndef __NEO_LED_OUT_H
#define __NEO_LED_OUT_H


#include "NeoMusic_board.h"


//таймер для ШИМ DOUT
#define       DOUT_TIM_AF                  5
#define       DOUT_TIMER                   TIM17
#define       DOUT_TIMER_ENR               APB2ENR
#define       DOUT_TIMER_CLK_EN            RCC_APB2ENR_TIM17EN
#define       DOUT_TIMER_IRQ               TIM17_IRQn
#define       DOUT_TIMER_PSC               (5 - 1) //частота 9,6 МГц
#define       DOUT_TIMER_ARR               (12 - 1) //частота 800 кГц
#define       DOUT_DMA_CHANNEL             DMA1_Channel1
#define       DOUT_DMA_IRQ                 DMA1_Channel1_IRQn

#define       LED_T1H_OUT                  10
#define       LED_T0H_OUT                  2

#define       LED_DATA_SIZE                24
#define       DMA_BUFFER_LEDS              4
#define       DMA_BUFFER_HALF_LEDS         (DMA_BUFFER_LEDS / 2)

#define       DMA_BUFFER_SIZE              (DMA_BUFFER_LEDS * LED_DATA_SIZE)
#define       DMA_BUFFER_HALF_SIZE         (DMA_BUFFER_LEDS / 2)
#define       EXTRA_OUT_BITS               DMA_BUFFER_SIZE//96//72//120




void DMA1_Channel1_IRQHandler(void);

void NeoOutInit(uint32_t *DataBuffer, uint8_t LEDsCount, void (*OutCompleteCallback)(void));

void NeoStartDataOut();

void PrepareData(uint8_t DMABufStart);

void PrepareReset(uint8_t DMABufStart);


#endif //__NEO_LED_OUT_H




