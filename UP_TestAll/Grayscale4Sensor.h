//
// Created by admin on 2017/6/1.
//

#ifndef RC2017_1V1_GRAYSCALE4SENSOR_H
#define RC2017_1V1_GRAYSCALE4SENSOR_H

#define G4S_GRAY_SCALE_SENSOR_FORWARD 0
#define G4S_GRAY_SCALE_SENSOR_BACKWARD 1
#define G4S_GRAY_SCALE_SENSOR_LEFT 2
#define G4S_GRAY_SCALE_SENSOR_RIGHT 3

void G4S_UpdateGrayScaleSensor();

int G4S_Direction2Centre();

extern int G4S_next_direction;

#endif //RC2017_1V1_GRAYSCALE4SENSOR_H
