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

//LeftSide = 1   RightSide = -1
//Target = stop point 
//true ref>=   false ref<=
//hold = 1 no stop = 0 brake = -1
void RefLinetrace (int MotorPower, int Ref, float Gain, int Side, int Target, bool BigSmall, int stop)
{
    int ref;
    int beforeRef;
    int diff;

    while (true)
    {
        ref = pup_color_sensor_reflection(ColorSensor);
        diff = ((Ref - ref) * Gain + (((Ref - ref) - beforeRef) ^ 2)) * Side; 
        beforeRef = Ref - ref;
        pup_motor_set_power(motorA, MotorPower - diff / 2);
        pup_motor_set_power(motorB, MotorPower + diff / 2);

        if (BigSmall == true)
        {
            if (ref >= Target)
            {
                break;
            }
        }
        else
        {
            if (ref <= Target)
            {
                break;
            }  
        }        
    }
    
    if (stop == 1)
    {
        pup_motor_hold(motorA);
        pup_motor_hold(motorB);
    }
    else if (stop == 0){}
    else
    {
        pup_motor_brake(motorA);
        pup_motor_brake(motorB);
    }
}

//hold = 1 no stop = 0 brake = -1
//LeftSide = 1   RightSide = -1
void AngleLinetrace (int MotorPower, int Ref, float Gain, int Side, int MotorAngle, int stop)
{
    int ref;
    int beforeRef;
    int diff;
    int motorDig_A, motorDig_B;
    
    pup_motor_reset_count(motorA);
    pup_motor_reset_count(motorB);

    while (true)
    {
        ref = pup_color_sensor_reflection(ColorSensor);
        diff = ((Ref - ref) * Gain + (((Ref - ref) - beforeRef) ^ 2)) * Side; 
        beforeRef = Ref - ref;
        pup_motor_set_power(motorA, MotorPower - (diff / 2));
        pup_motor_set_power(motorB, MotorPower + (diff / 2));

        motorDig_A = pup_motor_get_count(motorA);
        motorDig_B = pup_motor_get_count(motorB);
        if (abs(motorDig_A) >= MotorAngle && abs(motorDig_B) >= MotorAngle)
        {
            break;
        }
    }

    if (stop == 1)
    {
        pup_motor_hold(motorA);
        pup_motor_hold(motorB);
    }
    else if (stop == 0){}
    else
    {
        pup_motor_brake(motorA);
        pup_motor_brake(motorB);
    }
}