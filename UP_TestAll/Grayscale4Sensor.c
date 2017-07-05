//
// Created by admin on 2017/6/1.
//

#include "Grayscale4Sensor.h"
#include "UpLib/UP_System.h"
#include "utils.h"
#include "common.h"
#include "ServoMove.h"

int G4S_next_direction = DIRECTION_NONE;

u8 G4S_GrayScaleSensorList[4] = {0, 0, 0, 0};

int G4S_gray_scale_origin_data[4] = {0, 0, 0, 0};

#define G4S_SENSOR_DATA_LENGTH 4
/*
 * 0->forward
 * 1->backward
 * 2->left
 * 3->right
 */
const int kG4S_SensorData[4][G4S_SENSOR_DATA_LENGTH] = {{3200, 3100, 3020, 2700},
                                                        {3150, 3030, 2720, 2450},
                                                        {3150, 3050, 2720, 2350},
                                                        {3100, 3000, 2680, 2380}};
int motor_speed = 500;

void G4S_UpdateGrayScaleSensor(void) {
    G4S_gray_scale_origin_data[0] = UP_ADC_GetValue(G4S_GrayScaleSensorList[0]);
    G4S_gray_scale_origin_data[1] = UP_ADC_GetValue(G4S_GrayScaleSensorList[1]);
    G4S_gray_scale_origin_data[2] = UP_ADC_GetValue(G4S_GrayScaleSensorList[2]);
    G4S_gray_scale_origin_data[3] = UP_ADC_GetValue(G4S_GrayScaleSensorList[3]);

    G4S_next_direction = G4S_Direction2Centre();

    if (G4S_next_direction == DIRECTION_NONE) {
        SM_Move(DIRECTION_FORWARD, motor_speed);
        return;
    }
    if (G4S_next_direction & DIRECTION_LEFT) {
        SM_Spin(DIRECTION_LEFT, motor_speed);
    } else if (G4S_next_direction & DIRECTION_RIGHT) {
        SM_Spin(DIRECTION_RIGHT, motor_speed);
    }
    if (G4S_next_direction & DIRECTION_FORWARD) {
        SM_Move(DIRECTION_FORWARD, motor_speed);
    } else if (G4S_next_direction & DIRECTION_BACK) {
        SM_Move(DIRECTION_BACK, motor_speed);
    }
}

int G4S_direction_data[4] = {0};

int G4S_Direction2Centre() {
    int i, x = 0, y = 0, result = DIRECTION_NONE;

    //find the nearest value in the array
    for (i = 0; i < 4; i++) {
        G4S_direction_data[i] = find_nearest_in_array_descend(G4S_gray_scale_origin_data[i], kG4S_SensorData[i], 0,
                                                              G4S_SENSOR_DATA_LENGTH);
    }

    y = G4S_direction_data[1] - G4S_direction_data[0];//backward - forward
    x = G4S_direction_data[3] - G4S_direction_data[2];//right - left

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

