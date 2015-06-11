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

#define IR_SENSOR_RIGHT				2
#define IR_SENSOR_RIGHT_front		3
#define IR_SENSOR_FRONT				1
#define IR_SENSOR_LEFT_front		0
#define IR_SENSOR_LEFT				4


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

u32                             Baudrate_DXL = 	1000000;
u32                             Baudrate_PC = 57600;
u16								ADCres_buf[10];
u16 diff = 0;
float SCALE = 0.7f;

void __ISR_DELAY(void);





int main(void)
{
	s16 i = 0;
	u16 tempADCres, j = 0, k = 0;
	u16 sum, avg;
	float hans;
	double bo;
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

	GPIO_SetBits(ADC_6_PORT_SIG_MOT, ADC_6_PIN_SIG_MOT1P);
	GPIO_ResetBits(ADC_6_PORT_SIG_MOT, ADC_6_PIN_SIG_MOT1M);

	while(1)
	{

// MEASUREMENTS AND AVERAGE OF SENSORS
		for(k = 0; k < 5; k++){
			sum = 0;

/*			for(j = 0; j < 9; j++){
				sum = sampleADC(i);
			}
*/
			sum = sampleADC(k);	

			switch(i){
				case 0:
				// IR_SENSOR_LEFT_front
				TxDString("sensor 1:");
				break;

				case 1:
				//IR_SENSOR_front
				TxDString("sensor 2:");
				break;

				case 2:
				//IR_SENSOR_RIGHT
				TxDString("sensor 3:");
				break;

				case 3:
				//IR_SENSOR_RIGHT_front
				TxDString("sensor 4:");
				break;

				case 4:
				//IR_SENSOR_LEFT
				TxDString("sensor 5:");
				break;
			}
			TxDByte_PC((avg&0xFF00)>>8);
			TxDByte_PC((avg&0x00FF));
			TxDString("\r");
		}	

		mDelay(2000);
/*
// SIMPLE ORIENTATION BEHAVIOUR
		for (j = 0; j<5; j++)
		{
			ADCres_buf[j] = sampleADC(NUM_ADC1+j);

		}
		move_forward(400);
		if((ADCres_buf[IR_SENSOR_RIGHT] >ADCres_buf[IR_SENSOR_LEFT]) || (ADCres_buf[IR_SENSOR_RIGHT_front] > ADCres_buf[IR_SENSOR_LEFT_front]))
		{
			if(ADCres_buf[IR_SENSOR_FRONT] > 200)
			{
				move_right(((ADCres_buf[IR_SENSOR_RIGHT] - ADCres_buf[IR_SENSOR_LEFT])) + ((ADCres_buf[IR_SENSOR_RIGHT_front]-ADCres_buf[IR_SENSOR_LEFT_front])*2) + ADCres_buf[IR_SENSOR_FRONT]*2);
			}

			move_left(((ADCres_buf[IR_SENSOR_RIGHT] - ADCres_buf[IR_SENSOR_LEFT])) + ((ADCres_buf[IR_SENSOR_RIGHT_front]-ADCres_buf[IR_SENSOR_LEFT_front]))*2);
		}
		else if((ADCres_buf[IR_SENSOR_LEFT] > ADCres_buf[IR_SENSOR_RIGHT]) || (ADCres_buf[IR_SENSOR_LEFT_front] > ADCres_buf[IR_SENSOR_RIGHT_front]) )
		{
			if(ADCres_buf[IR_SENSOR_FRONT] > 300)
			{
				move_right(((ADCres_buf[IR_SENSOR_LEFT] - ADCres_buf[IR_SENSOR_RIGHT])) + ((ADCres_buf[IR_SENSOR_LEFT_front]-ADCres_buf[IR_SENSOR_RIGHT_front])*2) + ADCres_buf[IR_SENSOR_FRONT]*2);
			}

			move_right(((ADCres_buf[IR_SENSOR_LEFT] - ADCres_buf[IR_SENSOR_RIGHT])) + ((ADCres_buf[IR_SENSOR_LEFT_front]-ADCres_buf[IR_SENSOR_RIGHT_front]))*2);
		}


*/


/*
//SWEEP CONTROLLER
		tempADCres = 0;
		TxDString("1\n\r");
		set_IR_position(214);
		while(DXL_RX_com_buf[5]&0x01)
		{
			DXL_read_byte(2,MOVING);
			TxDByte_PC(DXL_RX_com_buf[5]);

		}
		mDelay(5000);
		TxDString("2\n\r");
		set_IR_position(814);
		while(DXL_RX_com_buf[5]&0x01)
		{
			DXL_read_byte(2,MOVING);
			TxDByte_PC(DXL_RX_com_buf[5]);

		}
		mDelay(1000);
		
		i = 214;
		while(i <= 814)
		{
			set_IR_position(i);

			for (j = 0; j<10; j++)
			{
				tempADCres += (sampleADC(NUM_ADC1+5));
				
			}
			TxDString("ADC data ");
			TxDString(": ");
			TxDByte_PC((tempADCres&0xFF00)>>8);
			TxDByte_PC((tempADCres&0x00FF));
			TxDString("\n\r");
			i +=40;
		}

		mDelay(1000);
		i = 774;
		tempADCres = 0;
		while(i >= 214)
		{
			set_IR_position(i);
			for (j = 0; j<10; j++)
			{
				tempADCres += (sampleADC(NUM_ADC1+5));

			}
			TxDString("ADC data ");
			TxDString(": ");
			TxDByte_PC((tempADCres&0xFF00)>>8);
			TxDByte_PC((tempADCres&0x00FF));
			TxDString("\n\r");
			i -=40;
		} */



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

