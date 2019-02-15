/*
********************************************************************************
* COPYRIGHT(c) ЗАО «ЧИП и ДИП», 2018
* 
* Программное обеспечение предоставляется на условиях «как есть» (as is).
* При распространении указание автора обязательно.
********************************************************************************
*/



#include "ADC.h"



const uint16_t ADCChanls[] = {ADC_CHANNEL_1, ADC_CHANNEL_2, };

const uint16_t ADCTodBValues[] = 
{
  dBM44_VOLTAGE, dBM42_VOLTAGE, dBM40_VOLTAGE, dBM38_VOLTAGE,
  dBM36_VOLTAGE, dBM34_VOLTAGE, dBM32_VOLTAGE, dBM30_VOLTAGE,
  dBM28_VOLTAGE, dBM26_VOLTAGE, dBM24_VOLTAGE, dBM22_VOLTAGE,
  dBM20_VOLTAGE, dBM18_VOLTAGE, dBM16_VOLTAGE, dBM14_VOLTAGE,
  dBM12_VOLTAGE, dBM10_VOLTAGE, dBM08_VOLTAGE, dBM06_VOLTAGE,
  dBM04_VOLTAGE, dBM02_VOLTAGE, dB_00_VOLTAGE, dB_01_VOLTAGE,
  dB_02_VOLTAGE, dB_03_VOLTAGE, dB_04_VOLTAGE, dB_05_VOLTAGE,
  dB_06_VOLTAGE, dB_07_VOLTAGE, dB_08_VOLTAGE, dB_09_VOLTAGE,
  dB_10_VOLTAGE, dB_11_VOLTAGE, dB_12_VOLTAGE, 
};









