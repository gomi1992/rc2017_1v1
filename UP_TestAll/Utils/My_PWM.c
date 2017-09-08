//
// Created by admin on 2017/9/8.
//

#include "../UpLib/STM32Lib/stm32f10x.h"
#include "My_PWM.h"

#define My_PWM_NUM        4

#define My_PWM0_L        (GPIOA->BSRR = GPIO_Pin_6)
#define My_PWM0_H        (GPIOA->BRR = GPIO_Pin_6)
#define My_PWM1_L        (GPIOA->BSRR = GPIO_Pin_7)
#define My_PWM1_H        (GPIOA->BRR = GPIO_Pin_7)
#define My_PWM2_L        (GPIOB->BSRR = GPIO_Pin_0)
#define My_PWM2_H        (GPIOB->BRR = GPIO_Pin_0)
#define My_PWM3_L        (GPIOB->BSRR = GPIO_Pin_1)
#define My_PWM3_H        (GPIOB->BRR = GPIO_Pin_1)

u32 g_My_PWMPeriod = 24000;
u16 g_My_PWMDutyTime[My_PWM_NUM] = {2048, 2048, 2048, 2048};
u8 g_My_bPWMMode[My_PWM_NUM] = {0};
TIM_OCInitTypeDef g_TIM_OCInitStructure;

void My_PWM_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    My_PWM2_L;
    My_PWM3_L;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Prescaler = 2;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 24000;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0x0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    g_TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    g_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //����ͨ����Ч
    g_TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;//����ͨ����Ч
    g_TIM_OCInitStructure.TIM_Pulse = 12000;                            //50%ռ��ʱ��
    g_TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //�������
    g_TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;     //�����˵ļ���
    g_TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
    g_TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_Cmd(TIM3, ENABLE);

    TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

void My_PWM_SetDutyTime(u8 Channel, u16 DutyTime) {
    if (Channel > My_PWM_NUM || DutyTime > 4095)
        return;
    g_My_PWMDutyTime[Channel] = DutyTime;
    switch (Channel) {
        case 0:
            TIM3->CCR1 = (g_My_PWMPeriod * DutyTime) / 4095;
            break;
        case 1:
            TIM3->CCR2 = (g_My_PWMPeriod * DutyTime) / 4095;
            break;
        case 2:
            TIM3->CCR3 = (g_My_PWMPeriod * DutyTime) / 4095;
            break;
        case 3:
            TIM3->CCR4 = (g_My_PWMPeriod * DutyTime) / 4095;
            break;
        default:
            break;
    }
}

void My_PWM_SetFrequency(u32 Frequency) {
    u16 temp = 0;
    if (Frequency != 0) {
        TIM3->PSC = temp = 2000 / Frequency;        //��Ƶֵ��Ŀ��Ƶ��Խ�ͷ�ƵԽ��
        TIM3->ARR = g_My_PWMPeriod = 72000000 / (temp + 1) / Frequency;    //����ֵ
        temp = g_My_PWMPeriod >> 1;
        TIM3->CCR1 = temp;            //ռ��ʱ��Ĭ��50%
        TIM3->CCR2 = temp;
        TIM3->CCR3 = temp;
        TIM3->CCR4 = temp;
    }
}

void My_PWM_SetIO(u8 Channel, u8 Value) {
    if (Channel > My_PWM_NUM || g_My_bPWMMode[Channel] == 1)
        return;
    switch (Channel)                                //����Ӧ�Ķ˿����ó���Ҫ�ĵ�ƽ
    {
        case 0:
            Value ? My_PWM0_H : My_PWM0_L;
            break;
        case 1:
            Value ? My_PWM1_H : My_PWM1_L;
            break;
        case 2:
            Value ? My_PWM2_H : My_PWM2_L;
            break;
        case 3:
            Value ? My_PWM3_H : My_PWM3_L;
            break;
        default:
            break;
    }
}

void My_PWM_Enable(u8 Channel, FunctionalState NewState) {
    if (Channel > My_PWM_NUM)
        return;
    if (NewState == ENABLE) {
        g_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;    //����ͨ����Ч
        g_TIM_OCInitStructure.TIM_Pulse = (g_My_PWMPeriod * g_My_PWMDutyTime[Channel]) / 4095;    //ռ��ʱ��
        g_My_bPWMMode[Channel] = 1;
    } else {
        g_TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;    //����ͨ����Ч
        g_My_bPWMMode[Channel] = 0;
        My_PWM_SetIO(Channel, 0);
    }
    switch (Channel) {
        case 0:
            TIM_OC1Init(TIM3, &g_TIM_OCInitStructure);
            break;
        case 1:
            TIM_OC2Init(TIM3, &g_TIM_OCInitStructure);
            break;
        case 2:
            TIM_OC3Init(TIM3, &g_TIM_OCInitStructure);
            break;
        case 3:
            TIM_OC4Init(TIM3, &g_TIM_OCInitStructure);
            break;
        default:
            break;
    }
}