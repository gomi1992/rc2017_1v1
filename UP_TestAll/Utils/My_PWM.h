//
// Created by admin on 2017/9/8.
//

#ifndef RC2017_1V1_MY_PWM_H
#define RC2017_1V1_MY_PWM_H

void My_PWM_Init(void);                                //PWM相关初始化
void My_PWM_Enable(u8 Channel, FunctionalState NewState);    //使能PWM模式输出，通道0~3，状态ENABLE或DISABLE
void My_PWM_SetDutyTime(u8 Channel, u16 DutyTime);    //设置高电平时长0~4095对应0%~100%
void My_PWM_SetFrequency(u32 Frequency);                //设置频率（Hz），50~1000000
void My_PWM_SetIO(u8 Channel, u8 Value);                //设置PWM端口IO输出电平，通道0~3，值0或1

#endif //RC2017_1V1_MY_PWM_H
