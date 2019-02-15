/*
********************************************************************************
* COPYRIGHT(c) ЗАО «ЧИП и ДИП», 2018
* 
* Программное обеспечение предоставляется на условиях «как есть» (as is).
* При распространении указание автора обязательно.
********************************************************************************
*/




#ifndef __NEO_STM_BOARD_H
#define __NEO_STM_BOARD_H

#include "stm32f030x6.h"



#define       LED_COUNT_PIN_0              0
#define       LED_COUNT_PIN_1              1
#define       LED_COUNT_GPIO               GPIOF
#define       LED_COUNT_MASK               ((1 << LED_COUNT_PIN_0) | (1 << LED_COUNT_PIN_1))

#define       DOUT_PIN                     7
#define       DOUT_GPIO                    GPIOA

//АЦП
#define       ADC_GPIO                     GPIOA
#define       ADC_PIN_1                    0
#define       ADC_PIN_2                    1
#define       ADC_USED                     ADC1
#define       ADC_ENR                      APB2ENR
#define       ADC_CLK_EN                   RCC_APB2ENR_ADCEN
#define       ADC_IRQ                      ADC1_IRQn
#define       ADC_CHANNEL_1                ADC_CHSELR_CHSEL0
#define       ADC_CHANNEL_2                ADC_CHSELR_CHSEL1

#define       AUDIO_CHNL_COUNT_PIN         2
#define       AUDIO_CHNL_COUNT_GPIO        ADC_GPIO

//таймеры для ШИМ
#define       PWM_GPIO                     GPIOA
#define       PWM_1_PIN                    4
#define       PWM_2_PIN                    9
#define       PWM_3_PIN                    10
#define       PWM_1_TIM_AF                 4
#define       PWM_2_TIM_AF                 2
#define       PWM_3_TIM_AF                 2
#define       PWM_1_TIMER                  TIM14
#define       PWM_2_TIMER                  TIM1
#define       PWM_3_TIMER                  TIM1
#define       PWM_1_CLK_EN                 RCC_APB1ENR_TIM14EN
#define       PWM_2_CLK_EN                 RCC_APB2ENR_TIM1EN
#define       PWM_3_CLK_EN                 RCC_APB2ENR_TIM1EN

#define       MAX_BRIGHTNESS               255
#define       PWM_PSC                      187
#define       PWM_ARR                      (MAX_BRIGHTNESS - 1)

//таймер
#define       LEDS_UPDATE_TIMER            TIM16
#define       LEDS_UPDATE_TIMER_ENR        APB2ENR
#define       LEDS_UPDATE_TIMER_CLK_EN     RCC_APB2ENR_TIM16EN
#define       LEDS_UPDATE_TIMER_PSC        799//1199//2399//
#define       LEDS_UPDATE_TIMER_ARR        3199//1599//799//
#define       LEDS_UPDATE_TIMER_IRQ        TIM16_IRQn

#define       LEDS_HALF                    0
#define       LEDS_BASE                    (1 << LED_COUNT_PIN_0)
#define       LEDS_BASEx2                  (1 << LED_COUNT_PIN_1)
#define       LEDS_BASEx4                  LED_COUNT_MASK

#define       dB_START_INDEX               0

#define       LEDS_BASE_COUNT              16//24
#define       LEDS_HALF_COUNT              (LEDS_BASE_COUNT / 2)
#define       LEDS_BASEx2_COUNT            (LEDS_BASE_COUNT * 2)
#define       LEDS_BASEx4_COUNT            (LEDS_BASE_COUNT * 4)

#define       ADC_SAMPLES                  128
#define       AUDIO_CHANNELS_STEREO        2
#define       NO_SOUND_ADC_VALUE           1600

#define       LEDS_COUNT_MAX               LEDS_BASEx4_COUNT//900            

#define       ACTIVE_COLOR_R               0
#define       ACTIVE_COLOR_G               255
#define       ACTIVE_COLOR_B               0

//шкала зеленый-желтый-красный
#define       START_LED_COLOR              ((uint32_t)(0x10008F))
#define       START_SECOND_LED_COLOR       ((uint32_t)(0x3F001F))
#define       LOW_ZONE_LED_COLOR           ((uint32_t)((ACTIVE_COLOR_G << 16) | (ACTIVE_COLOR_R << 8) | ACTIVE_COLOR_B))
#define       PEAK_LED_COLOR               ((uint32_t)(0x00FF00))
#define       MIDDLE_ZONE_LED_COLOR        ((uint32_t)(0x8FFF00))
#define       BACKGROUND_LED_COLOR         ((uint32_t)(0x020000))//((uint32_t)(0x030303))


