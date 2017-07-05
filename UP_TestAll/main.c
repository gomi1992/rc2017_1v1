#include "UpLib/UP_System.h"
#include "common.h"
#include "Grayscale4Sensor.h"
#include "ServoMove.h"


#ifdef DEBUG_ON
//then the program is in debug mode
#endif


u8 GLOBAL_SENSOR_LIST[4] = {0, 1, 2, 3};
extern u8 G4S_GrayScaleSensorList[4];

void init() {
    G4S_GrayScaleSensorList[0] = GLOBAL_SENSOR_LIST[0];
    G4S_GrayScaleSensorList[1] = GLOBAL_SENSOR_LIST[1];
    G4S_GrayScaleSensorList[2] = GLOBAL_SENSOR_LIST[2];
    G4S_GrayScaleSensorList[3] = GLOBAL_SENSOR_LIST[3];
    SM_Init();
}

int main(void) {

#ifdef DEBUG_ON
    UP_Bluetooth_EnableIT();
#endif

    UP_Timer_EnableIT(0, 5000);//5ms
    UP_Timer_SetHadler(0, G4S_UpdateGrayScaleSensor);
    UP_System_Init();
    UP_delay_ms(100);

    init();

    while (1) {

    }
}
