//
// Created by admin on 2017/7/14.
//

#include "common.h"
#include "UserAction01.h"
#include "UpLib/UP_System.h"
#include "Grayscale4Sensor.h"
#include "ServoMove.h"
#include "CheckState.h"
#include "stdlib.h"

u8 UA01_arm_servo_list[4] = {3, 4, 5, 6};
u8 UA01_hand_motor_list[4] = {7, 8, 9, 10};
s16 UA01_hand_motor_speed[4] = {0, 0, 0, 0};

void UA01_Init() {
    int i = 0;
    for (i = 0; i < 4; i++) {
        UP_CDS_SetMode(UA01_arm_servo_list[i], CDS_SEVMODE);
        UP_CDS_SetMode(UA01_hand_motor_list[i], CDS_MOTOMODE);
    }
}

void UA01_FrontArmUp() {
    UP_CDS_SetAngle(UA01_arm_servo_list[0], 250, 512);
    UP_CDS_SetAngle(UA01_arm_servo_list[1], 750, 512);
}

void UA01_FrontArmDown() {
    UP_CDS_SetAngle(UA01_arm_servo_list[0], 750, 512);
    UP_CDS_SetAngle(UA01_arm_servo_list[1], 250, 512);
}

void UA01_BackArmUp() {
    UP_CDS_SetAngle(UA01_arm_servo_list[2], 750, 512);
    UP_CDS_SetAngle(UA01_arm_servo_list[3], 250, 512);
}

void UA01_BackArmDown() {
    UP_CDS_SetAngle(UA01_arm_servo_list[2], 250, 512);
    UP_CDS_SetAngle(UA01_arm_servo_list[3], 750, 512);
}

void UA01_PreAction() {
    int i = 0;
    for (i = 0; i < 4; i++) {
        UA01_hand_motor_speed[i] = 0;
    }
    UP_CDS_Set4MotoSpeed(UA01_hand_motor_list, UA01_hand_motor_speed);
    UA01_FrontArmUp();
    UA01_BackArmUp();
}

void UA01_GetOnStage(int direction) {
    UA01_PreAction();
    if (direction == DIRECTION_FORWARD) {
        UA01_hand_motor_speed[0] = 500;
        UA01_hand_motor_speed[1] = -500;
        UA01_hand_motor_speed[2] = 500;
        UA01_hand_motor_speed[3] = -500;
        UP_CDS_Set4MotoSpeed(UA01_hand_motor_list, UA01_hand_motor_speed);
        SM_Move(DIRECTION_FORWARD, 500);
    } else {
        UA01_hand_motor_speed[0] = -500;
        UA01_hand_motor_speed[1] = 500;
        UA01_hand_motor_speed[2] = -500;
        UA01_hand_motor_speed[3] = 500;
        UP_CDS_Set4MotoSpeed(UA01_hand_motor_list, UA01_hand_motor_speed);
        SM_Move(DIRECTION_BACK, 500);
    }
    UP_delay_ms(100);
    if (direction == DIRECTION_FORWARD) UA01_FrontArmDown();
    else UA01_BackArmDown();
    UP_delay_ms(1000);
    if (direction == DIRECTION_FORWARD) UA01_BackArmDown();
    else UA01_FrontArmDown();
    UP_delay_ms(1000);
    if (direction == DIRECTION_FORWARD) UA01_FrontArmUp();
    else UA01_BackArmUp();
    if (direction == DIRECTION_FORWARD) UA01_BackArmUp();
    else UA01_FrontArmUp();
    UP_delay_ms(400);
}

int UA01_FindStage() {
    int stable_count = 0;

    while (CS_State != STATE_UNDER_STAGE_FACE_TO_STAGE) {
        SM_Spin(DIRECTION_LEFT, 300);
        UP_delay_ms(100);
    }
    SM_Move(DIRECTION_FORWARD, 0);

    while (1) {
        stable_count = 0;
        while (stable_count < 10) {
            SM_Move(DIRECTION_FORWARD, 500);
            if (G4S_direction_data[2] > 2 && G4S_direction_data[3] > 2 && G4S_direction_data[0] > 2)
                return DIRECTION_FORWARD;
            else
                stable_count++;
            UP_delay_ms(200);
        }
        stable_count = 0;
        while (stable_count < 10) {
            SM_Move(DIRECTION_BACK, 500);
            if (G4S_direction_data[2] > 2 && G4S_direction_data[3] > 2 && G4S_direction_data[1] > 2)
                return DIRECTION_BACK;
            else
                stable_count++;
            UP_delay_ms(200);
        }

        SM_Spin(DIRECTION_LEFT, 300);
        UP_delay_ms(400);
    }

}