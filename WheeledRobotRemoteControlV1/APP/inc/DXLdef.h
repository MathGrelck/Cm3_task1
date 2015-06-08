/*
 * DXLdef.h
 *
 *  Created on: 08/06/2015
 *      Author: Marc
 */

#ifndef APP_INC_DXLDEF_H_
#define APP_INC_DXLDEF_H_


/* INSTRUCTIONS */
#define PING		0x01
#define READ_DATA 	0x02
#define WRITE_DATA	0x03
#define REG_WRITE	0x04
#define	ACTION		0x05
#define DXL_RESET	0x06
#define SYNC_WRITE	0x83

/*ADDRESSES*/
#define LED				0x19
#define MOVING_SPEED_L	0x20
#define MOVING_SPEED_H	0x21
#endif /* APP_INC_DXLDEF_H_ */
