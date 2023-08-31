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
#include "BaseProgram/Junior2_Arm.h"
#include "BaseProgram/Junior2_Gyro.h"
#include "BaseProgram/Junior2_ColorMotor.h"
#include "BaseProgram/Junior2_MotorBase.h"
#include "MoveProgram/Junior2_System.h"

pup_motor_t *motorA;             //モータAを使う変数
pup_motor_t *motorB;             //モータBを使う変数
pup_motor_t *motorC;             //モータCを使う変数
pup_motor_t *motorD;             //モータDを使う変数
pup_device_t *ColorSensor;       //カラーセンサーを使う変数

void PacificMove(int WhitePoint, int BlackPoint, int StandardAngle)
{
	int pattern, moveAngle;

	PutMove(WhitePoint, BlackPoint, StandardAngle, &pattern, &moveAngle, true);
	tADJMotor(1, 100, 40, 40, moveAngle, 1, false);
  pup_motor_hold(motorD);
  GyroTurn(90, -40, 40, 1, true);
  SecMotor(-50, 1.2f, -1);
	hub_imu_init();
	dly_tsk(300000);
  if (pattern == 0)
  {
    ADJMotor(40, 143, 1, true);
		dly_tsk(200000);
    GyroTurn(90, 40, -40, 1, false);
    dly_tsk(200000);
    RefADJMotor(40, 15, false, 0, true);
  }
  else if (pattern == 1)
  {
    ADJMotor(40, 143, 1, true);
    dly_tsk(200000);
    GyroTurn(90, -40, 40, 1, false);
    dly_tsk(200000);
    RefADJMotor(40, 15, false, 0, true);
  }
  else if (pattern == 2)
  {
    ADJMotor(40, 430, 1, true);
    dly_tsk(200000);
    GyroTurn(90, -40, 40, 1, false);
    dly_tsk(200000);
    RefADJMotor(40, 15, false, 0, true);
  }
  else if (pattern == 3)
  {
    ADJMotor(40, 143, 1, true);
    dly_tsk(200000);
    GyroTurn(90, 40, -40, 1, false);
    dly_tsk(200000);
    RefADJMotor(-40, 15, false, 0, true);
  }
	else if (pattern == 4)
	{
		ADJMotor(40, 143, 1, true);
		dly_tsk(200000);
    GyroTurn(90, -40, 40, 1, false);
    dly_tsk(200000);
    RefADJMotor(-40, 15, false, 0, true);
	}
}

void CaribMove(int WhitePoint, int BlackPoint, int StandardAngle)
{
	int pattern, moveAngle;

	PutMove(WhitePoint, BlackPoint, StandardAngle, &pattern, &moveAngle, true);
	tADJMotor(1, 100, 40, 40, moveAngle, 1, false);
  	pup_motor_hold(motorD);
  	GyroTurn(90, -40, 40, 1, true);
  	SecMotor(-50, 1.2f, -1);
	hub_imu_init();
	dly_tsk(300000);
	ADJMotor(30, 90, 1, true);
	dly_tsk(400000);
	ArmLR(1);
	ADJMotor(-30, 60, 0, true);
	pup_motor_set_power(motorD, 60);
	SecMotor(-40, 0.3f, -1);
	ArmLR(0);
	if (pattern == 0)
	{
		ADJMotor(40, 205, 1, true);
		dly_tsk(200000);
		GyroTurn(90, 40, -40, 1, false);
		dly_tsk(200000);
		RefADJMotor(40, 15, false, 0, true);
	}
	else if (pattern == 1)
	{
		ADJMotor(40, 195, 1, true);
		dly_tsk(200000);
		GyroTurn(90, -40, 40, 1, false);
		dly_tsk(200000);
		RefADJMotor(40, 15, false, 0, true);
	}
	else if (pattern == 2)
	{
		ADJMotor(40, 490, 1, true);
		dly_tsk(200000);
		GyroTurn(90, -40, 40, 1, false);
		dly_tsk(200000);
		RefADJMotor(40, 15, false, 0, true);
	}
	else if (pattern == 3)
	{
		ADJMotor(40, 205, 1, true);
		dly_tsk(200000);
		GyroTurn(90, 40, -40, 1, false);
		dly_tsk(200000);
		RefADJMotor(-40, 15, false, 0, true);
	}
	else if (pattern == 4)
	{
		ADJMotor(40, 205, 1, true);
		dly_tsk(200000);
    	GyroTurn(90, -40, 40, 1, false);
    	dly_tsk(200000);
    	RefADJMotor(-40, 15, false, 0, true);
	}
}

