/*
 * MotorControl.h
 *
 *  Created on: 09/06/2015
 *      Author: Marc
 */

#ifndef APP_INC_MOTORCONTROL_H_
#define APP_INC_MOTORCONTROL_H_

#include "stm32f10x_lib.h"

void move_forward(u32 speed);
void move_backward(u32 speed);
void move_left(u32 speed);
void move_right(u32 speed);
void init_motors();
void set_IR_position(u16 pos);
#endif /* APP_INC_MOTORCONTROL_H_ */
