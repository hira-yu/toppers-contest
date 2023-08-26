#include <stdlib.h>
#include <kernel.h>

#include <spike/hub/system.h>

#include <Junior_Main.h>

#include "spike/pup/motor.h"
#include "spike/pup/colorsensor.h"
#include "spike/pup/forcesensor.h"
#include "spike/pup/ultrasonicsensor.h"

#include "spike/hub/battery.h"
#include "spike/hub/button.h"
#include "spike/hub/display.h"
#include "spike/hub/imu.h"
#include "spike/hub/light.h"

#include <pbio/color.h>

#include <stdlib.h>

pup_motor_t *motorA;             //モータAを使う変数
pup_motor_t *motorB;             //モータBを使う変数
pup_device_t *ColorSensor;       //カラーセンサーを使う変数
pup_device_t *ForceSensor;       //フォースセンサーを使う変数
pup_device_t *UltraSonicSensor;  //距離センサーを使う変数

//Left turn 1 Right turn -1
void GyroTurn90(int direction)
{
    float Gx[3];
    float deg;
    float time = 0.01;

    hub_imu_init();
    dly_tsk(500000);

    pup_motor_set_power(motorA, -40 * direction);
    pup_motor_set_power(motorB, 40 * direction);

    while (abs(deg) < 82)
    {
        hub_imu_get_angular_velocity(Gx);
        deg += Gx[2] * time;
        dly_tsk(time*1000*1000);
    }

    pup_motor_hold(motorA);
    pup_motor_hold(motorB);
}

//Left turn 1 Right turn -1
void GyroTurn45(int direction)
{
    hub_imu_init();
    pup_motor_set_power(motorA, -40 * direction);
    pup_motor_set_power(motorB, 40 * direction);

    float Gx[3];
    float deg;
    float time = 0.01;

    while (abs(deg) < 25)
    {
        hub_imu_get_angular_velocity(Gx);
        deg += Gx[2] * time;
        dly_tsk(time*1000*1000);
    }

    pup_motor_hold(motorA);
    pup_motor_hold(motorB);   
}


void GyroTurn(int LeftPower, int RightPower, float angle)
{
    hub_imu_init();
    pup_motor_set_power(motorA, LeftPower);
    pup_motor_set_power(motorB, RightPower);

    float Gx[3];
    float deg;
    float time = 0.005f;

    while (abs(deg) < angle)
    {
        hub_imu_get_angular_velocity(Gx);
        deg += Gx[2] * time;
        dly_tsk(time*1000*1000);
    }
    
    pup_motor_hold(motorA);
    pup_motor_hold(motorB);    
}