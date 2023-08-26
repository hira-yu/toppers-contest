//PUPデバイス変数
pup_motor_t *motor_A;
pup_motor_t *motor_B;
pup_motor_t *motor_C;
pup_device_t *cs_A;
pup_device_t *cs_B;
hub_button_t pressed;

//dly_task(ms ver.)
void dlyms(float time) {
  dly_tsk(time*1000);
}

//pup device setup
void pup_device_setup() {
  motor_A = pup_motor_get_device(PBIO_PORT_ID_A);
  motor_B = pup_motor_get_device(PBIO_PORT_ID_B);
  motor_C = pup_motor_get_device(PBIO_PORT_ID_E);

  cs_A = pup_color_sensor_get_device(PBIO_PORT_ID_C);
  cs_B = pup_color_sensor_get_device(PBIO_PORT_ID_D);

  pup_motor_setup(motor_A, PUP_DIRECTION_COUNTERCLOCKWISE, true);
  pup_motor_setup(motor_B, PUP_DIRECTION_CLOCKWISE, true);
  pup_motor_setup(motor_C, PUP_DIRECTION_COUNTERCLOCKWISE, true);
}

//linetrace | 引数 : モーターパワー, 回転数, ゲイン, 停止色, ダブルセンサー
void linetrace(int power, float count, float gein, char color, bool duble) {
  //モーターエンコーダ初期化
  pup_motor_reset_count(motor_A);
  pup_motor_reset_count(motor_B);

  //条件ヒットまでループ
  while (true) {
    //モーターA, Bの回転角度を格納
    int motor_1_deg = pup_motor_get_count(motor_A);
    int motor_2_deg = pup_motor_get_count(motor_B);
    
    //反射光 白:35付近, 黒:5付近
    //反射光に応じてモーターパワーを指定
    if (duble) {
      pup_motor_set_power(motor_A, power*1.2 - ((pup_color_sensor_reflection(cs_B) - 5) - (pup_color_sensor_reflection(cs_A) - 5) * 2) * gein);
      pup_motor_set_power(motor_B, power*1.2 + ((pup_color_sensor_reflection(cs_B) - 5) - (pup_color_sensor_reflection(cs_A) - 5) * 2) * gein);
    } else {
      pup_motor_set_power(motor_A, power*1.2 - (50 - (pup_color_sensor_reflection(cs_A) - 5) * 3) * gein);
      pup_motor_set_power(motor_B, power*1.2 + (50 - (pup_color_sensor_reflection(cs_A) - 5) * 3) * gein);
    }

    //条件ヒットしたらループを抜ける
    if (count * 360 <= (motor_1_deg + motor_2_deg) / 2) {
      break;
    } else if (pup_color_sensor_color_name(cs_A, true) == color && pup_color_sensor_color_name(cs_B, true) == color && !duble) {
      break;
    } else if (color == 'w' && pup_color_sensor_color_name(cs_B, true) == color) {
      break;
    }
  }

  //モーター停止
  pup_motor_stop(motor_A);
  pup_motor_stop(motor_B);
}

//linetrace2 | 引数 : モーターパワー, 回転数, ゲイン, 停止色, ダブルセンサー
void linetrace2(int power, float count, float gein, char color, bool duble) {
  //モーターエンコーダ初期化
  pup_motor_reset_count(motor_A);
  pup_motor_reset_count(motor_B);

  //条件ヒットまでループ
  while (true) {
    //モーターA, Bの回転角度を格納
    int motor_1_deg = pup_motor_get_count(motor_A);
    int motor_2_deg = pup_motor_get_count(motor_B);
    
    //反射光 白:35付近, 黒:5付近
    //反射光に応じてモーターパワーを指定
    if (duble) {
      pup_motor_set_power(motor_A, power*1.2 + ((pup_color_sensor_reflection(cs_A) - 5) - (pup_color_sensor_reflection(cs_B) - 5) * 2) * gein);
      pup_motor_set_power(motor_B, power*1.2 - ((pup_color_sensor_reflection(cs_A) - 5) - (pup_color_sensor_reflection(cs_B) - 5) * 2) * gein);
    } else {
      pup_motor_set_power(motor_A, power*1.2 + (50 - (pup_color_sensor_reflection(cs_B) - 5) * 3) * gein);
      pup_motor_set_power(motor_B, power*1.2 - (50 - (pup_color_sensor_reflection(cs_B) - 5) * 3) * gein);
    }

    //条件ヒットしたらループを抜ける
    if (count * 360 <= (motor_1_deg + motor_2_deg) / 2) {
      break;
    } else if (pup_color_sensor_color_name(cs_A, true) == color && !duble) {
      break;
    }
  }

  //モーター停止
  pup_motor_stop(motor_A);
  pup_motor_stop(motor_B);
}