void ObjectPut(int Whitepoint, int Blackpoint)
{
	while (true)
	{
		if (Whitepoint == 1 || Blackpoint == 0)
		{
			ADJMotor(40, pup_motor_get_count(motorA) + 330, 1, false);
			if (Whitepoint == 1 && Blackpoint == 0)
			{
				dly_tsk(600000);
				ArmLRput();
				break;
			}
			else if (Whitepoint == 1)
			{
				dly_tsk(500000);
				ArmRput();
				if (Blackpoint == 2)
				{
					ADJMotor(-40, 145, 0, true);
					RefADJMotor(-40, 15, false, 0, false);
					tADJMotor(-1, 50, 40, 40, abs(pup_motor_get_count(motorA)) + 120, 1, false);
					dly_tsk(300000);
					GyroTurn180(-50, 50);
					dly_tsk(400000);
					ArmLput();
					break;
				}
				else if (Blackpoint == 3)
				{
					ADJMotor(-40, 145, 0, true);
					RefADJMotor(-40, 15, false, 0, false);
					tADJMotor(-1, 50, 40, 40, abs(pup_motor_get_count(motorA)) + 500, 1, false);
					ArmLput();
					break;
				}
			}
			else if (Blackpoint == 0)
			{
				dly_tsk(400000);
				ArmLput();
				if (Whitepoint == 2)
				{
					ADJMotor(-40, 150, 0, true);
					RefADJMotor(-40, 15, false, 0, false);
					tADJMotor(-1, 50, 40, 40, abs(pup_motor_get_count(motorA)) + 560, 1, false);
					dly_tsk(400000);
					ArmRput();
					break;
				}
				else if (Whitepoint == 3)
				{
					ADJMotor(-40, 150, 0, true);
					RefADJMotor(-40, 15, false, 0, false);
					ADJMotor(-40, abs(pup_motor_get_count(motorA)) + 30, 1, false);
					GyroTurn(95, 0, -50, 1, true);
					ADJMotor(-40, 200, 1, true);
					dly_tsk(500000);
					ArmRput();
					break;
				}			
			}
			
			break;
		}

		if (Blackpoint == 2)
		{
			ADJMotor(40, pup_motor_get_count(motorA) + 330, 1, false);
			ArmLput();
			if (Whitepoint == 0)
			{
				ADJMotor(-40, 150, 0, true);
				RefADJMotor(-50, 15, false, 0, false);
				tADJMotor(-1, 60, 50, 40, abs(pup_motor_get_count(motorA)) + 390, 1, false);
				GyroTurn(95, -50, 0, 1, true);
				ADJMotor(-40, 200, 1, true);
				dly_tsk(500000);
				ArmRput();
				break;
			}
			else if (Whitepoint == 3)
			{
				ADJMotor(-40, 150, 1, true);
				GyroTurn(90, 40, -40, 1, true);
				tADJMotor(1, 60, 40, 40, 230, 1, true);
				GyroTurn(182, 0, 50, 1, true);
				dly_tsk(200000);
				ADJMotor(-30, 100, 1, true);
				dly_tsk(400000);
				ArmRput();
				break;
			}

			break;	
		}
		
		if (Blackpoint == 1)
		{
			ADJMotor(-40, abs(pup_motor_get_count(motorA)) + 210, 1, false);
			dly_tsk(200000);
			GyroTurn(90, -40, 40, 1, true);
			ArmLput();
			if (Whitepoint == 0)
			{
				ADJMotor(-30, 60, 1, true);
				GyroTurn(95, 0, -50, 1, true);
				ADJMotor(-30, 130, 1, true);
				GyroTurn(90, -40, 40, 1, true);
				dly_tsk(300000);
				ArmRput();
				break;
			}
			else if (Whitepoint == 2)
			{
				ADJMotor(-30, 10, 0, true);
				GyroTurn(95, 50, 0, 1, false);
				dly_tsk(200000);
				tADJMotor(1, 50, 40, 30, 850, 1, true);
				dly_tsk(100000);
				GyroTurn(182, 50, -50, 1, false);
				dly_tsk(500000);
				ArmRput();
				break;
			}
			else if (Whitepoint == 3)
			{
				ADJMotor(-40, 120, 1, true);
				dly_tsk(200000);
				GyroTurn(94, -50, 0, 1, false);
				dly_tsk(200000);
				tADJMotor(-1, 60, 40, 30, 470, 1, true);
				dly_tsk(200000);
				GyroTurn(90, 40, -40, 1, false);
				dly_tsk(500000);
				ArmRput();
				break;
			}
			
			break;
		}
		
		if (Whitepoint == 2 && Blackpoint == 3)
		{
			tADJMotor(-1, 60, 40, 30, abs(pup_motor_get_count(motorA)) + 600, 1, false);
			ArmLRput();
			break;
		}
		
		if (Whitepoint == 0 && Blackpoint == 3)
		{
			tADJMotor(1, 50, 40, 30, abs(pup_motor_get_count(motorA)) + 400, 1, false);
			ArmLput();
			tADJMotor(-1, 60, 30, 30, 820, 1, true);
			dly_tsk(200000);
			GyroTurn(90, -40, 40, 1, false);
			ADJMotor(-30, 30, 1, true);
			dly_tsk(700000);
			ArmRput();
			break;
		}		
	}	
}