/*
//шкала лед
#define       START_LED_COLOR              ((uint32_t)(0xEDCCFF))
#define       START_SECOND_LED_COLOR       ((uint32_t)(0xDB87FF))
#define       LOW_ZONE_LED_COLOR           ((uint32_t)(0xA860FF))
#define       PEAK_LED_COLOR               ((uint32_t)(0x0000FF))
#define       MIDDLE_ZONE_LED_COLOR        ((uint32_t)(0x6A26FF))
#define       BACKGROUND_LED_COLOR         ((uint32_t)(0x030303))
*/
/*
//шкала пламя
#define       START_LED_COLOR              ((uint32_t)(0xF7FF93))
#define       START_SECOND_LED_COLOR       ((uint32_t)(0xCFFF7C))
#define       LOW_ZONE_LED_COLOR           ((uint32_t)(0x9EFF54))
#define       PEAK_LED_COLOR               ((uint32_t)(0x00FF00))
#define       MIDDLE_ZONE_LED_COLOR        ((uint32_t)(0x59FF30))
#define       BACKGROUND_LED_COLOR         ((uint32_t)(0x030303))
*/

#define       STATE_LEDS_OUT_GOING         (1 << 0)
#define       STATE_LEDS_UPDATE            (1 << 1)

#define       CHNL_L                       0
#define       CHNL_R                       1

#define       LEDS_UPDATE_PRIORITY         1


#define       dBM44_VOLTAGE                5 //0,006 V
#define       dBM42_VOLTAGE                7 //0,006 V
#define       dBM40_VOLTAGE                9 //0,008 V
#define       dBM38_VOLTAGE                12 //0,009 V
#define       dBM36_VOLTAGE                15 //0,012 V
#define       dBM34_VOLTAGE                20 //0,016 V
#define       dBM32_VOLTAGE                24 //0,019 V
#define       dBM30_VOLTAGE                30 //0,024 V
#define       dBM28_VOLTAGE                45 //0,036 V
#define       dBM26_VOLTAGE                48 //0,039 V
#define       dBM24_VOLTAGE                60 //0,049 V
#define       dBM22_VOLTAGE                76 //0,062 V
#define       dBM20_VOLTAGE                96 //0,078 V
#define       dBM18_VOLTAGE                121 //0,098 V
#define       dBM16_VOLTAGE                152 //0,123 V
#define       dBM14_VOLTAGE                192 //0,155 V
#define       dBM12_VOLTAGE                242 //0,195 V
#define       dBM10_VOLTAGE                304 //0,245 V
#define       dBM08_VOLTAGE                383 //0,309 V
#define       dBM06_VOLTAGE                482 //0,388 V
#define       dBM04_VOLTAGE                607 //0,489 V
#define       dBM02_VOLTAGE                764 //0,616 V
#define       dB_00_VOLTAGE                962 //0,775 V
#define       dB_01_VOLTAGE                1079 //0,870 V
#define       dB_02_VOLTAGE                1211 //0,976 V
#define       dB_03_VOLTAGE                1358 //1,095 V
#define       dB_04_VOLTAGE                1524 //1,228 V
#define       dB_05_VOLTAGE                1710 //1,378 V
#define       dB_06_VOLTAGE                1918 //1,546 V
#define       dB_07_VOLTAGE                2153 //1,735 V
#define       dB_08_VOLTAGE                2416 //1,947 V
#define       dB_09_VOLTAGE                2710 //2,184 V
#define       dB_10_VOLTAGE                3041 //2,451 V
#define       dB_11_VOLTAGE                3412 //2,750 V
#define       dB_12_VOLTAGE                3828 //3,085 V

#define       dB_VLAUES_COUNT              LEDS_BASE_COUNT

#define       SMA_SAMPLES                  16 


extern const uint16_t ADCTodBValues[];

typedef struct
{
  uint16_t Data[SMA_SAMPLES + 1];
  uint16_t Result;
    
} SMAType;

typedef struct
{
  uint16_t Noise;
  uint16_t Value;
  uint16_t LEDsOffset;
  SMAType  Filter;
  
} AudioChnlType;

typedef struct
{
  uint16_t ActiveLEDs;
  uint16_t PeakLEDIndex;
  uint8_t Counter;
  uint8_t PeakPhase;
  uint8_t Direction;
  
} FallingLEDEffectType;
  
enum PeakPhase
{
  PAUSE,
  FALLING,
};

enum PhaseCntVals
{
  FALLING_VALUE = 6,
  PAUSE_VALUE = 20,
};

enum EffectDir
{
  UP,
  DOWN,
};


void ChangeEffectFrame(FallingLEDEffectType *Effect, AudioChnlType *Chnl);

void RightChnlTopToBottom();

void Module_Init();

void LedsOutComplete();

void LedsUpdate();

void LEDS_UPDATE_TIMER_ISR(void);


#endif //__NEO_STM_BOARD_H


