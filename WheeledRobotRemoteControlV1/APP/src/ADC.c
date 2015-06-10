/*
 * ADC.c
 *
 *  Created on: 09/06/2015
 *      Author: Marc
 */

#include "stm32f10x_lib.h"
#include "ADC.h"



u16 sampleADC(u8 ADCnum)
{
	u16 ADCres = 0;


	switch(ADCnum)
	{
		case NUM_ADC1:

			ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1 , ADC_SampleTime_239Cycles5);

			GPIO_SetBits(ADC_1_PORT_SIG_MOT, ADC_1_PIN_SIG_MOT1P);
			GPIO_ResetBits(ADC_1_PORT_SIG_MOT, ADC_1_PIN_SIG_MOT1M);

			GPIO_ResetBits(PORT_ADC_SELECT0,PIN_ADC_SELECT0);
			GPIO_ResetBits(PORT_ADC_SELECT1,PIN_ADC_SELECT1);

			uDelay(30);

			/* Start ADC1,ADC2 Software Conversion */
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);

			//ADC_SoftwareStartConvCmd(ADC2, ENABLE);
			uDelay(5);

			ADCres = (ADC_GetConversionValue(ADC1));

			GPIO_SetBits(ADC_1_PORT_SIG_MOT, ADC_1_PIN_SIG_MOT1P);
			GPIO_ResetBits(ADC_1_PORT_SIG_MOT, ADC_1_PIN_SIG_MOT1M);

			break;

		case NUM_ADC2:

			ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1 , ADC_SampleTime_239Cycles5);




			break;

		case NUM_ADC3:

			ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1 , ADC_SampleTime_239Cycles5);




			break;

		case NUM_ADC4:

			ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1 , ADC_SampleTime_239Cycles5);




			break;

		case NUM_ADC5:

			ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1 , ADC_SampleTime_239Cycles5);




			break;

		case NUM_ADC6:

			ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1 , ADC_SampleTime_239Cycles5);




			break;

		default:
			break;

	}

	return ADCres;
}

void init_ADC()
{


	ADC_InitTypeDef ADC_InitStructure;

		ADC_StructInit(&ADC_InitStructure);

		/* ADC1 configuration ------------------------------------------------------*/
		ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
		ADC_InitStructure.ADC_ScanConvMode = DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
		ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfChannel = 1;

		ADC_Init(ADC1, &ADC_InitStructure);

		/* ADC1 regular channels configuration */
		ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1 , ADC_SampleTime_239Cycles5);
		//ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

		/* Enable ADC1 DMA */
		//ADC_DMACmd(ADC1, ENABLE);

		/* Enable ADC1 */
		ADC_Cmd(ADC1, ENABLE);

		/* Enable ADC1 reset calibration register */
		/* Check the end of ADC1 reset calibration register */
		ADC_ResetCalibration(ADC1);
		while(ADC_GetResetCalibrationStatus(ADC1));


		/* Start ADC1 calibration */
		/* Check the end of ADC1 calibration */
		ADC_StartCalibration(ADC1);
		while(ADC_GetCalibrationStatus(ADC1));


		/* Start ADC1 Software Conversion */
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
