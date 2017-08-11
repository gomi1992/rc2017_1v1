//
// Created by admin on 2017/6/1.
//

#include "Grayscale4Sensor.h"
#include "../UpLib/UP_System.h"
#include "utils.h"
#include "common.h"
#include "ServoMove.h"
#include "stdlib.h"
#include "CheckState.h"

int G4S_Enable = DISABLE;

int G4S_danger = 0;

int G4S_next_direction = DIRECTION_NONE;

u8 G4S_GrayScaleSensorList[4] = {0, 0, 0, 0};

int G4S_gray_scale_origin_data[4] = {0, 0, 0, 0};

int G4S_direction_data[4] = {0};

int update_count = 0;
int rand_move_flag = 0;

void G4S_init() {
    UP_Timer_EnableIT(0, 5000);//5ms    设置定时器
    UP_Timer_SetHadler(0, G4S_UpdateGrayScaleSensor);//定时器中断 四灰度检测
    debug_bluetooth_puts("g4s init\n");
}

void G4S_enable(int enable) {
    if (enable == ENABLE) {
        debug_bluetooth_puts("g4s enable\n");
        G4S_Enable = ENABLE;
    } else {
        debug_bluetooth_puts("g4s disable\n");
        G4S_Enable = DISABLE;
        G4S_next_direction = DIRECTION_NONE;
    }
}

//数值更新
void G4S_UpdateGrayScaleSensor(void) {
    int i = 0;
//    debug_bluetooth_puts("g4s update\n");
    for (i = 0; i < 4; i++) {
        G4S_gray_scale_origin_data[i] = UP_ADC_GetValue(G4S_GrayScaleSensorList[i]);
    }

    G4S_next_direction = G4S_Direction2Centre();

    if (update_count % 100 == 0)
        srand(G4S_gray_scale_origin_data[0]);
    update_count++;
    update_count %= 10000;
    if (update_count % 200 == 0)
        rand_move_flag = 1;

    if (G4S_Enable == DISABLE)
        return;

    if (G4S_danger == 0 && (((CS_EnemyState & 0x0f) != 0) || ((CS_State & 0x0f) != 0)))
        return;

    if (G4S_next_direction == DIRECTION_NONE && rand_move_flag == 1) {
        switch (rand() % 10) {
            case 0:
            case 1:
            case 2:
                SM_Move(DIRECTION_FORWARD, MOTOR_SPEED);
                break;
            case 3:
            case 4:
            case 5:
                SM_Move(DIRECTION_BACK, MOTOR_SPEED);
                break;
            case 6:
            case 7:
                SM_Spin(DIRECTION_LEFT, MOTOR_SPEED);
                break;
            case 8:
            case 9:
                SM_Spin(DIRECTION_RIGHT, MOTOR_SPEED);
                break;
            default:
                SM_Move(DIRECTION_FORWARD, MOTOR_SPEED);
                break;
        }
        rand_move_flag = 0;
        return;
    }

    if (G4S_next_direction & DIRECTION_LEFT) {
        SM_Spin(DIRECTION_LEFT, MOTOR_SPEED);
    } else if (G4S_next_direction & DIRECTION_RIGHT) {
        SM_Spin(DIRECTION_RIGHT, MOTOR_SPEED);
    }
    if (G4S_next_direction & DIRECTION_FORWARD) {
        SM_Move(DIRECTION_FORWARD, MOTOR_SPEED);
    } else if (G4S_next_direction & DIRECTION_BACK) {
        SM_Move(DIRECTION_BACK, MOTOR_SPEED);
    }
}


//方向判断   返回方向值
int G4S_Direction2Centre() {
    int i, x = 0, y = 0, result = DIRECTION_NONE;

    G4S_danger = 0;
    //find the nearest value in the array
    for (i = 0; i < 4; i++) {
//        G4S_direction_data[i] = find_nearest_in_array_descend(G4S_gray_scale_origin_data[i], kG4S_SensorData[i], 0,
//                                                              G4S_SENSOR_DATA_LENGTH);
//        G4S_direction_data[i] = find_in_array_descend_upper(G4S_gray_scale_origin_data[i], kG4S_SensorData[i], 0,
//                                                            G4S_SENSOR_DATA_LENGTH);
        G4S_direction_data[i] = find_in_array_descend_lower(G4S_gray_scale_origin_data[i], kG4S_SensorData[i], 0,
                                                            G4S_SENSOR_DATA_LENGTH);
        if (G4S_direction_data[i] > 2) {
            G4S_danger = 1;
        }
    }

    y = G4S_direction_data[G4S_GRAY_SCALE_SENSOR_BACKWARD] -
        G4S_direction_data[G4S_GRAY_SCALE_SENSOR_FORWARD];//backward - forward
    x = G4S_direction_data[G4S_GRAY_SCALE_SENSOR_RIGHT] -
        G4S_direction_data[G4S_GRAY_SCALE_SENSOR_LEFT];//right - left

    if (x == 0 && y == 0) {
        return result;
    }
    if (x > 0) {
        result |= DIRECTION_LEFT;
    } else if (x < 0) {
        result |= DIRECTION_RIGHT;
    }

    if (y > 0) {
        result |= DIRECTION_FORWARD;
    } else if (y < 0) {
        result |= DIRECTION_BACK;
    }

    return result & 0x0f;
}

