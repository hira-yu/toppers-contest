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

#define GYRO_ADJ 0.00032f
#define GYRO_TIME 0.001f
#define GYROTURN_ADJ 42.0f

void GyroSetting()
{
  float Gx[3];
  float deg;

  hub_imu_init();
  dly_tsk(300000);
  while (true)
  {
    dly_tsk(GYRO_TIME * 1000000);    
    hub_imu_get_angular_velocity(Gx);
    deg += (Gx[2] * GYRO_TIME) - GYRO_ADJ;
    hub_display_number(deg * 10);
  }
  
}

void GyroTurnSetting()
{
  float Gx[3];
  float deg;

  hub_imu_init();
  dly_tsk(300000);
  pup_motor_set_power(motorA, 40);
  pup_motor_set_power(motorB, -40);
  while (abs(deg) <= GYROTURN_ADJ)
  {
    dly_tsk(GYRO_TIME * 1000000);
    hub_imu_get_angular_velocity(Gx);
    deg += (Gx[2] * GYRO_TIME) - GYRO_ADJ;
  
  }
  
  MotorStop(1);
  dly_tsk(300000);
  exit(0);
}

void GyroTurn180(int LeftPower, int RightPower)
{
  float Gx[3];
  float deg;

  hub_imu_init();
  dly_tsk(300000);
  pup_motor_set_power(motorA, LeftPower);
  pup_motor_set_power(motorB, RightPower);
  while (abs(deg) <= 91)
  {
    dly_tsk(GYRO_TIME * 1000000);
    hub_imu_get_angular_velocity(Gx);
    deg += (Gx[2] * GYRO_TIME) - GYRO_ADJ;
  
  }
  pup_motor_hold(motorA);
  pup_motor_hold(motorB);
}

//hold = 1 no stop = 0 brake = -1
void GyroTurn(float Deg, int LeftPower, int RightPower, int stop, int reset)
{
  float Gx[3];
  float deg;

  if (reset == true)
  {
    hub_imu_init();
    dly_tsk(300000);
  }
  pup_motor_set_power(motorA, LeftPower);
  pup_motor_set_power(motorB, RightPower);
  while (abs(deg) <= Deg * GYROTURN_ADJ / 90)
  {
    dly_tsk(GYRO_TIME * 1000000);
    hub_imu_get_angular_velocity(Gx);
    deg += (Gx[2] * GYRO_TIME) - GYRO_ADJ;
  
  }
  
  MotorStop(stop);
}