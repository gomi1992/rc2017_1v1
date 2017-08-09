#include "../UpLib/UP_System.h"
#include "../Utils/common.h"
#include "../Utils/Grayscale4Sensor.h"
#include "../Utils/ServoMove.h"
#include "../Utils/CheckState.h"
#include "../Utils/UserAction01.h"
#include "../Utils/utils.h"


#ifdef DEBUG_ON
//then the program is in debug mode
#endif

//0-3 灰度传感器
//4-7 底部四红外
//8-11 8-9前红外，10-11后红外
//12-13 横倾角，竖倾角
u8 GLOBAL_SENSOR_LIST[14] = {0, 1, 2, 3, 4, 5, 6, 7, 12, 13, 14, 15, 8, 9};

const int kG4S_SensorData[4][G4S_SENSOR_DATA_LENGTH] = {{3500, 3470, 3400, 3350},
                                                        {3450, 3410, 3360, 3300},
                                                        {3429, 3380, 3362, 3308},
                                                        {3360, 3328, 3250, 3200}};

void init() {
    G4S_GrayScaleSensorList[0] = GLOBAL_SENSOR_LIST[0];
    G4S_GrayScaleSensorList[1] = GLOBAL_SENSOR_LIST[1];
    G4S_GrayScaleSensorList[2] = GLOBAL_SENSOR_LIST[2];
    G4S_GrayScaleSensorList[3] = GLOBAL_SENSOR_LIST[3];
    CS_IRSensorList[0] = GLOBAL_SENSOR_LIST[4];
    CS_IRSensorList[1] = GLOBAL_SENSOR_LIST[5];
    CS_IRSensorList[2] = GLOBAL_SENSOR_LIST[6];
    CS_IRSensorList[3] = GLOBAL_SENSOR_LIST[7];
    CS_IRSensorList[4] = GLOBAL_SENSOR_LIST[8];
    CS_IRSensorList[5] = GLOBAL_SENSOR_LIST[9];
    CS_IRSensorList[6] = GLOBAL_SENSOR_LIST[10];
    CS_IRSensorList[7] = GLOBAL_SENSOR_LIST[11];
    CS_InclinationSensorList[0] = GLOBAL_SENSOR_LIST[12];
    CS_InclinationSensorList[1] = GLOBAL_SENSOR_LIST[13];

    SM_Init();
    UA01_Init();
}

int main(void) {

#ifdef DEBUG_ON
    UP_Bluetooth_EnableIT();//开启蓝牙
#endif

    G4S_init();
    CS_Init();
    UP_System_Init();   //系统初始化
    UP_delay_ms(100);

    init();     //初始化

    while (!(UP_ADC_GetIO(CS_IRSensorList[2]) == 0 && UP_ADC_GetIO(CS_IRSensorList[3]) == 0));

    UA01_GetOnStage(DIRECTION_FORWARD);//上台

    G4S_enable(ENABLE); //使能
    CS_enable(ENABLE);  //使能

    int under_stage_count = 0;  //擂台计数
    while (1) {
        if ((CS_State & 0xf0) == 0) {
            debug_bluetooth_puts("On stage\n");

            under_stage_count = 0;
            G4S_enable(ENABLE);
            CS_enable(DISABLE);

            UA01_PreAttack();
            if (G4S_danger) {
                debug_bluetooth_puts("g4s danger\n");
                UA01_StopAttack();
                UP_delay_ms(20);
                continue;
            }
            if (CS_EnemyState != 0) {
                if (CS_EnemyState & 0x03) {
                    debug_bluetooth_puts("cs ef\n");
                    UA01_Attack(DIRECTION_FORWARD);
                } else {
                    debug_bluetooth_puts("cs eb\n");
                    UA01_Attack(DIRECTION_BACK);
                }
                UP_delay_ms(5);
                continue;
            }
            switch (CS_State) {
                case STATE_ON_STAGE:
                    debug_bluetooth_puts("OS\n");
                    UA01_PreAttack();
                    break;
                case STATE_ENEMY_FORWARD:
                    debug_bluetooth_puts("EF\n");
                    UA01_Attack(DIRECTION_FORWARD);
                    break;
                case STATE_ENEMY_BACKWARD:
                    debug_bluetooth_puts("EB\n");
                    UA01_Attack(DIRECTION_BACK);
                    break;
                case STATE_ENEMY_LEFT:
                    debug_bluetooth_puts("EL\n");
                    UA01_StopAttack();
                    SM_Spin(DIRECTION_LEFT, 400);
                    break;
                case STATE_ENEMY_RIGHT:
                    debug_bluetooth_puts("ER\n");
                    UA01_StopAttack();
                    SM_Spin(DIRECTION_RIGHT, 400);
                    break;
                default:
                    debug_bluetooth_puts("NO_ON_STAGE\n");
                    break;
            }
            UP_delay_ms(10);
            continue;
        }

        debug_bluetooth_puts("Under stage\n");
        G4S_enable(DISABLE);
        CS_enable(DISABLE);
        SM_Move(DIRECTION_FORWARD, 0);
        switch (CS_State) {
            case STATE_UNDER_STAGE_FACE_TO_STAGE:
            case STATE_UNDER_STAGE_FACE_NOT_TO_STAGE:
                debug_bluetooth_puts("US\n");
                under_stage_count++;
                if (under_stage_count > UNDER_COUNT) {
                    UA01_PreAction();
                    debug_bluetooth_puts("get on stage\n");
                    under_stage_count = 0;
                    UA01_GetOnStage(UA01_FindStage());
                }
                break;
            case STATE_STUCK_LEFT:
            case STATE_STUCK_RIGHT:
                under_stage_count++;
                if (under_stage_count > UNDER_COUNT) {
                    UA01_PreAction();
                    under_stage_count = 0;
                    UA01_FrontArmDown();
                    UA01_BackArmDown();
                }
                break;
            case STATE_STUCK_FRONT:
                under_stage_count++;
                if (under_stage_count > UNDER_COUNT) {
                    UA01_PreAction();
                    under_stage_count = 0;
                    UA01_GetOnStage(DIRECTION_FORWARD);
                }
                break;
            case STATE_STUCK_BACK:
                under_stage_count++;
                if (under_stage_count > UNDER_COUNT) {
                    UA01_PreAction();
                    under_stage_count = 0;
                    UA01_GetOnStage(DIRECTION_BACK);
                }
                break;
            default:
                debug_bluetooth_puts("NO_UNDER_STAGE\n");
                break;
        }
        UP_delay_ms(100);
    }
}
