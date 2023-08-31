/*
 * Copyright (c) 2023 Afrel Co., Ltd. 
 * All rights reserved
 */
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

//make header
#include "MoveProgram/Junior2_ObjectPut.h"

pup_motor_t *motorA;             //モータAを使う変数
pup_motor_t *motorB;             //モータBを使う変数
pup_motor_t *motorC;             //モータCを使う変数
pup_motor_t *motorD;             //モータDを使う変数
pup_device_t *ColorSensor;       //カラーセンサーを使う変数


void Main(intptr_t exinf)
{
  hub_imu_init();
  motorA = pup_motor_get_device(PBIO_PORT_ID_A);
  motorB = pup_motor_get_device(PBIO_PORT_ID_B);
  motorC = pup_motor_get_device(PBIO_PORT_ID_C);
  motorD = pup_motor_get_device(PBIO_PORT_ID_D);
  ColorSensor = pup_color_sensor_get_device(PBIO_PORT_ID_E);
  pup_motor_setup(motorA, PUP_DIRECTION_COUNTERCLOCKWISE, true);
  pup_motor_setup(motorB, PUP_DIRECTION_CLOCKWISE, true);
  pup_motor_setup(motorC, PUP_DIRECTION_CLOCKWISE, true);
  pup_motor_setup(motorD, PUP_DIRECTION_COUNTERCLOCKWISE, true);
  
  /*
  while (true)
  {
    hub_display_number(pup_color_sensor_rgb(ColorSensor).r);
  }
  */


  hub_button_t pressed;
  while (!(pressed&HUB_BUTTON_LEFT))
  {
    hub_button_is_pressed(&pressed);
  }

  //ArmLR(0);
  //GyroSetting();
  //GyroTurnSetting();
  //RefSetting();
  //RefSearch();

  SecMotor(-40, 0.4f, -1);
  hub_imu_init();
  dly_tsk(300000);
  tADJMotor(1, 70, 40, 30, 725, 1, true);
  ArmL(1);
  dly_tsk(200000);
  GyroTurn(60, 50, 0 , 1, false);
  dly_tsk(200000);
  ADJMotor(40, 130, 1, true);
  ArmL(0);
  ADJMotor(-40, 100, 1, true);
  dly_tsk(200000);
  GyroTurn(60, -50, 50, 1, false);
  tADJMotor(1, 100, 40, 30, 2300, 0, true);
  GyroTurn(90, -50, 50, 0, false);
  SecMotor(-40, 0.5f, -1);
  hub_imu_init();
  dly_tsk(300000);
  ADJMotor(40, 100, 1, true);
  dly_tsk(200000);
  GyroTurn(90, 40, -40, 1, false);
  dly_tsk(200000);
  ArmR(1);
  ADJMotor(50, 350, 1, true);
  ArmR(0);

  tADJMotor(-1, 100, 40, 40, 900, 0, true);
	RefADJMotor(-40, 15, false, 0, false);
	ADJMotor(-40, abs(pup_motor_get_count(motorA)) + 35, 1, false);
  dly_tsk(200000);
	GyroTurn(95, 0, -50, 1, true);
  dly_tsk(200000);
	ADJMotor(-40, 230, 1, true);
	dly_tsk(1000000);
	ArmRput();
  dly_tsk(200000);
	tADJMotor(-1, 100, 50, 40, 1000, 1, true);
  SecMotor(-40, 0.7f, -1);

  ADJMotor(40, 160, 1, true);
  dly_tsk(200000);
  GyroTurn(90, -40, 40, 1, false);
  dly_tsk(200000);
  ADJMotor(-40, 200, 1, true);
  dly_tsk(200000);
  RefADJMotor(40, 15, false, 0, true);
  ADJMotor(40, 360, 1, true);
  dly_tsk(300000);
	pup_motor_set_power(motorC, -30);
  dly_tsk(600000);
  pup_motor_set_power(motorC, -50);
  dly_tsk(100000);
  ADJMotor(-40, 80, 1, true);
  dly_tsk(100000);
  ArmL(0);

  dly_tsk(200000);
  GyroTurn(94, 0, -50, 1, false);
  dly_tsk(200000);
  ADJMotor(40, 230, 1, true);
  GyroTurn(94, 0, 50, 1, false);
  tADJMotor(1, 80, 40, 60, 970, 0, true);
  GyroTurn(92, 80, 30, 0, false);
  tADJMotor(1, 80, 60, 40, 950, 1, true);
  dly_tsk(200000);
  GyroTurn(30, -60, -30, 0, false);
  GyroTurn(33, -30, -60, 0, false);
  ADJMotor(-100, 600, 0, true);
  SecMotor(-80, 0.6f, -1);


  exit(0);
}
