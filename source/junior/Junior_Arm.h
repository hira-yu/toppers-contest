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

pup_motor_t *motorA;             //モータAを使う変数
pup_motor_t *motorB;             //モータBを使う変数
pup_motor_t *motorC;
pup_motor_t *motorE;
pup_device_t *ColorSensor;       //カラーセンサーを使う変数
pup_device_t *ForceSensor;       //フォースセンサーを使う変数
pup_device_t *UltraSonicSensor;  //距離センサーを使う変数

//up = -1  down = 1
void MotorCE (int UpDown)
{
    int power;
    power = 70 * UpDown;
    if(UpDown == 1)
    {
        power = 20;
    }
    pup_motor_set_power(motorC, power);
    pup_motor_set_power(motorE, power);
    dly_tsk(600000);
    if (UpDown == 1)
    {
        pup_motor_set_power(motorC, 70);
        dly_tsk(200000);
    }
    pup_motor_hold(motorC);
    pup_motor_hold(motorE);
}

//up = -1  down = 1
void MotorC (int UpDown)
{   
    int power;
    power = 70 * UpDown;
    if(UpDown == 1)
    {
        power = 20;
    }
    pup_motor_set_power(motorC, power);
    dly_tsk(600000);
    if (UpDown == 1)
    {
        pup_motor_set_power(motorC, 70);
        dly_tsk(200000);
    }
    pup_motor_hold(motorC);
}

//up = -1  down = 1
void MotorE (int UpDown)
{
    int power;
    power = 70 * UpDown;
    if(UpDown == 1)
    {
        power = 30;
    } 
    pup_motor_set_power(motorE, power);
    dly_tsk(700000);
    pup_motor_hold(motorE);
}

void Solar (void)
{   
    pup_motor_reset_count(motorE);
    pup_motor_set_power(motorE, 60);
    while (abs(pup_motor_get_count(motorE)) <= 135){}
    pup_motor_hold(motorE);
    AdjMotor(-60, 135, 1, true);
    pup_motor_set_power(motorE, 100);
    AdjMotor(40, 200, 1, true);
    pup_motor_set_power(motorE, -80);
    dly_tsk(700000);
    pup_motor_hold(motorE);
}

void MotorCput(void)
{
    MotorC(1);
    AdjMotor(40, 60, 1, true);
    AdjMotor(-40, 70, 1, true);
    MotorC(-1);    
}

void MotorEput(void)
{
    MotorE(1);
    AdjMotor(40, 60, 1, true);
    AdjMotor(-40, 90, 1, true);
    MotorE(-1);
}

void MotorCEput (void)
{
    MotorCE(1);
    AdjMotor(40, 60, 1, true);
    AdjMotor(-40, 70, 1, true);
    MotorC(-1);
    AdjMotor(-30, 20, 1, true);
    MotorE(-1);
}

void MotorCputP(void)
{
    MotorC(1);
    AdjMotor(40, 100, 1, true);
    AdjMotor(-40, 110, 1, true);
    MotorC(-1);    
}

void MotorEputP(void)
{
    MotorE(1);
    AdjMotor(40, 100, 1, true);
    AdjMotor(-40, 130, 1, true);
    MotorE(-1);
}

void MotorCEputP(void)
{
    MotorCE(1);
    AdjMotor(40, 100, 1, true);
    AdjMotor(-40, 110, 1, true);
    MotorC(-1);
    AdjMotor(-30, 20, 1, true);
    MotorE(-1);
}
