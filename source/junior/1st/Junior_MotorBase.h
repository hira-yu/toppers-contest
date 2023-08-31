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

//hold = 1 no stop = 0 brake = 2
void MotorBase(int LeftMotor, int RightMotor, int MotorAngle, int hold)
{
    int motorA_dig = 0;
    int motorB_dig = 0;
    int motor_dig =  0;

    pup_motor_reset_count(motorA);
    pup_motor_reset_count(motorB);

    pup_motor_set_power(motorA, LeftMotor);
    pup_motor_set_power(motorB, RightMotor);

    while (motor_dig < MotorAngle)
    {
        motorA_dig = pup_motor_get_count(motorA);
        motorB_dig = pup_motor_get_count(motorB);

        motor_dig = (motorA_dig + motorB_dig) / 2;
    }

    if (hold == 1)
    {
        pup_motor_hold(motorA);
        pup_motor_hold(motorB);
    }
    else if (hold == 0){}
    else
    {
        pup_motor_brake(motorA);
        pup_motor_brake(motorB);
    }
    
}

//hold = 1 no stop = 0 brake = 2
void GyroMotor(int MotorPower, int MotorAngle, int hold)
{
    pup_motor_reset_count(motorA);
    pup_motor_reset_count(motorB);

    float Gx[3];
    float deg;
    float time = 0.1;

    pup_motor_set_power(motorA, MotorPower);
    pup_motor_set_power(motorB, MotorPower);

    hub_imu_init();

    while (true)
    {
        int motorA_dig = pup_motor_get_count(motorA);
        int motorB_dig = pup_motor_get_count(motorB);

        hub_imu_get_angular_velocity(Gx);

        deg += Gx[2] * time + 0.08;

        dly_tsk(time * 1000 * 1000);

        pup_motor_set_power(motorB, MotorPower + -deg * 3);

        if (-motorA_dig >= MotorAngle && MotorPower < 0)
        {
            break;
        }
        else if (motorA_dig >= MotorAngle && MotorPower > 0)
        {
            break;
        }
        
        if (-motorB_dig >= MotorAngle && MotorPower < 0)
        {
            break;
        }
        else if (motorB_dig >= MotorAngle && MotorPower > 0)
        {
            break;
        }        
    }
    
    if (hold == 1)
    {
        pup_motor_hold(motorA);
        pup_motor_hold(motorB);
    }
    else if (hold == 0){}
    else
    {
        pup_motor_brake(motorA);
        pup_motor_brake(motorB);
    }
}

//hold = 1 no stop = 0 brake = 2 
void AdjMotor(int MotorPower, int MotorAngle, int hold, bool reset)
{   
    int motorA_dig = 0;
    int motorB_dig = 0;
    int motor_diff = 0;

    if (reset == true)
    {
        pup_motor_reset_count(motorA);
        pup_motor_reset_count(motorB);
    }
    
    pup_motor_set_power(motorA, MotorPower);
    pup_motor_set_power(motorB, MotorPower);

    #define G 1.8f
    while (true)
    {
        motorA_dig = pup_motor_get_count(motorA);
        motorB_dig = pup_motor_get_count(motorB);
        motor_diff = abs(motorA_dig) - abs(motorB_dig);

        if (MotorPower >= 0)
        {
            if (motor_diff > 0)
            {
                pup_motor_set_power(motorA, MotorPower);
                pup_motor_set_power(motorB, MotorPower + (abs(motor_diff) * G));
            }
            else if (motor_diff < 0)
            {
                pup_motor_set_power(motorA, MotorPower + (abs(motor_diff) * G));
                pup_motor_set_power(motorB, MotorPower);
            }
            else
            {
                pup_motor_set_power(motorA, MotorPower);
                pup_motor_set_power(motorB, MotorPower);
            }
        }
        else
        {
            if (motor_diff > 0)
            {   
                pup_motor_set_power(motorA, MotorPower);
                pup_motor_set_power(motorB, MotorPower + (abs(motor_diff) * G * -1));
            }
            else if (motor_diff < 0)
            {
                pup_motor_set_power(motorA, MotorPower + (abs(motor_diff) * G * -1));
                pup_motor_set_power(motorB, MotorPower);
            }
            else
            {
                pup_motor_set_power(motorA, MotorPower);
                pup_motor_set_power(motorB, MotorPower);
            }
        }
        
        if (abs(motorA_dig) >= MotorAngle && abs(motorB_dig) >= MotorAngle)
        {
            break;
        }

        dly_tsk(1000);
    }

    if (hold == 1)
    {
        pup_motor_hold(motorA);
        pup_motor_hold(motorB);
    }
    else if (hold == 0){}
    else
    {
        pup_motor_brake(motorA);
        pup_motor_brake(motorB);
    }
}


