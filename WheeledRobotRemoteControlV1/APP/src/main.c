/************************* (C) COPYRIGHT 2010 ROBOTIS **************************
* File Name          : main.c
* Author             : danceww
* Version            : V0.0.1
* Date               : 08/23/2010
* Description        : Main program body
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "stm32f10x_lib.h"
#include "dynamixel.h"
#include "dxl_hal.h"

#include "sys_init.h"
#include "PC_Com.h"
#include "DXL.h"
#include "DXLdef.h"





/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile u8                   gbpRxInterruptBuffer[256]; // dxl buffer
volatile u8                   gbRxBufferWritePointer,gbRxBufferReadPointer;

u32                             Baudrate_DXL = 	1000000;
u32                             Baudrate_PC = 57600;
vu32                            capture = 0;
u16                            GoalPos[2] = {0, 1023};
//word                            GoalPos[2] = {0, 1023};  //For EX and MX series
u16                            Position;
u16                            wPresentPos;
u8                            INDEX = 0;
u8                            Voltage;
u8                            id = 1;
u8                            bMoving, CommStatus;


//void TimerInterrupt_1ms(void);
void RxD0Interrupt(void);
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
	TxDString(" HELLO :)\n\r");
	DXL_RX_com_buf[14] = 0;
	while(1)
	{
		TxDString("sending");
		TxDString(DXL_RX_com_buf);
		mDelay(5);
		DXL_send_data(10, MOVING_SPEED_L, 0xFF);
		TxDString(DXL_RX_com_buf);
		DXL_send_data(10, MOVING_SPEED_H, 0x01);
		TxDString(DXL_RX_com_buf);
		mDelay(500);


	}

/*	while(1)
	{
		bMoving = dxl_read_byte( id, P_MOVING );
		CommStatus = dxl_get_result();
		if( CommStatus == COMM_RXSUCCESS )
		{
			if( bMoving == 0 )
			{
				// Change goal position
				if( INDEX == 0 )
					INDEX = 1;
				else
					INDEX = 0;

				// Write goal position
				dxl_write_word( id, P_GOAL_POSITION_L, GoalPos[INDEX] );
			}

			PrintErrorCode();

			// Read present position
			wPresentPos = dxl_read_word( id, P_PRESENT_POSITION_L );
			TxDWord16(GoalPos[INDEX]);
			TxDString("   ");
			TxDWord16(wPresentPos);
			TxDByte_PC('\r');
			TxDByte_PC('\n');
		}
		else
			PrintCommStatus(CommStatus);

	}*/
	return 0;
}




/* Interrupt functionality below!! */

/*void TimerInterrupt_1ms(void) //OLLO CONTROL
{
	if (TIM_GetITStatus(TIM2, TIM_IT_CC1) != RESET) // 1ms//
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

		capture = TIM_GetCapture1(TIM2);
		TIM_SetCompare1(TIM2, capture + CCR1_Val);

		if(gw1msCounter > 0)
			gw1msCounter--;
	}
}*/

/*__interrupt*/
void RxD0Interrupt(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
		gbpRxInterruptBuffer[gbRxBufferWritePointer++] = USART_ReceiveData(USART1);
}



void __ISR_DELAY(void)
{
	if (gwTimingDelay != 0x00)
		gwTimingDelay--;
}

/*
void StartDiscount(s32 StartTime)
{
	gw1msCounter = StartTime;
}

u8 CheckTimeOut(void)
{
	// Check timeout
	// Return: 0 is false, 1 is true(timeout occurred)

	if(gw1msCounter == 0)
		return 1;
	else
		return 0;
}*/





/*void USART1_Configuration(u32 baudrate)
{
	USART_Configuration(USART_DXL, baudrate);
}*/



/*
void DisableUSART1(void)
{
	USART_Cmd(USART1, DISABLE);
}

void ClearBuffer256(void)
{
	gbRxBufferReadPointer = gbRxBufferWritePointer = 0;
}

byte CheckNewArrive(void)
{
	if(gbRxBufferReadPointer != gbRxBufferWritePointer)
		return 1;
	else
		return 0;
}

void TxDByte_DXL(byte bTxdData)
{
	GPIO_ResetBits(PORT_ENABLE_RXD, PIN_ENABLE_RXD);	// RX Disable
	GPIO_SetBits(PORT_ENABLE_TXD, PIN_ENABLE_TXD);	// TX Enable

	USART_SendData(USART1,bTxdData);
	while( USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET );

	GPIO_ResetBits(PORT_ENABLE_TXD, PIN_ENABLE_TXD);	// TX Disable
	GPIO_SetBits(PORT_ENABLE_RXD, PIN_ENABLE_RXD);	// RX Enable
}

byte RxDByte_DXL(void)
{
	byte bTemp;

	while(1)
	{
		if(gbRxBufferReadPointer != gbRxBufferWritePointer) break;
	}

	bTemp = gbpRxInterruptBuffer[gbRxBufferReadPointer];
	gbRxBufferReadPointer++;

	return bTemp;
}


// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
	case COMM_TXFAIL:
		TxDString("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		TxDString("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		TxDString("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		TxDString("COMM_RXWAITING: Now recieving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		TxDString("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		TxDString("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		TxDString("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		TxDString("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		TxDString("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		TxDString("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		TxDString("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		TxDString("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		TxDString("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		TxDString("Instruction code error!\n");
} */



/*void TxDWord16(word wSentData)
{
	TxDByte16((wSentData >> 8) & 0xff);
	TxDByte16(wSentData & 0xff);
}*/

/*void TxDByte16(byte bSentData)
{
	byte bTmp;

	bTmp = ((byte) (bSentData >> 4) & 0x0f) + (byte) '0';
	if (bTmp > '9')
		bTmp += 7;
	TxDByte_PC(bTmp);
	bTmp = (byte) (bSentData & 0x0f) + (byte) '0';
	if (bTmp > '9')
		bTmp += 7;
	TxDByte_PC(bTmp);
}*/

