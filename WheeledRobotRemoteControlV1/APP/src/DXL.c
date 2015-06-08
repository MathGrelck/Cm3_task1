/*
 * AX12W.c
 *
 *  Created on: 08/06/2015
 *      Author: Marc
 */


#include "stm32f10x_lib.h"
#include "DXL.h"
#include "PC_Com.h"
#include "DXLdef.h"

#define PIN_ENABLE_TXD			GPIO_Pin_4
#define PIN_ENABLE_RXD			GPIO_Pin_5
#define PIN_DXL_TXD				GPIO_Pin_6
#define PIN_DXL_RXD				GPIO_Pin_7

#define PORT_ENABLE_TXD			GPIOB
#define PORT_ENABLE_RXD			GPIOB
#define PORT_DXL_TXD			GPIOB
#define PORT_DXL_RXD			GPIOB


/* GLOBAL VAR */

vu8 DXL_RX_com_buf[15]; // FOR ISR
u8 DXL_TX_com_buf[15];

/* LOCAL VAR */
vu32 DXL_RX_buff_index = 0;
u32 DXL_TX_buff_index = 0;

/* LOCAL FUNC */
void USARTConfiguration();
unsigned short update_crc(unsigned short crc_accum, unsigned char *data_blk_ptr, unsigned short data_blk_size);
void DXL_TX(u8 *dataPtr, u8 len);


void DXL_init(u32 baud)
{
	USARTConfiguration(baud);
}


void DXL_send_data(u8 devId, u8 add, u8 data)
{
	u8 i, checksum = 0;
	// Preamble
	DXL_TX_com_buf[0] = 0xff;
	DXL_TX_com_buf[1] = 0xff;

	//id
	DXL_TX_com_buf[2] = devId;

	//length (�the number of Parameters (N) + 2�) = 4 (for write)
	DXL_TX_com_buf[3] = 0x04;

	// instruction
	DXL_TX_com_buf[4] = WRITE_DATA;


	// parameters
	DXL_TX_com_buf[5] = add;
	DXL_TX_com_buf[6] = data;

	// Calc CRC!
	checksum = 0;
	for( i=0; i<5; i++ )
	{
		checksum += DXL_TX_com_buf[i+2];
	}
	DXL_TX_com_buf[7] = ~checksum;


	DXL_TX_com_buf[8] = 0x00; // NULL termination!


	DXL_RX_buff_index = 0; // RX buffer index!

	DXL_TX(DXL_TX_com_buf, 8); // send data


}

void DXL_read_data(u8 devId, u8 function, u8 param, u16 numParam)
{

}


void DXL_TX(u8 *dataPtr, u8 len)
{
	u8 i = 0;
	GPIO_ResetBits(PORT_ENABLE_RXD, PIN_ENABLE_RXD);	// RX Disable
	GPIO_SetBits(PORT_ENABLE_TXD, PIN_ENABLE_TXD);	// TX Enable


	for(i = 0; i < len; i++)
	{
		USART_SendData(USART1, *dataPtr);
		while( USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET ){} // wait for TX to complete
		dataPtr++;
	}


	GPIO_ResetBits(PORT_ENABLE_TXD, PIN_ENABLE_TXD);	// TX Disable
	GPIO_SetBits(PORT_ENABLE_RXD, PIN_ENABLE_RXD);	// RX Enable
}



void DXL_RX(u8 devId, u8 *add)
{

}

void DXL_RX_interrupt(void)
{/* This funtion is clled when 8 bitsisrecied through the UART (stm32f10c_it.c) */
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) // IF RX interrupt RXNE bit auto-clear!
	{
		DXL_RX_com_buf[DXL_RX_buff_index] = USART_ReceiveData(USART1);
	}

}




void USARTConfiguration(u32 baudrate)
{ /* !!!! STOLEN !!! */

	USART_InitTypeDef USART_InitStructure;

	USART_StructInit(&USART_InitStructure);


	USART_InitStructure.USART_BaudRate = baudrate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;


		USART_DeInit(USART1);
		mDelay(10);
		/* Configure the USART1 */
		USART_Init(USART1, &USART_InitStructure);

		/* Enable USART1 Receive and Transmit interrupts */
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		//USART_ITConfig(USART1, USART_IT_TC, ENABLE);

		/* Enable the USART1 */
		USART_Cmd(USART1, ENABLE);

}

