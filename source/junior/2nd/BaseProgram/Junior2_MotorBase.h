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

#define ADJMOTOR_GAIN 2.0f

//hold = 1 no stop = 0 brake = -1
void MotorStop(int stop)
{
  if (stop == 1)
  {
    pup_motor_hold(motorA);
    pup_motor_hold(motorB);
    pup_motor_set_power(motorA, 0);
    pup_motor_set_power(motorB, 0);
  }
  else if (stop == 0){}
  else
  {
    pup_motor_brake(motorA);
    pup_motor_brake(motorB);
    pup_motor_set_power(motorA, 0);
    pup_motor_set_power(motorB, 0);
  }
}

//hold = 1 no stop = 0 brake = -1
void ADJMotor(int MotorPower, int MotorAngle, int stop, bool reset)
{
  int motorDiff, leftAngle, rightAngle, rightPower;
  rightPower = MotorPower;
  if (reset == true)
  {
    pup_motor_reset_count(motorA);
    pup_motor_reset_count(motorB);
  }
  
  while (true)
  { 
    pup_motor_set_power(motorA, MotorPower);
    pup_motor_set_power(motorB, rightPower);

    leftAngle = pup_motor_get_count(motorA);
    rightAngle = pup_motor_get_count(motorB);
    motorDiff = leftAngle - rightAngle;
    rightPower = MotorPower + (motorDiff * ADJMOTOR_GAIN);

    if (abs(leftAngle) >= MotorAngle && abs(rightAngle) >= MotorAngle)
    {
      break;
    }
    
    dly_tsk(1000);
  }
  
  MotorStop(stop);
}

//hold = 1 no stop = 0 brake = -1
void TraMotor(int TargetPower, int FinPower, int MotorAngle, int stop)
{
  float accel = 0.2f;
  int startPower, finPower, accelTime, accelAngle, brakeTime, brakeAngle, startAngle, moveAngle;
  finPower = FinPower;

  startPower = pup_motor_get_power(motorA);
  startAngle = pup_motor_get_count(motorA);
  accelTime = (TargetPower - startPower) / accel;
  brakeTime = (TargetPower - finPower) / accel;

  for (int i = 0; i < accelTime; i++)
  {
    pup_motor_set_power(motorA, startPower + (accel * i));
    pup_motor_set_power(motorB, startPower + (accel * i));
    dly_tsk(1000);
  }

  accelAngle = abs(pup_motor_get_count(motorA) - startAngle);
  brakeAngle = accelAngle * ((TargetPower + finPower) * brakeTime) / ((TargetPower + startPower) * accelTime);
  moveAngle = MotorAngle - (brakeAngle * 1.1f);

  while (!(abs(pup_motor_get_count(motorA)) >= moveAngle)){}
  
  for (int i = 0; i < brakeTime; i++)
  {
    pup_motor_set_power(motorA, TargetPower - (accel * i));
    pup_motor_set_power(motorB, TargetPower - (accel * i));
    dly_tsk(1000);
  }

  while (!(abs(pup_motor_get_count(motorA)) >= MotorAngle && abs(pup_motor_get_count(motorB)) >= MotorAngle)){}

  MotorStop(stop);
}

//hold = 1 no stop = 0 brake = -1
void tADJMotor(int FrontBack, int MotorPower, int StartPower, int FinPower, int MotorAngle, int stop, bool reset)
{
  float accel = 0.2f;
  int startPower, finPower, accelTime, accelAngle, brakeTime, brakeAngle, startAngle, moveAngle, basePower;
  int motorDiff, leftAngle, rightAngle, rightPower;
  finPower = FinPower;

  if (reset == true)
  {
    pup_motor_reset_count(motorA);
    pup_motor_reset_count(motorB);
  }

  startPower = StartPower;
  if (startPower < 20)
  {
    startPower = 20;
  }
  startAngle = pup_motor_get_count(motorA);
  accelTime = (MotorPower - startPower) / accel;
  brakeTime = (MotorPower - finPower) * 0.9f / accel;

  for (int i = 0; i < accelTime; i++)
  {
    basePower = (startPower + (accel * i)) * FrontBack;

    leftAngle = pup_motor_get_count(motorA);
    rightAngle = pup_motor_get_count(motorB);
    motorDiff = leftAngle - rightAngle;
    rightPower = basePower + (motorDiff * ADJMOTOR_GAIN);

    pup_motor_set_power(motorA, basePower);
    pup_motor_set_power(motorB, rightPower);

    dly_tsk(1000);
  }

  accelAngle = abs(pup_motor_get_count(motorA) - startAngle);
  brakeAngle = accelAngle * ((MotorPower + finPower) * brakeTime) / ((MotorPower + startPower) * accelTime);
  moveAngle = MotorAngle - brakeAngle - 50;

  ADJMotor(MotorPower * FrontBack, moveAngle, 0, false);

  for (int i = 0; i < brakeTime; i++)
  {
    basePower = (MotorPower - (accel * i)) * FrontBack;

    leftAngle = pup_motor_get_count(motorA);
    rightAngle = pup_motor_get_count(motorB);
    motorDiff = leftAngle - rightAngle;
    rightPower = basePower + (motorDiff * ADJMOTOR_GAIN);

    pup_motor_set_power(motorA, basePower);
    pup_motor_set_power(motorB, rightPower);

    dly_tsk(1000);
  }

  ADJMotor(finPower * FrontBack, MotorAngle, stop, false);
}

//hold = 1 no stop = 0 brake = -1
void Turn(int LeftPower, int RightPower, int LeftAngle, int RightAngle, int stop)
{
  pup_motor_reset_count(motorA);
  pup_motor_reset_count(motorB);

  pup_motor_set_power(motorA, LeftPower);
  pup_motor_set_power(motorB, RightPower);

  while (true)
  {
    if (abs(pup_motor_get_count(motorA)) >= LeftAngle && abs(pup_motor_get_count(motorB)) >= RightAngle)
    {
      break;
    }
  }
  
  MotorStop(stop);
}

//hold = 1 no stop = 0 brake = -1
void SecMotor(int MotorPower, float Sec, int stop)
{
  pup_motor_set_power(motorA, MotorPower);
  pup_motor_set_power(motorB, MotorPower);

  dly_tsk(Sec * 1000000);

  MotorStop(stop);
}