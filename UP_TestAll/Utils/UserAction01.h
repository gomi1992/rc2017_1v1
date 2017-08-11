//
// Created by admin on 2017/7/14.
//

#ifndef RC2017_1V1_USERACTION01_H
#define RC2017_1V1_USERACTION01_H

extern int UA01_isAttacking;

void UA01_Init();

void UA01_FrontArmUp();

void UA01_FrontArmDown();

void UA01_BackArmUp();

void UA01_BackArmDown();

void UA01_PreAction();

void UA01_GetOnStage(int direction);

int UA01_FindStage();

void UA01_PreAttack();

void UA01_PreAttackDirection(int direction);

void UA01_Attack(int direction);

void UA01_StopAttack();

#endif //RC2017_1V1_USERACTION01_H