//hold = 1 no stop = 0 brake = -1
//true ref >=  false ref <= 
void ColorMotor(int LeftMotor, int RightMotor, int Target, bool BigSmall, int hold)
{
    pup_motor_set_power(motorA, LeftMotor);
    pup_motor_set_power(motorB, RightMotor);

    int ref;
    
    while (true)
    {   
        ref = pup_color_sensor_reflection(ColorSensor);

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
        
    if (hold == 1)
    {
        pup_motor_hold(motorA);
        pup_motor_hold(motorB);
    }
    else if (hold == 0){}
    else
    {
        pup_motor_brake(motorA);
        pup_motor_brake(motorB);
    }    
}

//hold = 1 no stop = 0 brake = -1
void Turn (int LeftPower, int RightPower, int LeftAngle, int RightAngle, int hold)
{
    int motorA_dig, motorB_dig;

    pup_motor_reset_count(motorA);
    pup_motor_reset_count(motorB);

    pup_motor_set_power(motorA, LeftPower);
    pup_motor_set_power(motorB, RightPower);

    while (true)
    {
        motorA_dig = pup_motor_get_count(motorA);
        motorB_dig = pup_motor_get_count(motorB);

        if (abs(motorA_dig) >= LeftAngle && abs(motorB_dig) >= RightAngle)
        {
            break;
        }        
    }
    
    if (hold == 1)
    {
        pup_motor_hold(motorA);
        pup_motor_hold(motorB);
    }
    else if (hold == 0){}
    else
    {
        pup_motor_brake(motorA);
        pup_motor_brake(motorB);
    }      
}




//hold = 1 no stop = 0 brake = 2 
void AdjGyroMotor(int MotorPower, int MotorAngle, int hold, bool reset)
{   
    int motorA_dig = 0;
    int motorB_dig = 0;
    int motor_diff = 0;

    float Gx[3];
    float dig = 0;
    int i = 0;

    hub_imu_init();

    if (reset == true)
    {
        pup_motor_reset_count(motorA);
        pup_motor_reset_count(motorB);
    }
    
    pup_motor_set_power(motorA, MotorPower);
    pup_motor_set_power(motorB, MotorPower);

    #define G 1.8f
    while (true)
    {   
        i++;
        motorA_dig = pup_motor_get_count(motorA);
        motorB_dig = pup_motor_get_count(motorB);
        
        if(i % 5 == 0)
        {
            hub_imu_get_angular_velocity(Gx);
            dig += Gx[2] * 0.025;
        }
        
        motor_diff = abs(motorA_dig) - abs(motorB_dig);

        if (MotorPower >= 0)
        {
            motor_diff -= dig * 4;
            if (motor_diff > 0)
            {
                pup_motor_set_power(motorA, MotorPower);
                pup_motor_set_power(motorB, MotorPower + (abs(motor_diff) * G));
            }
            else if (motor_diff < 0)
            {
                pup_motor_set_power(motorA, MotorPower + (abs(motor_diff) * G));
                pup_motor_set_power(motorB, MotorPower);
            }
            else
            {
                pup_motor_set_power(motorA, MotorPower);
                pup_motor_set_power(motorB, MotorPower);
            }
        }
        else
        {   
            motor_diff += dig * 4;
            if (motor_diff > 0)
            {   
                pup_motor_set_power(motorA, MotorPower);
                pup_motor_set_power(motorB, MotorPower + (abs(motor_diff) * G * -1));
            }
            else if (motor_diff < 0)
            {
                pup_motor_set_power(motorA, MotorPower + (abs(motor_diff) * G * -1));
                pup_motor_set_power(motorB, MotorPower);
            }
            else
            {
                pup_motor_set_power(motorA, MotorPower);
                pup_motor_set_power(motorB, MotorPower);
            }
        }
        
        if (abs(motorA_dig) >= MotorAngle && abs(motorB_dig) >= MotorAngle)
        {
            break;
        }

        dly_tsk(5000);
    }

    if (hold == 1)
    {
        pup_motor_hold(motorA);
        pup_motor_hold(motorB);
    }
    else if (hold == 0){}
    else
    {
        pup_motor_brake(motorA);
        pup_motor_brake(motorB);
    }
}