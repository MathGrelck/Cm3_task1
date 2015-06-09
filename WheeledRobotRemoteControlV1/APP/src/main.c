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




/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u32                             Baudrate_DXL = 	1000000;
u32                             Baudrate_PC = 57600;



void __ISR_DELAY(void);






int main(void)
{
    /* System Clocks Configuration */
	RCC_Configuration();

	/* NVIC configuration */
	NVIC_Configuration();

	/* GPIO configuration */
	GPIO_Configuration(); // Setup of IOs

	SysTick_Configuration(); // used for delay function (PC_Com.c)

	DXL_init(1000000);

	USART_Configuration(USART_PC, Baudrate_PC);

	Init_Timer2();

	TxDString(" HELLO :)\n\r");
	DXL_RX_com_buf[14] = 0;

	while(1)
	{

		if(PC_data_rdy == 1)
		{
			PC_data_rdy = 0;

			if(PC_RX_com_buf[0]=='i')
			{
				move_forward(700);
			}
			else if(PC_RX_com_buf[0]=='j')
			{
				move_left(400);
			}
			else if(PC_RX_com_buf[0]=='k')
			{
				move_backward(700);
			}
			else if(PC_RX_com_buf[0]=='l')
			{
				move_right(400);
			}
			else
			{
				mDelay(100);
				move_backward(0);
			}
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



