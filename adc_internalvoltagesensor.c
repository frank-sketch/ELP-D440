/***********************************************************************************************************************
    @file    adc_internalvoltagesensor.c
    @author  FAE Team
    @date    25-May-2023
    @brief   THIS FILE PROVIDES ALL THE SYSTEM FUNCTIONS.
  **********************************************************************************************************************
    @attention

    <h2><center>&copy; Copyright(c) <2023> <MindMotion></center></h2>

      Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
    following conditions are met:
    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
       the following disclaimer in the documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or
       promote products derived from this software without specific prior written permission.

      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
    INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
    SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
    WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *********************************************************************************************************************/

/* Define to prevent recursive inclusion */
#define _ADC_INTERNALVOLTAGESENSOR_C_

/* Files include */
#include <stdio.h>
#include "platform.h"
#include "adc_internalvoltagesensor.h"

/**
  * @addtogroup MM32F0140_LibSamples
  * @{
  */

/**
  * @addtogroup ADC
  * @{
  */

/**
  * @addtogroup ADC_InternalVoltageSensor
  * @{
  */

/* Private typedef ****************************************************************************************************/

/* Private define *****************************************************************************************************/

/* Private macro ******************************************************************************************************/

/* Private variables **************************************************************************************************/

/* Private functions **************************************************************************************************/

/***********************************************************************************************************************
  * @brief
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
uint32_t ADC_Timer100msec = 0;

void ADC_Timer(void)
{

	if( ADC_Timer100msec ) ADC_Timer100msec--;
}



void ADC_Configure(void)
{
     ADC_InitTypeDef  ADC_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2ENR_ADC1, ENABLE);

    ADC_StructInit(&ADC_InitStruct);
    ADC_InitStruct.ADC_Resolution         = ADC_Resolution_12b;
    ADC_InitStruct.ADC_PRESCARE           = ADC_PCLK2_PRESCARE_16;
    ADC_InitStruct.ADC_Mode               = ADC_Mode_Imm;
    ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStruct.ADC_ExternalTrigConv   = ADC1_ExternalTrigConv_T1_CC1;
    ADC_InitStruct.ADC_DataAlign          = ADC_DataAlign_Right;
    ADC_Init(ADC1, &ADC_InitStruct);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 0, ADC_Samctl_240_5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 0, ADC_Samctl_240_5);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 0, ADC_Samctl_240_5);
    //ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 0, ADC_Samctl_240_5);

    /* All Channel Disable */
    ADC1->ADCHS = 0;

    RCC_AHBPeriphClockCmd(RCC_AHBENR_GPIOA, ENABLE);

    /* PA5(RV1) PA4(RV2) PA1(RV3) */
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    ADC_Cmd(ADC1, ENABLE);
	
}


float ADC_GetChannelVoltage(uint8_t Channel)
{
    uint16_t Value = 0;
    float Voltage;

    ADC1->ADCHS |= (1 << Channel);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    {
    }

    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

    Value = ADC_GetConversionValue(ADC1);



	if( Channel == ADC_Channel_1 ){
	}


	if( Channel == ADC_Channel_4 ){
	}



    Voltage = (float)Value / (float)(4096-1) * (float)3.3;		// 4096

    ADC1->ADCHS &= ~(1 << Channel);

    return (Voltage);
}



uint16_t ADC_GetChannel(uint8_t Channel)
{
    uint16_t Value = 0;
   // float Voltage;

    ADC1->ADCHS |= (1 << Channel);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
    {
    }

    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);

    Value = ADC_GetConversionValue(ADC1);


	ADC1->ADCHS &= ~(1 << Channel);

    return (Value);
}



/***********************************************************************************************************************
  * @brief
  * @note   none
  * @param  none
  * @retval none
  *********************************************************************************************************************/
void ADC_InternalVoltageSensor_Sample(void)
{
    // uint16_t ConversionValue = 0;
    uint16_t CalibrationData = *(uint16_t *)(0x1FFFF7E0);
    float    VrefCalculation = (float)CalibrationData * (float)3.3 / (float)4096.0;


	
	extern uint16_t CurrentADC;
	extern uint16_t TempADC;
	extern uint16_t PA6_ADC;

	//if( ADC_Timer100msec  ) return;
	//ADC_Timer100msec = 100-1;

	#if 0
    printf("\r\nRV1 Voltage = %0.2f  \tRV2 Voltage = %0.2f  \tRV3 Voltage = %0.2f",
               ADC_GetChannelVoltage(ADC_Channel_5),
               ADC_GetChannelVoltage(ADC_Channel_4),
               ADC_GetChannelVoltage(ADC_Channel_1));

	#endif

	CurrentADC 	= ADC_GetChannel(ADC_Channel_1);
	TempADC 	= ADC_GetChannel(ADC_Channel_4);
	PA6_ADC     = ADC_GetChannel(ADC_Channel_6);
	//printf("\r\nCurrentADC[%x] (%u)", CurrentADC, (CurrentADC*330)/(4096-1) );
	//printf("\r\nTempADC[%x] (%u)"   , TempADC,       (TempADC*330)/(4096-1) );
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/********************************************** (C) Copyright MindMotion **********************************************/

