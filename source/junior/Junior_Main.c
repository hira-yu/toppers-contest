/*
 * Copyright (c) 2023 Afrel Co., Ltd. 
 * All rights reserved
 */
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

//def files

#include "Junior_ObjectPut.h"

//Move files

pup_motor_t *motorA;             //モータAを使う変数
pup_motor_t *motorB;             //モータBを使う変数
pup_motor_t *motorC;
pup_motor_t *motorE;
pup_device_t *ColorSensor;       //カラーセンサーを使う変数
pup_device_t *ForceSensor;       //フォースセンサーを使う変数
pup_device_t *UltraSonicSensor;  //距離センサーを使う変数

void Main(intptr_t exinf)
{
  hub_imu_init();

  motorA = pup_motor_get_device(PBIO_PORT_ID_A);
  motorB = pup_motor_get_device(PBIO_PORT_ID_B);
  motorC = pup_motor_get_device(PBIO_PORT_ID_C);
  motorE = pup_motor_get_device(PBIO_PORT_ID_E);

  ColorSensor = pup_color_sensor_get_device(PBIO_PORT_ID_D);

  pup_motor_setup(motorA, PUP_DIRECTION_COUNTERCLOCKWISE, true);
  pup_motor_setup(motorB, PUP_DIRECTION_CLOCKWISE, true);
  pup_motor_setup(motorC, PUP_DIRECTION_COUNTERCLOCKWISE, true);
  pup_motor_setup(motorE, PUP_DIRECTION_CLOCKWISE, true);  

  hub_button_t pressed;
  
  while (!(pressed&HUB_BUTTON_LEFT))
  {
    hub_button_is_pressed(&pressed);
  }
  
  MotorC(1);
  
  //startpoint "u"nder" set  
  int CariColorCode[4] = {0, 0, 0, 0};
  int PaciColorCode[4] = {0, 0, 0, 0};
  
  AdjMotor(50, 10, 0, true);
  GyroTurn45(-1);
  GyroTurn(32, 60, 22.5f); 
  dly_tsk(250000);

  pup_motor_set_power(motorA, -60);
  pup_motor_set_power(motorB, -60);
  dly_tsk(600000);
  pup_motor_set_power(motorA, -40);
  pup_motor_set_power(motorB, -40);
  dly_tsk(500000);
  pup_motor_brake(motorA);
  pup_motor_brake(motorB);

  //Carib start
  AdjMotor(40, 295, 0, true);

  int j = 0;
  pup_color_rgb_t rgb;
  int ref[4] = {0,0,0,0};

  while (j < 4)
  {
    AdjMotor(40, 295 + (85 * (j + 1)), 0, false);

    rgb = pup_color_sensor_rgb(ColorSensor);

    if (rgb.r < 15)
    {
      CariColorCode[j] = 1;
    }
    else if (rgb.r > 45)
    {
      ref[j] = pup_color_sensor_reflection(ColorSensor);
      CariColorCode[j] = 2;
    }
    
    j++;
  }  
  
  pup_motor_hold(motorA);
  pup_motor_hold(motorB);

  int whiteref = 0;

  for (int i = 0; i < 4; i++)
  {
    if (ref[i] > whiteref)
    {
      whiteref = ref[i];
    }
  }
  
  for (int i = 0; i < 4; i++)
  {
    if (CariColorCode[i] == 2)
    {
      if (!(ref[i] >= whiteref))
      {
        CariColorCode[i] = 0;
      }
    }
  }  
  //Carib finish

  //Pacific start
  AdjMotor(40, 635 + 175, 0, false);

  j = 0;

  for (int i = 0; i < 4; i++)
  {
    ref[i] = 0;
  }

  while (j < 4)
  {
    AdjMotor(40, 810 + (85 * (j + 1)), 0, false);

    rgb = pup_color_sensor_rgb(ColorSensor);

    if (rgb.g < 15)
    {
      PaciColorCode[j] = 1;
    }
    else if (rgb.g > 45)
    {
      ref[j] = pup_color_sensor_reflection(ColorSensor);
      PaciColorCode[j] = 2;
    }
    
    j++;
  }  
  
  pup_motor_hold(motorA);
  pup_motor_hold(motorB);

  whiteref = 0;

  for (int i = 0; i < 4; i++)
  {
    if (ref[i] > whiteref)
    {
      whiteref = ref[i];
    }
  }
  
  for (int i = 0; i < 4; i++)
  {
    if (PaciColorCode[i] == 2)
    {
      if (!(ref[i] >= whiteref))
      {
        PaciColorCode[i] = 0;
      }
    }
  }
  //pacific finish  
  
  MotorC(-1);
  dly_tsk(100000);
  ColorMotor(40, 10, 50, true, 0);
  AngleLinetrace(30, 50, 6, -1, 150, 1);
  dly_tsk(100000);

  AdjMotor(-50, 220, 1, true);

  dly_tsk(200000);

  AngleLinetrace(30, 40, 6, -1, 125, 1);

  pup_motor_set_power(motorC, 70);
  pup_motor_set_power(motorE, 70);
  dly_tsk(500000);
  pup_motor_hold(motorC);
  pup_motor_hold(motorE);
  
  AdjMotor(50, 340, 0, true);
  dly_tsk(300000);
  pup_motor_hold(motorA);
  pup_motor_hold(motorB);

  AdjMotor(-30, 20, 1, true);

  MotorCE(-1);
  //catch fin
  
  AdjMotor(-50, 50, 1, true);
  MotorCE(-1);
  Turn(-60, -50, 100, 0, 0);
  AdjMotor(-90, 930, 0, true);
  AdjMotor(-50, 50, 1, true);
  
  GyroTurn(40, -40, 72);
  ColorMotor(-40, -40, 12, false, 0);
  AdjMotor(90, 1270, 0, true);
  AdjMotor(40, 50, 1, false);
  dly_tsk(300000);
  GyroTurn(-40, 40, 70);
  pup_motor_set_power(motorA, -70);
  pup_motor_set_power(motorB, -70);
  dly_tsk(900000);
  pup_motor_set_power(motorA, -40);
  pup_motor_set_power(motorB, -40);
  dly_tsk(300000);
  pup_motor_brake(motorA);
  pup_motor_brake(motorB); 
  AdjMotor(60, 260, 0, true);
  AdjMotor(40, 50, 1, false);
  dly_tsk(100000);
  GyroTurn(40, -40, 74);
  dly_tsk(300000);
  Solar();
  
  AdjMotor(70, 1210, 0, false);
  ColorMotor(50, 50, 12, false, 0);
  AdjMotor(50, 100, 1, true);
  dly_tsk(300000);
  GyroTurn(-40, 40, 70);
  pup_motor_set_power(motorA, -60);
  pup_motor_set_power(motorB, -60);
  dly_tsk(400000);
  pup_motor_set_power(motorA, -40);
  pup_motor_set_power(motorB, -40);
  dly_tsk(300000);
  pup_motor_brake(motorA);
  pup_motor_brake(motorB);    
  
  int whitepoint;
  int blackpoint;
  
  for (int i = 0; i < 4; i++)
  {
    if (CariColorCode[i] == 2)
    {
      whitepoint = i;
    }

    if (CariColorCode[i] == 1)
    {
      blackpoint = i;
    }
  }  
  
  Objectput(whitepoint, blackpoint);
  
  AdjMotor(50, 200, 1, true);
  dly_tsk(300000);
  GyroTurn(40, -40, 76);
  AdjMotor(70, 350, 0, true);
  ColorMotor(50, 50, 12, false, 0);
  AdjMotor(50, 180, 1, true);
  GyroTurn(-40, 40, 74);
  pup_motor_set_power(motorA, -50);
  pup_motor_set_power(motorB, -50);
  dly_tsk(600000);
  pup_motor_set_power(motorA, -40);
  pup_motor_set_power(motorB, -40);
  dly_tsk(400000);
  pup_motor_brake(motorA);
  pup_motor_brake(motorB);
  
  AdjMotor(50, 80, 0, true);
  AngleLinetrace(30, 40, 4, 1, 200, 0);
  AngleLinetrace(70, 40, 2, 1, 400, 0);
  AngleLinetrace(30, 40, 4, 1, 200, 1);
  AdjMotor(-30, 90, 1, true);
  pup_motor_set_power(motorC, 20);
  dly_tsk(600000);
  pup_motor_set_power(motorC, 70);
  dly_tsk(200000);
  pup_motor_hold(motorC);
  GyroTurn(50, 0, 55);
  MotorC(-1);
  GyroTurn(-60, 0, 51);
  AdjMotor(70, 300, 0, true);
  ColorMotor(70, 70, 12, false, 0);
  AdjMotor(50, 80, 0, true);
  AngleLinetrace(30, 40, 3, 1, 300, 1);
  AdjMotor(-30, 90, 1, true);
  pup_motor_set_power(motorC, 20);
  dly_tsk(600000);
  pup_motor_set_power(motorC, 70);
  dly_tsk(200000);
  pup_motor_hold(motorC);
  GyroTurn(50, 0, 55);
  MotorC(-1);
  GyroTurn(-60, 0, 53);
  AngleLinetrace(30, 40, 3, 1, 100, 0);
  AngleLinetrace(70, 40, 2, 1, 300, 0);
  AngleLinetrace(30, 40, 3, 1, 200, 1);
  pup_motor_set_power(motorC, 20);
  dly_tsk(600000);
  pup_motor_set_power(motorC, 70);
  dly_tsk(200000);
  pup_motor_hold(motorC);
  GyroTurn(70, 0, 146);
  AdjMotor(70, 920, 0, true);
  AdjMotor(40, 50, 1, true);
  GyroTurn(60, 30, 30);
  MotorC(-1);
  AdjMotor(70, 400, 0, true);
  GyroTurn(30, 60, 28);
  AdjMotor(70, 280, 0, true);
  GyroTurn(70, 0, 74);
  AdjMotor(90, 850, 1, true);
  dly_tsk(500000);
  MotorCput();
  
  AdjMotor(-70, 400, 1, true);
  dly_tsk(300000);
  GyroTurn(-50, 50, 146);
  dly_tsk(300000);
  AngleLinetrace(30, 30, 4, -1, 200, 0);
  pup_motor_set_power(motorA, 50);
  pup_motor_set_power(motorB, 50);
  while (!(pup_color_sensor_color_name(ColorSensor, true) == 'c' || pup_color_sensor_color_name(ColorSensor, true) == 'b')){}
  AdjMotor(50, 70, 1, true);
  dly_tsk(300000);
  GyroTurn(0, 60, 71);
  AdjMotor(40, 40, 1, true);
  Solar();

  AdjMotor(80, 1050, 0, true);
  ColorMotor(40, 40, 12, false, 1);
  AdjMotor(40, 110, 1, true);
  dly_tsk(300000);
  GyroTurn(-50, 50, 70);
  pup_motor_set_power(motorA, -70);
  pup_motor_set_power(motorB, -70);
  dly_tsk(800000);
  pup_motor_set_power(motorA, -40);
  pup_motor_set_power(motorB, -40);
  dly_tsk(300000);
  
  for (int i = 0; i < 4; i++)
  {
    if (PaciColorCode[i] == 2)
    {
      whitepoint = i;
    }

    if (PaciColorCode[i] == 1)
    {
      blackpoint = i;
    }
  }
  
  Pacificput(whitepoint, 3);
  
  Turn(65, 90, 0, 1200 , 0);
  AdjMotor(90, 50, 0, true);
  dly_tsk(700000);
  pup_motor_hold(motorA);
  pup_motor_hold(motorB);
  
  exit(0);
}
