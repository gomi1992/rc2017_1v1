//
// Created by ilalh on 2017/6/12.
//

#ifndef RC2017_1V1_SERVOMOVE_H
#define RC2017_1V1_SERVOMOVE_H

void SM_Init();

void SM_Move(int direction, int speed);

void SM_Spin(int direction, int speed);

void SM_TurnAround(int direction, int is_forward, int speed, int diff_speed);

void SM_ServoMove(int left_speed, int right_speed);

#endif //RC2017_1V1_SERVOMOVE_H