void RedMove(int Whitepoint, int Blackpoint)
{
	while (true)
	{
		if (Whitepoint == 1 || Blackpoint == 0)
		{
			if (Whitepoint == 1 && Blackpoint == 0)
			{
				ADJMotor(-40, 100, 0, true);
				RefADJMotor(-40, 15, false, 0, false);
				tADJMotor(-1, 50, 40, 30, abs(pup_motor_get_count(motorA)) + 200, 1, false);
				dly_tsk(200000);
				GyroTurn(90, -40, 40, 1, true);
				dly_tsk(200000);
				tADJMotor(1, 100, 30, 40, 1150, 1, true);
				break;
			}
			else if (Whitepoint == 1)
			{
				
				if (Blackpoint == 2)
				{
					GyroTurn(95, 40, 0, 1, true);
					dly_tsk(200000);
					tADJMotor(1, 100, 30, 40, 1000, 1, true);
					break;
				}
				else if (Blackpoint == 3)
				{
					ADJMotor(-40, 30, 0, true);
  					GyroTurn(45, -40, 40, 0, true);
					GyroTurn(45, -40, 0, 1, false);
					dly_tsk(200000);
					hub_imu_init();
					dly_tsk(300000);
					ADJMotor(50, 350, 0, true);
					GyroTurn(80, 40, 0, 0, false);
					ADJMotor(50, 380, 0, true);
					GyroTurn(80, 0, 40, 0, false);
					tADJMotor(1, 100, 50, 40, 800, 1, true);
					break;
				}			
			}
			else if (Blackpoint == 0)
			{
				if (Whitepoint == 2)
				{
					ADJMotor(-40, 30, 0, true);
  					GyroTurn(45, -40, 40, 0, true);
					GyroTurn(45, -40, 0, 1, false);
					dly_tsk(200000);
					hub_imu_init();
					dly_tsk(300000);
					ADJMotor(50, 350, 0, true);
					GyroTurn(80, 40, 0, 0, false);
					ADJMotor(50, 395, 0, true);
					GyroTurn(80, 0, 40, 0, false);
					tADJMotor(1, 100, 50, 40, 600, 1, true);
					break;
				}
				else if (Whitepoint == 3)
				{
					dly_tsk(200000);
					GyroTurn(182, -50, 50, 1, false);
					dly_tsk(200000);
					tADJMotor(1, 100, 50, 40, 800, 1, true);
					break;
				}			
			}
			
			break;
		}

		if (Blackpoint == 2)
		{
			if (Whitepoint == 0)
			{
				GyroTurn(90, 40, -40, 1, false);
				dly_tsk(100000);
				tADJMotor(1, 80, 30, 30, 850, 1, true);
				dly_tsk(200000);
				GyroTurn(90, 40, -40, 1, false);
				dly_tsk(100000);
				tADJMotor(1, 100, 50, 40, 800, 1, true);
				break;
			}
			else if (Whitepoint == 3)
			{
				dly_tsk(200000);
				GyroTurn(182, -50, 50, 1, false);
				dly_tsk(200000);
				tADJMotor(1, 100, 50, 40, 850, 1, true);
				break;
			}

			break;	
		}
		
		if (Blackpoint == 1)
		{
			if (Whitepoint == 0)
			{
				GyroTurn(90, 40, -40, 1, false);
				dly_tsk(100000);
				tADJMotor(1, 80, 30, 30, 850, 1, true);
				dly_tsk(200000);
				GyroTurn(90, 40, -40, 1, false);
				dly_tsk(100000);
				tADJMotor(1, 100, 50, 40, 800, 1, true);
				break;
			}
			else if (Whitepoint == 2)
			{
				ADJMotor(-40, 30, 0, true);
  				GyroTurn(45, -40, 40, 0, true);
				GyroTurn(45, -40, 0, 1, false);
				dly_tsk(200000);
				hub_imu_init();
				dly_tsk(300000);
				ADJMotor(50, 320, 0, true);
				GyroTurn(80, 40, 0, 0, false);
				ADJMotor(50, 490, 0, true);
				GyroTurn(80, 0, 40, 0, false);
				tADJMotor(1, 100, 50, 40, 600, 1, true);
				break;
			}
			else if (Whitepoint == 3)
			{
				dly_tsk(200000);
				GyroTurn(182, -50, 50, 1, false);
				dly_tsk(200000);
				tADJMotor(1, 100, 50, 40, 850, 1, true);
				break;
			}
			
			break;
		}
		
		if (Whitepoint == 2 && Blackpoint == 3)
		{
			ADJMotor(-40, 30, 0, true);
  			GyroTurn(45, -40, 40, 0, true);
			GyroTurn(45, -40, 0, 1, false);
			dly_tsk(200000);
			hub_imu_init();
			dly_tsk(300000);
			ADJMotor(50, 320, 0, true);
			GyroTurn(80, 40, 0, 0, false);
			ADJMotor(50, 490, 0, true);
			GyroTurn(80, 0, 40, 0, false);
			tADJMotor(1, 100, 50, 40, 600, 1, true);
			break;
		}
		
		if (Whitepoint == 0 && Blackpoint == 3)
		{
			GyroTurn(90, 40, -40, 1, false);
			dly_tsk(100000);
			tADJMotor(1, 80, 30, 30, 880, 1, true);
			dly_tsk(200000);
			GyroTurn(90, 40, -40, 1, false);
			dly_tsk(100000);
			tADJMotor(1, 100, 50, 40, 800, 1, true);
			break;
		}		
	}	
}

