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

//0 = Up   1 = drop 2 pieces
void ArmL(int Updown)
{
  if (Updown == 0)
  {
    pup_motor_set_power(motorC, 60);
    dly_tsk(500000);
    pup_motor_hold(motorC);
    pup_motor_set_power(motorC, 0);
  }
  else
  {
    pup_motor_set_power(motorC, -30);
    dly_tsk(600000);
    pup_motor_set_power(motorC, -50);
    dly_tsk(100000);
    pup_motor_hold(motorC);
    pup_motor_set_power(motorC, 0);
  }
}

//0 = Up   1 = drop 2 pieces
void ArmR(int Updown)
{
  if (Updown == 0)
  {
    pup_motor_set_power(motorD, 60);
    dly_tsk(500000);
    pup_motor_hold(motorD);
    pup_motor_set_power(motorD, 0);
  }
  else
  {
    pup_motor_set_power(motorD, -30);
    dly_tsk(600000);
    pup_motor_hold(motorD);
    pup_motor_set_power(motorD, 0);
  }
}

//0 = Up   1 = drop 2 pieces
void ArmLR(int Updown)
{
  if (Updown == 0)
  {
    pup_motor_set_power(motorC, 60);
    pup_motor_set_power(motorD, 60);
    dly_tsk(500000);
    pup_motor_hold(motorC);
    pup_motor_hold(motorD);
    pup_motor_set_power(motorC, 0);
    pup_motor_set_power(motorD, 0);
  }
  else
  {
    pup_motor_set_power(motorC, -30);
    pup_motor_set_power(motorD, -30);
    dly_tsk(600000);
    pup_motor_hold(motorD);
    pup_motor_set_power(motorD, 0);
    pup_motor_set_power(motorC, -50);
    dly_tsk(100000);
    pup_motor_hold(motorC);
    pup_motor_set_power(motorC, 0);
  }
}

void ArmLRput()
{
  dly_tsk(200000);
  pup_motor_reset_count(motorC);
  while (abs(pup_motor_get_count(motorC)) <= 30)
  {
    pup_motor_set_power(motorC, -30);
  }
  pup_motor_hold(motorC);
  dly_tsk(200000);
  pup_motor_set_power(motorC, 50);
  dly_tsk(300000);
  pup_motor_set_power(motorC, -30);
  pup_motor_set_power(motorD, -30);
  dly_tsk(600000);
  ADJMotor(-30, 65, 0, true);
  pup_motor_set_power(motorD, 70);
  ADJMotor(-30, 90, 1, false);
  pup_motor_set_power(motorC, 70);
  dly_tsk(400000);
  pup_motor_hold(motorC);
  pup_motor_hold(motorD);
  pup_motor_set_power(motorC, 0);
  pup_motor_set_power(motorD, 0);
}

void ArmLput()
{
  dly_tsk(200000);
  pup_motor_reset_count(motorC);
  pup_motor_set_power(motorC, -30);
  while (abs(pup_motor_get_count(motorC)) <= 30){}
  pup_motor_set_power(motorC, 0);
  pup_motor_hold(motorC);
  dly_tsk(200000);
  pup_motor_set_power(motorC, 50);
  dly_tsk(400000);
  pup_motor_set_power(motorC, -30);
  dly_tsk(600000);
  pup_motor_set_power(motorC, -50);
  dly_tsk(100000);
  ADJMotor(-40, 80, 1, true);
  dly_tsk(100000);
  ArmL(0);
}

void ArmRput()
{
  dly_tsk(500000);
  pup_motor_set_power(motorD, -40);
  dly_tsk(400000);
  ADJMotor(-40, 65, 1, true);
  dly_tsk(200000);
  ArmR(0);
}