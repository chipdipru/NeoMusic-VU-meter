/*
********************************************************************************
* COPYRIGHT(c) ЗАО «ЧИП и ДИП», 2018
* 
* Программное обеспечение предоставляется на условиях «как есть» (as is).
* При распространении указание автора обязательно.
********************************************************************************
*/


#ifndef __ADC_H
#define __ADC_H


#include "NeoMusic_board.h"



#define       ADC_CHANNELS_COUNT           AUDIO_CHANNELS


extern const uint16_t ADCChanls[];


void ADC_Init();

void ADC_GetData(uint16_t *DataBuff);


#endif //__ADC_H


