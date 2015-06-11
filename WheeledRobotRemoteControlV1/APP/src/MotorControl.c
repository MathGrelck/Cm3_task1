/*
 * MotorControl.c
 *
 *  Created on: 09/06/2015
 *      Author: Marc
 */
#include "stm32f10x_lib.h"

#include "DXL.h"
#include "DXLdef.h"


#define MOTOR_LEFT_ADD 	10
#define MOTOR_RIGHT_ADD	9
#define MOTOR_STEP	2

#define LEFT_MOTOR_CORR 10

u16 old_speed = 0;


void set_IR_position(u16 pos)
{
	if(pos > 1023)
	{
		pos = 1023;
	}

	DXL_send_word(MOTOR_STEP, GOAL_POSITION_L, pos);

}

void move_forward(u16 speed)
{
	u16 temp = 0;

	if(speed > 1023)
	{
		speed = 1023;
	}

	temp = 1024 + speed; //Set MSB!
	old_speed = temp; // save speed for "turn" function
	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, temp);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, speed);
}

void move_backward(u16 speed)
{
	u16 temp = 0;
	if(speed > 1023)
	{
		speed = 1023;
	}

	temp = 1024 + speed; //Set MSB!
	old_speed = speed;
	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, speed);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, temp);

}

void move_left(u16 speed)
{

	if(speed > 1023)
	{
		speed = 1023;
	}
	speed = old_speed - speed;

	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, old_speed);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, speed^1024);

}

void move_right(u16 speed)
{

	if(speed > 1023)
	{
		speed = 1023;
	}
	speed = old_speed - speed;

	DXL_send_word(MOTOR_LEFT_ADD, MOVING_SPEED_L, speed);
	DXL_send_word(MOTOR_RIGHT_ADD, MOVING_SPEED_L, old_speed ^1024);

}

void init_motors()
{



}

