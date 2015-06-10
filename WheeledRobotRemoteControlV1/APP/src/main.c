/************************* (C) COPYRIGHT 2010 ROBOTIS **************************
* File Name          : main.c
* Author             : danceww
* Version            : V0.0.1
* Date               : 08/23/2010
* Description        : Main program body
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_lib.h"

#include "sys_init.h"
#include "PC_Com.h"
#include "DXL.h"
#include "DXLdef.h"
#include "MotorControl.h"

#include "ADC.h"

#define PORT_SIG_MOT1P          GPIOA
#define PORT_SIG_MOT1M          GPIOA

#define PIN_SIG_MOT1P           GPIO_Pin_0 //Turn on pin
#define PIN_SIG_MOT1M           GPIO_Pin_1 //Turn on pin

#define	SIG_ADC_1 				ADC_Channel_5 //select adc channel 
#define SIG_ADC_0				ADC_Channel_10  // select adc channel 

#define PORT_ADC_SELECT0        GPIOC
#define PORT_ADC_SELECT1        GPIOC

#define PORT_LED_POWER			GPIOC

#define PIN_ADC_SELECT0         GPIO_Pin_1
#define PIN_ADC_SELECT1         GPIO_Pin_2
#define PIN_ADC0				GPIO_Pin_0
#define PIN_VDD_VOLT			GPIO_Pin_3

#define PIN_PC_TXD				GPIO_Pin_10

#define PIN_LED_POWER			GPIO_Pin_13


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u32                             Baudrate_DXL = 	1000000;
u32                             Baudrate_PC = 57600;



void __ISR_DELAY(void);
void GPIO_Start(void);





int main(void)
{
	s16 i = 0;
	u16 tempADCres, j;
    /* System Clocks Configuration */
	RCC_Configuration();

	/* NVIC configuration */
	NVIC_Configuration();

	/* GPIO configuration */
	GPIO_Configuration(); // Setup of IOs
	//GPIO_Start();

	SysTick_Configuration(); // used for delay function (PC_Com.c)

	DXL_init(1000000);

	USART_Configuration(USART_PC, Baudrate_PC);

	Init_Timer2();

	TxDString(" HELLO :)\n\r");
	DXL_RX_com_buf[14] = 0;

	init_ADC();

	while(1)
	{
		TxDString("starting wait\n\r");

		mDelay(5000);
		i = 0;
		while(i <= 410)
		{
			set_IR_position(i);

			for (j = 0; j<6; j++)
			{
				tempADCres = sampleADC(NUM_ADC1+j);
				TxDString("ADC data ");
				TxDByte_PC(j);
				TxDString(": ");
				TxDByte_PC((tempADCres&0xFF00)>>8);
				TxDByte_PC((tempADCres&0x00FF));
				TxDString("\n\r");
				
			}
			i +=25;
		}

		mDelay(1000);
		i = 385;
		
		while(i >= 0)
		{
			set_IR_position(i);
			for (j = 0; j<6; j++)
			{
				tempADCres = sampleADC(NUM_ADC1+j);
				TxDString("ADC data ");
				TxDByte_PC(j);
				TxDString(": ");
				TxDByte_PC((tempADCres&0xFF00)>>8);
				TxDByte_PC((tempADCres&0x00FF));
				TxDString("\n\r");
				
			}
			i -=25;
		}



	}

	return 0;
}


void __TIM2_ISR()
{
	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET)
	{
		TxDString("TIM2 ISR\n \r");
		TIM2->SR &= ~TIM_IT_CC1;
	}
}


void __ISR_DELAY(void)
{
	if (gwTimingDelay != 0x00)
		gwTimingDelay--;
}

