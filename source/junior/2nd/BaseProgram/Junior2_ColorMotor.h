#include <stdlib.h>
#include <kernel.h>

#include <spike/hub/system.h>

#include <Junior2_Main.h>

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
pup_motor_t *motorC;             //モータCを使う変数
pup_motor_t *motorD;             //モータDを使う変数
pup_device_t *ColorSensor;       //カラーセンサーを使う変数

#define REF_BLACK 10
#define REF_WHITE 94

void RefSetting()
{
  int ref;
  while (true)
  {
    ref = pup_color_sensor_reflection(ColorSensor);
    hub_display_number(ref);
  }
  
}

int ADJRef(float UnADJref)
{
  int Reref;
  Reref = (UnADJref - REF_BLACK + 5) * 100 / (REF_WHITE - REF_BLACK);
  return Reref;
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
    ref = ADJRef(pup_color_sensor_reflection(ColorSensor));
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

  MotorStop(stop);
}

//hold = 1 no stop = 0 brake = -1
//true ref >=  false ref <= 
void RefMotor(int LeftPower, int RightPower, int Ref, bool BigSmall, int stop)
{
  int ref;

  pup_motor_set_power(motorA, LeftPower);
  pup_motor_set_power(motorB, RightPower);

  while (true)
  {
    ref = ADJRef(pup_color_sensor_reflection(ColorSensor));
    
    if (BigSmall == true)
    {
      if (ref >= Ref)
      {
        break;
      }
    }
    else
    {
      if (ref <= Ref)
      {
        break;
      }
      
    }
    
  }
  
  MotorStop(stop);
}

//black = k  purple = v  blue = b cyan = c  green = g  yellow = y  red = r  white = w
void ColorMotor(int LeftPower, int RightPower, char ColorCode, int stop)
{
  int color;

  pup_motor_set_power(motorA, LeftPower);
  pup_motor_set_power(motorB, RightPower);

  while (true)
  {
    color = pup_color_sensor_color_name(ColorSensor, true);
    if (color == ColorCode)
    {
      break;
    }
  }
  
  MotorStop(stop);
}

void RefSearch()
{
  int ref;
  while (true)
  {
    ref = pup_color_sensor_reflection(ColorSensor);
    hub_display_number(ADJRef(ref));
  }
  
}

//hold = 1 no stop = 0 brake = -1
//true ref >=  false ref <= 
void RefADJMotor(int MotorPower, int Ref, bool BigSmall, int stop, bool reset)
{
  int ref;
  int motorDiff, leftAngle, rightAngle, rightPower;

  if (reset == true)
  {
    pup_motor_reset_count(motorA);
    pup_motor_reset_count(motorB);
  }

  while (true)
  {
    ref = ADJRef(pup_color_sensor_reflection(ColorSensor));
    pup_motor_set_power(motorA, MotorPower);
    pup_motor_set_power(motorB, rightPower);

    leftAngle = pup_motor_get_count(motorA);
    rightAngle = pup_motor_get_count(motorB);
    motorDiff = leftAngle - rightAngle;
    rightPower = MotorPower + (motorDiff * 2.0f);
    
    if (BigSmall == true)
    {
      if (ref >= Ref)
      {
        break;
      }
    }
    else
    {
      if (ref <= Ref)
      {
        break;
      }
      
    }

    dly_tsk(1000);
  }
  
  MotorStop(stop);
}