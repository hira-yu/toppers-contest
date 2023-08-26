/*
 * Copyright (c) 2023 Afrel Co., Ltd. 
 * All rights reserved
 */
#include <stdlib.h>
#include <kernel.h>

#include <spike/hub/system.h>

#include <Senior_Main.h>

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

#include "Senior_Lib.h"

void Main(intptr_t exinf) {

  //左ボタンが押されるまで待機
  while(!(pressed & HUB_BUTTON_LEFT)) hub_button_is_pressed(&pressed);

  //pupデバイスセットアップ
  pup_device_setup();  

  //大型船を押す
  tank(-80, -80, 1.72, 1.72, false, false, 'n');
  tank(-30, -30, 0.6, 0.6, false, false, 'n');
  dlyms(50);

  //反転して大型船の横に
  gyro(182, 25, 90);
  tank(80, 80, 1.12, 1.12, false, false, 'n');
  gyro(92.5, -60, 60);
  arm(80, 280);
  
  //大型船へアプローチ
  tank(60, 60, 0.7, 0.7, false, false, 'n');
  dlyms(50);

  //大型船を持つ
  arm(-80, 470);

  //本線へ向かう
  gyro(106, -60, 60);
  tank(60, 60, 2, 2, true, true, 'w');
  tank(40, 40, 1, 1, true, true, 'k');
  tank(40, 40, 0.24, 0.24, false, false, 'n');
  gyro(106, -60, 60);
  
  //ライントレースで外洋へ向かう
  linetrace(30, 2, 0.8, 'w', false);
  linetrace(40, 1.2, 0.8, 'n', true);
  linetrace(60, 1.2, 0.5, 'n', true);
  linetrace(80, 5.5, 0.275, 'n', true);
  linetrace(60, 4, 0.25, 'k', false);
  tank(40, 40, 0.12, 0.12, false, false, 'n');
  gyro(-92, 60, -60);

  //ライントレースでクレーンへ向かい赤コンテナを降ろす
  linetrace(30, 2, 0.6, 'w', false);
  linetrace(40, 1, 0.6, 'n', true);
  linetrace(60, 1, 0.4, 'n', true);

  //後ろに下がって左を向いて大型船を置く
  tank(-60, -60, 1.31, 1.31, false, false, 'n');
  gyro(98, -60, 60);
  tank(40, 40, 0.14, 0.14, false, false, 'n');
  arm(80, 280);
  tank(-60, -60, 0.3, 0.3, false, false, 'n');

  //反転
  gyro(166, -60, 60);

  //ライントレースで白コンテナへ向かう
  linetrace(30, 2, 0.8, 'w', false);
  linetrace(40, 0.6, 0.8, 'k', true);
  linetrace(50, 0.6, 0.6, 'k', true);
  linetrace(60, 5, 0.4, 'k', false);
  tank(25, 25, 0.1, 0.1, false, false, 'n');
  gyro(-90, 60, -60);

  //ライントレースで本線から白コンテナ置き場へ向かう
  linetrace(30, 2, 0.6, 'w', false);
  linetrace(40, 0.8, 0.72, 'k', true);
  linetrace(40, 0.35, 0.3, 'k', true);

  //コンテナへアプローチ
  gyro(-60, 60, 0);
  tank(40, 40, 0.16, 0.16, false, false, 'n');
  gyro(63.5, -60, 60);
  tank(30, 30, 0.2, 0.2, false, false, 'n');
  tank(40, 40, 1, 1, true, false, 'c');
  tank(-25, -25, 0.06, 0.06, false, false, 'n');
  
  //コンテナ持ち上げ
  arm(-40, 90);
  arm(-80, 400);
  arm(30, 145);
  arm(-80, 22);
  dlyms(50);

  //バックで本線に戻る
  tank(-60, -60, 1.7, 1.7, false, false, 'n');
  tank(-60, -60, 3, 3, true, true, 'w');
  tank(-60, -60, 3, 3, true, true, 'k');
  tank(25, 25, 0.1, 0.1, false, false, 'n');
  gyro(-88, 60, -60);

  //ライントレースで大型船へ向かう
  linetrace(30, 2, 0.6, 'w', false);
  linetrace(40, 1, 0.5, 'k', true);
  linetrace(60, 2, 0.5, 'k', true);
  linetrace(60, 4, 0.4, 'k', false);
  dlyms(50);
  tank(-40, -40, 0.03, 0.03, false, false, 'n');

  //コンテナを大型船に置く
  pup_motor_set_power(motor_C, -80);
  dlyms(200);
  tank(30, 30, 0.5, 0.5, false, false, 'n');
  arm(40, 260);
  arm(80, 10);
  tank(-60, -60, 0.4, 0.4, false, false, 'n');
  
  //反転
  gyro(170, -60, 60);
  gyro(-40, 60, 0);
  gyro(40, 0, 60);

  //ライントレースで2種コンテナへ向かう
  linetrace(30, 2, 0.8, 'w', false);
  linetrace(40, 1, 0.8, 'k', true);
  linetrace(40, 0.5, 0.6, 'k', true);
  linetrace(60, 0.5, 0.4, 'k', true);
  linetrace(80, 3, 0.3, 'k', true);
  linetrace(60, 5, 0.4, 'k', false);
  tank(25, 25, 0.1, 0.1, false, false, 'n');
  gyro(-88, 60, -60);

  //ライントレースで本線から2種コンテナ置き場へ向かう
  linetrace(30, 2, 0.6, 'w', false);
  linetrace(40, 0.6, 0.72, 'k', true);
  linetrace(40, 0.4, 0.3, 'k', true);

  //コンテナへアプローチ
  gyro(-56, 50, 0);
  tank(40, 40, 0.2, 0.2, false, false, 'n');
  gyro(56, 0, 50);
  arm(60, 140);
  tank(30, 30, 0.2, 0.2, false, false, 'n');
  tank(40, 40, 1, 1, true, false, 'c');
  tank(-25, -25, 0.09, 0.09, false, false, 'n');

  //コンテナ持ち上げ
  arm(-38, 400);
  arm(60, 120);
  tank(40, 40, 0.2, 0.2, false, false, 'n');
  tank(-25, -25, 0.09, 0.09, false, false, 'n');
  arm(-40, 400);
  arm(-80, 22);
  dlyms(100);

  //反転してラインへ向かう
  tank(-50, -50, 0.4, 0.4, false, false, 'n');
  gyro(104, -60, 60);
  tank(40, 40, 1, 1, true, true, 'k');
  tank(25, 25, 0.28, 0.28, false, false, 'n');
  gyro(96, -60, 60);
  
  //ライントレースで小型船へ向かう
  linetrace(30, 2, 0.6, 'w', false);
  linetrace(40, 0.8, 0.6, 'n', true);
  linetrace(60, 2, 0.4, 'k', false);
  linetrace(40, 1, 0.3, 'k', true);
  
  //コンテナを小型船に置く
  pup_motor_set_power(motor_C, -80);
  dlyms(200);
  tank(40, 40, 0.4, 0.4, false, false, 'n');
  arm(40, 200);
  arm(80, 10);
  tank(-60, -60, 0.5, 0.5, false, false, 'n');
  arm(-40, 5);

  //小型船へアプローチ
  linetrace2(40, 0.6, 0.8, 'n', true);
  linetrace2(40, 2, 0.4, 'k', false);
  dlyms(50);

  //小型船を持つ
  arm(-80, 470);

  //反転して本線へ向かう
  gyro(174, -40, 40);
  linetrace(30, 2, 0.8, 'w', false);
  linetrace(40, 1, 0.6, 'n', true);
  linetrace(40, 2, 0.6, 'k', false);
  tank(25, 25, 0.2, 0.2, false, false, 'n');
  gyro(-92, 40, -40);

  //ライントレースで外洋へ向かう
  linetrace(30, 2, 0.8, 'w', false);
  linetrace(40, 1, 0.6, 'k', true);
  linetrace(60, 6, 0.4, 'k', false);
  linetrace(80, 2.8, 0.3, 'n', true);

  //右に曲がり小型船を置く
  gyro(-100, 40, -40);
  tank(60, 60, 0.2, 0.2, false, false, 'n');
  arm(80, 300);
  tank(-60, -60, 1, 1, true, true, 'w');
  tank(-40, -40, 1, 1, true, true, 'k');
  tank(25, 25, 0.04, 0.04, false, false, 'n');
  gyro(-90, 60, -60);

  //ライントレースで2種コンテナへ向かう
  linetrace(30, 2, 0.8, 'w', false);
  linetrace(40, 1, 0.6, 'k', true);
  linetrace(60, 1, 0.4, 'k', true);
  linetrace(80, 2, 0.3, 'k', true);
  linetrace(60, 5, 0.5, 'k', false);
  tank(25, 25, 0.1, 0.1, false, false, 'n');
  gyro(-80, 60, -60);

  //ライントレースで本線から2種コンテナ置き場へ向かう
  linetrace(30, 2, 0.6, 'w', false);
  linetrace(40, 0.6, 0.72, 'k', true);
  linetrace(40, 0.6, 0.3, 'k', true);

  //コンテナへアプローチ
  gyro(70, -60, 60);
  tank(40, 40, 0.33, 0.33, false, false, 'n');
  gyro(-62, 60, 0);
  arm(60, 140);
  tank(30, 30, 0.2, 0.2, false, false, 'n');
  tank(40, 40, 1, 1, true, false, 'c');
  tank(-25, -25, 0.09, 0.09, false, false, 'n');

  //コンテナ持ち上げ
  arm(-38, 400);
  arm(60, 120);
  tank(40, 40, 0.2, 0.2, false, false, 'n');
  tank(-25, -25, 0.09, 0.09, false, false, 'n');
  arm(-40, 400);
  arm(-80, 22);
  dlyms(100);

  //バックで本線に戻る
  tank(-60, -60, 1.8, 1.8, false, false, 'n');
  tank(-60, -60, 5, 5, true, true, 'w');
  tank(-60, -60, 5, 5, true, true, 'k');
  tank(40, 40, 0.064, 0.064, false, false, 'n');
  gyro(-90, 60, -60);
  
  //ライントレースで大型船へ向かう
  linetrace(30, 2, 0.8, 'w', false);
  linetrace(40, 1, 0.6, 'k', true);
  linetrace(60, 1, 0.5, 'n', true);
  linetrace(80, 4, 0.25, 'k', true);
  linetrace(60, 4, 0.4, 'k', false);
  tank(-25, -25, 0.01, 0.01, false, false, 'n');
  
  //コンテナを大型船に置く
  pup_motor_set_power(motor_C, -90);
  dlyms(200);
  gyro(78, -60, 60);
  tank(40, 40, 0.38, 0.38, false, false, 'n');
  gyro(-80, 60, -40);
  tank(40, 40, 0.3, 0.3, true, false, 'r');
  tank(40, 40, 0.02, 0.02, false, false, 'n');
  arm(40, 200);
  arm(80, 60);

  //後ろに下がってゴールへ
  tank(-60, -60, 0.1, 0.1, false, false, 'k');
  tank(-80, -80, 0.25, 0.25, true, true, 'k');
  gyro(95, -100, 100);
  tank(100, 100, 1.5, 1.5, false, false, 'n');

  //end
  exit(0);
}