void Goal(int Whitepoint, int Blackpoint)
{
	while (true)
	{
		if (Whitepoint == 1 || Blackpoint == 0)
		{
			if (Whitepoint == 1 && Blackpoint == 0)
			{
				ADJMotor(-40, 100, 0, true);
				RefADJMotor(-40, 15, false, 0, false);
				GyroTurn(90, -40, 40, 0, false);
				ADJMotor(40, 250, 0, true);
				GyroTurn(94, 50, 0, 0, false);
				tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 800, 0, true);
				GyroTurn(75, 60, 100, 0, false);
				SecMotor(100, 0.8f, -1);
				
				break;
			}
			else if (Whitepoint == 1)
			{
				
				if (Blackpoint == 2)
				{
					GyroTurn(175, 60, 0, 0, false);
					dly_tsk(200000);
					tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 800, 0, true);
					GyroTurn(75, 60, 100, 0, false);
					SecMotor(100, 0.8f, -1);
					break;
				}
				else if (Blackpoint == 3)
				{
					ADJMotor(-40, 30, 0, true);
  					GyroTurn(45, -40, 40, 0, true);
					GyroTurn(45, -40, 0, 0, false);
					ADJMotor(50, 360, 0, true);
					GyroTurn(94, 60, 0, 0, false);
					tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 1500, 0, true);
					GyroTurn(75, 60, 100, 0, false);
					SecMotor(100, 0.8f, -1);
					break;
				}
			
			}
			else if (Blackpoint == 0)
			{
				if (Whitepoint == 2)
				{
					ADJMotor(-40, 30, 0, true);
  					GyroTurn(45, -40, 40, 0, true);
					GyroTurn(45, -40, 0, 0, false);
					ADJMotor(50, 360, 0, true);
					GyroTurn(94, 60, 0, 0, false);
					tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 1500, 0, true);
					GyroTurn(75, 60, 100, 0, false);
					SecMotor(100, 0.8f, -1);
					break;
				}
				else if (Whitepoint == 3)
				{
					ADJMotor(30, 20, 0, true);
					GyroTurn(90, -50, 50, 1, false);
					tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 1500, 0, true);
					GyroTurn(75, 60, 100, 0, false);
					SecMotor(100, 0.8f, -1);
					break;
				}			
			}

			break;
		}

		if (Blackpoint == 2)
		{
			if (Whitepoint == 0)
			{
				ADJMotor(30, 20, 0, true);
				GyroTurn(90, -50, 50, 1, false);
				tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 600, 0, true);
				GyroTurn(75, 60, 100, 0, false);
				SecMotor(100, 0.8f, -1);
				break;
			}
			else if (Whitepoint == 3)
			{
				ADJMotor(30, 20, 0, true);
				GyroTurn(90, -50, 50, 1, false);
				tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 1500, 0, true);
				GyroTurn(75, 60, 100, 0, false);
				SecMotor(100, 0.8f, -1);
				break;
			}

			break;	
		}
		
		if (Blackpoint == 1)
		{
			if (Whitepoint == 0)
			{
				ADJMotor(30, 20, 0, true);
				GyroTurn(90, -50, 50, 1, false);
				tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 600, 0, true);
				GyroTurn(75, 60, 100, 0, false);
				SecMotor(100, 0.8f, -1);
				break;
			}
			else if (Whitepoint == 2)
			{
				ADJMotor(-40, 30, 0, true);
  				GyroTurn(45, -40, 40, 0, true);
				GyroTurn(45, -40, 0, 0, false);
				ADJMotor(50, 330, 0, true);
				GyroTurn(97, 60, 0, 0, false);
				tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 1500, 0, true);
				GyroTurn(75, 60, 100, 0, false);
				SecMotor(100, 0.8f, -1);
				break;
			}
			else if (Whitepoint == 3)
			{
				ADJMotor(30, 20, 0, true);
				GyroTurn(90, -50, 50, 1, false);
				tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 1500, 0, true);
				GyroTurn(75, 60, 100, 0, false);
				SecMotor(100, 0.8f, -1);
				break;
			}
			
			break;
		}
		
		if (Whitepoint == 2 && Blackpoint == 3)
		{
			ADJMotor(-40, 30, 0, true);
  			GyroTurn(45, -40, 40, 0, true);
			GyroTurn(45, -40, 0, 0, false);
			ADJMotor(50, 330, 0, true);
			GyroTurn(95, 60, 0, 0, false);
			tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 1500, 0, true);
			GyroTurn(75, 60, 100, 0, false);
			SecMotor(100, 0.8f, -1);
			break;
		}
		
		if (Whitepoint == 0 && Blackpoint == 3)
		{
			ADJMotor(30, 20, 0, true);
			GyroTurn(90, -50, 50, 1, false);
			tADJMotor(1, 100, 40, 70, abs(pup_motor_get_count(motorA)) + 600, 0, true);
			GyroTurn(75, 60, 100, 0, false);
			SecMotor(100, 0.8f, -1);
			break;
		}		
	}	
}