//tank | 引数 : モーターAパワー, モーターBパワー, モーターA回転数, モーターB回転数, 色読停止, デュアル読, 停止色
void tank(int power1, int power2, float count1, float count2, bool color_stop, bool duo, char color) {  
  //モーターエンコーダ初期化
  pup_motor_reset_count(motor_A);
  pup_motor_reset_count(motor_B);

  //モーターパワー指定
  pup_motor_set_power(motor_A, power1);
  pup_motor_set_power(motor_B, power2);

  //同速で移動時にジャイロ使用
  if (power1 == power2) {
    //imu初期化
    hub_imu_init();
  }
  
  //ジャイロ用変数
  float gx[3];
  float time = 0.02;
  float deg;
  
  //条件ヒットまでループ
  while (true) {
    //モーターA, Bの回転角度を格納
    int motor_1_deg = pup_motor_get_count(motor_A);
    int motor_2_deg = pup_motor_get_count(motor_B);

    //同速で移動時にジャイロ使用
    if (power1 == power2) {
      //角速度を格納
      hub_imu_get_angular_velocity(gx);

      //z軸を積分格納して待機
      //動いていなくても軸がマイナスに動くためプラス側に補正
      deg += gx[2] * time + 0.024;
      dlyms(time * 1000);

      //debug
      hub_display_number((int)deg);

      //モーターパワーを補正
      pup_motor_set_power(motor_B, power2 + -deg * 4);
    }
    
    //条件ヒットしたらループを抜ける
    if (-motor_1_deg >= count1*360 && power1 < 0) { //負回転かつ指定回転数を超えたらループを抜ける
      break;
    } else if (motor_1_deg >= count1*360 && 0 < power1) { //正回転かつ指定回転数を超えたらループを抜ける
      break;
    } else if (pup_motor_is_stalled(motor_A) || pup_motor_is_stalled(motor_B)) { //モーターがストールしたらループを抜ける
      break;
    }

    if (-motor_2_deg >= count2*360 && power2 < 0) { //負回転かつ指定回転数を超えたらループを抜ける
      break;
    } else if (motor_2_deg >= count2*360 && 0 < power2) { //正回転かつ指定回転数を超えたらループを抜ける
      break;
    } else if (pup_motor_is_stalled(motor_A) || pup_motor_is_stalled(motor_B)) {　//モーターがストールしたらループを抜ける
      break;
    }

    if (color_stop) {
      if (pup_color_sensor_color_name(cs_A, true) == color && pup_color_sensor_color_name(cs_B, true) == color && duo) { //両方のセンサーが指定色を読み取ったときループを抜ける
        break;
      }
      else if (pup_color_sensor_color_name(cs_A, true) == color || pup_color_sensor_color_name(cs_B, true) == color && !duo) {　//片方のセンサーが指定色を読み取ったときループを抜ける
        break;
      }
    }
  }

  //モーター停止
  pup_motor_stop(motor_A);
  pup_motor_stop(motor_B);
}

//arm | 引数 : モーターパワー, 回転角
void arm(int power, int deg) {  
  //モータートルクを指定
  pup_motor_set_duty_limit(motor_C, 100);

  //モーターエンコーダ初期化
  pup_motor_reset_count(motor_C);

  //モーターパワー指定
  pup_motor_set_power(motor_C, power);

  //条件ヒットまでループ
  while (true) {
    //モーターの回転角度を格納
    int motor_3_deg = pup_motor_get_count(motor_C);
    
    //条件ヒットしたらループを抜ける
    if (-motor_3_deg >= deg && power < 0) { //負回転かつ指定回転数を超えたらループを抜ける
      break;
    } else if (motor_3_deg >= deg && 0 < power) { //正回転かつ指定回転数を超えたらループを抜ける
      break;
    } else if (pup_motor_is_stalled(motor_C)) { //モーターがストールしたらループを抜ける
      break;
    }
  }

  //モーター停止
  pup_motor_hold(motor_C);
}

//gyro turn | 引数 : 旋回角, モーターA回転数, モーターB回転数
void gyro(float set_deg, int power1, int power2) {
  //imu初期化
  hub_imu_init();
  
  //ジャイロ用変数
  float gx[3];
  float time = 0.02;
  float deg;

  //旋回角調整値
  int correction = 16;

  //モーターパワー指定
  pup_motor_set_power(motor_A, power1);
  dlyms(50);
  pup_motor_set_power(motor_B, power2);

  //条件ヒットまでループ
  while (true) {
    //角速度を格納
    hub_imu_get_angular_velocity(gx);

    //z軸を積分格納して待機
    //動いていなくても軸がマイナスに動くためプラス側に補正
    deg += gx[2] * time + 0.024;
    dlyms(time * 1000);

    //debug
    hub_display_number((int)deg);

    //条件ヒットしたらループを抜ける
    if (deg <= set_deg + 12 + correction && set_deg < 0) { //負旋回かつ指定旋回角を超えたらループを抜ける
      break;
    } else if (deg >= set_deg - 12 - correction && set_deg > 0) { //正旋回かつ指定旋回角を超えたらループを抜ける
      break;
    }
  }

  //モーター停止
  pup_motor_stop(motor_A);
  pup_motor_stop(motor_B);
}
