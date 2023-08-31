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

void PutMove(int WhitePoint, int BlackPoint, int StandardAngle, int *pattern, int *moveAngle, bool Carib)
{
  int addAngle = 0;
  if (Carib == true)
  {
    addAngle = 100;
  }
  
  if (WhitePoint == 0 && BlackPoint == 2)
  {
    *pattern = 1;
    *moveAngle = StandardAngle + 300 + addAngle;
  }
  else if (WhitePoint == 3 && BlackPoint == 2)
  {
    *pattern = 1;
    *moveAngle = StandardAngle + 300 + addAngle;
  }
  else if (WhitePoint == 0 && BlackPoint == 3)
  {
    *pattern = 2;
    *moveAngle = StandardAngle + 400 + addAngle;
  }
  else if (WhitePoint == 2 && BlackPoint == 3)
  {
    *pattern = 3;
    *moveAngle = StandardAngle + 300 + addAngle;
  }
  else if (BlackPoint == 1)
  {
    *pattern = 4;
    *moveAngle = StandardAngle + addAngle;
  }
  else
  {
    *pattern = 0;
    *moveAngle = StandardAngle;
  }
}