/*
 * AX12W.h
 *
 *  Created on: 08/06/2015
 *      Author: Marc
 */

#include "stm32f10x_lib.h"

void DXL_init(u32 );//baud



/* FOR ISR */
void DXL_RX_interrupt(void);
void DXL_send_data(u8 , u8 , u8);

extern vu8 DXL_RX_com_buf[15];
