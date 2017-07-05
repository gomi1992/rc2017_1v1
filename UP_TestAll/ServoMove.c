//
// Created by ilalh on 2017/6/12.
//

#include "ServoMove.h"
#include "common.h"
#include "UpLib/STM32Lib/stm32f10x.h"
#include "UpLib/UP_CDS5500.h"

//SM_Motor_Servo_List[0]--left motor
//SM_Motor_Servo_List[1]--right motor
u8 SM_Motor_Servo_List[4] = {1, 2, 0, 0};
s16 SM_Motor_Servo_Speed[4] = {0, 0, 0, 0};

void SM_Init() {
    UP_CDS_SetMode(SM_Motor_Servo_List[0], CDS_MOTOMODE);
    UP_CDS_SetMode(SM_Motor_Servo_List[1], CDS_MOTOMODE);
}

void SM_Move(int direction, int speed) {
    speed = speed > 0 ? speed : -speed;
    if (direction == DIRECTION_FORWARD) {
        SM_ServoMove(speed, -speed);
    } else if (direction == DIRECTION_BACK) {
        SM_ServoMove(-speed, speed);
    } else {
        SM_ServoMove(0, 0);
    }
}

void SM_Spin(int direction, int speed) {
    speed = speed > 0 ? speed : -speed;
    if (direction == DIRECTION_LEFT) {
        SM_ServoMove(-speed, -speed);
    } else if (direction == DIRECTION_RIGHT) {
        SM_ServoMove(speed, speed);
    }
}

void SM_TurnAround(int direction, int is_forward, int speed, int diff_speed) {
    speed = speed > 0 ? speed : -speed;
    diff_speed = diff_speed > 0 ? diff_speed : -diff_speed;
    int left_speed = 0, right_speed = 0;
    if (direction == DIRECTION_LEFT) {
        left_speed = speed;
        right_speed = speed + diff_speed;
    } else if (direction == DIRECTION_RIGHT) {
        left_speed = speed + diff_speed;
        right_speed = speed;
    }

    if (is_forward == DIRECTION_FORWARD) {
        SM_ServoMove(left_speed, -right_speed);
    } else {
        SM_ServoMove(-left_speed, right_speed);
    }
}

void SM_ServoMove(int left_speed, int right_speed) {
    SM_Motor_Servo_Speed[0] = left_speed;
    SM_Motor_Servo_Speed[1] = right_speed;
    UP_CDS_Set4MotoSpeed(SM_Motor_Servo_List, SM_Motor_Servo_Speed);
}
