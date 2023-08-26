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
#include "Junior_GyroMotor.h"
#include "Junior_MotorBase.h"
#include "Junior_Logic.h"
#include "Junior_Linetrace.h"
#include "Junior_Arm.h"

//Move files

pup_motor_t *motorA;             //モータAを使う変数
pup_motor_t *motorB;             //モータBを使う変数
pup_motor_t *motorC;
pup_motor_t *motorE;
pup_device_t *ColorSensor;       //カラーセンサーを使う変数
pup_device_t *ForceSensor;       //フォースセンサーを使う変数
pup_device_t *UltraSonicSensor;  //距離センサーを使う変数

void Objectput (int Whitepoint, int Blackpoint)
{
    while (true)
    {
        AngleLinetrace(30, 25, 3, 1, 130, 0);
        ColorMotor(30, 30, 12, false, 1);
        dly_tsk(300000);

        if (Whitepoint == 1 || Blackpoint == 0)
        {
            AdjMotor(-30, 25, 1, true);
            dly_tsk(300000);
            GyroTurn(30, -30, 74);
            dly_tsk(300000);
            AdjMotor(50, 210, 1, true);

            if (Whitepoint == 1 && Blackpoint == 0)
            {
                MotorCEput();
                AdjMotor(-50, 180, 1, true);
                break;                
            }
            else if (Whitepoint == 1)
            {
                MotorEput();
                AdjMotor(-50, 180, 1, true);

                if (Blackpoint == 2)
                {
                    GyroTurn(-40, 40, 150);
                    AdjMotor(50, 190, 1, true);
                    MotorCput();
                    break;            
                }
                else if (Blackpoint == 3)
                {
                    AdjMotor(-50, 760, 1, false);
                    dly_tsk(300000);
                    GyroTurn(40, -40, 74);
                    dly_tsk(300000);
                    AdjMotor(-50, 400, 1, true);
                    MotorCput();
                    break;
                }                
            }
            else if (Blackpoint == 0)
            {
                MotorCput();
                AdjMotor(-50, 200, 1, true);

                if (Whitepoint == 2)
                {
                    AdjMotor(-60, 690, 10, true);
                    AdjMotor(-40, 50, 1, true);
                    MotorEput();
                    break;
                }
                else if (Whitepoint == 3)
                {
                    AdjMotor(-50, 460, 1, false);
                    AdjMotor(-40, 50, 1, false);
                    dly_tsk(300000);
                    GyroTurn(40, -40, 73);
                    dly_tsk(300000);
                    AdjMotor(-50, 350, 1, true);
                    MotorEput();
                    break;                    
                }
            }

            break;
        }

        if (Blackpoint == 2)
        {
            AdjMotor(-40, 45, 1, true);
            dly_tsk(300000);
            GyroTurn(-30, 30, 71.5f);
            dly_tsk(300000);
            AdjMotor(50, 200, 1, true);
            MotorCput();
            AdjMotor(-50, 165, 1, true);

            if (Whitepoint == 0)
            {
                AdjMotor(-60, 580, 0, true);
                AdjMotor(-40, 50, 1, true);
                dly_tsk(300000);
                GyroTurn(-40, 40, 73);
                dly_tsk(300000);
                AdjMotor(-50, 360, 1, true);
                MotorEput();         
                break;       
            }
            else if (Whitepoint == 3)
            {   
                dly_tsk(300000);
                GyroTurn(40, -40, 76);
                dly_tsk(300000);
                AdjMotor(40, 80, 0, true);
                AngleLinetrace(30, 30, 3, 1, 120, 1);
                dly_tsk(300000);
                GyroTurn(0, 60, 142);
                dly_tsk(300000);
                AdjMotor(-50, 100, 1, true);
                MotorEput();
                break;
            }
            
            break;
        }
        
        if (Blackpoint == 1)
        {
            GyroTurn(30, -30, 74);
            dly_tsk(300000);
            AdjMotor(50, 305, 1, true);
            dly_tsk(300000);
            GyroTurn(30, -30, 74);
            dly_tsk(300000);
            MotorCput();
            dly_tsk(300000);
            GyroTurn(40, -40, 74);
            dly_tsk(300000);
            AdjMotor(50, 305, 1, true);
            dly_tsk(300000);
            GyroTurn(40, -40, 74);
            dly_tsk(300000);
            pup_motor_set_power(motorA, -50);
            pup_motor_set_power(motorB, -50);
            dly_tsk(1000000);
            pup_motor_brake(motorA);
            pup_motor_brake(motorB);

            if (Whitepoint == 0)
            {
                AdjMotor(50, 180, 0, true);
                AngleLinetrace(40, 40, 2, 1, 260, 1);
                dly_tsk(300000);
                GyroTurn(-40, 40, 73);
                dly_tsk(300000);
                AdjMotor(-60, 540, 0, true);
                AdjMotor(-40, 60, 1, true);
                dly_tsk(300000);
                GyroTurn(-40, 40, 76);
                dly_tsk(300000);
                MotorEput();
                GyroTurn(40, -40, 76);
                break;
            }
            else if (Whitepoint == 2)
            {
                AdjMotor(50, 125, 1, true);
                dly_tsk(300000);
                GyroTurn(30, -30, 74);
                dly_tsk(300000);
                AdjMotor(-60, 660, 0, true);
                AdjMotor(-40, 50, 1, true);
                MotorEput();
                break;
            }
            else if (Whitepoint == 3)
            {
                AdjMotor(50, 190, 0, true);
                AngleLinetrace(30, 30, 3, 1, 150, 1);
                dly_tsk(300000);
                GyroTurn(0, 60, 142);
                dly_tsk(300000);
                AdjMotor(-40, 100, 1, true);
                MotorEput();
                break;
            }
            
            break;
        }
        
        if (Whitepoint == 2 && Blackpoint == 3)
        {
            AdjMotor(-40, 35, 1, true);
            dly_tsk(300000);
            GyroTurn(30, -30, 74);
            dly_tsk(300000);
            AdjMotor(-50, 730, 1, true);
            MotorCEput();
            break;     
        }        

        if (Whitepoint == 0 && Blackpoint == 3)
        {
            AdjMotor(40, 30, 0, true);
            AngleLinetrace(30, 30, 2, 1, 200, 1);
            dly_tsk(300000);
            GyroTurn(-30, 30, 73);
            dly_tsk(300000);
            AdjMotor(50, 120, 1, true);
            MotorCput();
            AdjMotor(-60, 640, 0, true);
            AdjMotor(-40, 55, 1, true);
            dly_tsk(300000);
            GyroTurn(-40, 40, 74);
            dly_tsk(300000);
            AdjMotor(-40, 50, 1, true);
            MotorEput();
            dly_tsk(300000);
            GyroTurn(40, -40, 76);
            break;
        }        
    }
    
    CaribWall(Whitepoint, Blackpoint);
}

void CaribWall(int Whitepoint, int Blackpoint)
{
    while (true)
    {
        if (Whitepoint == 1 || Blackpoint == 0)
        {
            if (Whitepoint == 1 && Blackpoint == 0)
            {
                dly_tsk(300000);
                GyroTurn(50, -50, 71);
                dly_tsk(300000);
                AdjMotor(-60, 360, 0, true);
                AdjMotor(-40, 50, 1, false);
                dly_tsk(300000);
                GyroTurn(50, -50, 76);
                AdjMotor(-80, 1200, 0, true);
                break;
            }
            else if (Whitepoint == 1)
            {
                if (Blackpoint == 2)
                {
                    AdjMotor(-50, 200, 1, true);
                    dly_tsk(300000);
                    GyroTurn(-50, 50, 71);
                    dly_tsk(300000);
                    AdjMotor(-60, 370, 0, true);
                    AdjMotor(-40, 50, 1, false);
                    dly_tsk(300000);
                    GyroTurn(50, -50, 76);
                    AdjMotor(-80, 1200, 0, true);
                    break;
                }
                else if (Blackpoint == 3)
                {
                    AdjMotor(40, 30, 1, true);
                    dly_tsk(300000);
                    GyroTurn(50, -50, 70);
                    AdjMotor(-80, 1600, 0, true);
                    break;
                }                
            }
            else if (Blackpoint == 0)
            {
                if (Whitepoint == 2)
                {
                    dly_tsk(300000);
                    GyroTurn(50, -50, 76);
                    AdjMotor(-60, 350, 1, true);
                    dly_tsk(300000);
                    GyroTurn(50, -50, 73);
                    AdjMotor(-80, 1900, 0, true);
                    break;
                }
                else if (Whitepoint == 3)
                {
                    dly_tsk(300000);
                    GyroTurn(50, -50, 73);
                    AdjMotor(-80, 1500, 0, true);
                    break;
                }                
            }            
        }
        
        if (Blackpoint == 2)
        {
            if (Whitepoint == 0)
            {
                dly_tsk(300000);
                GyroTurn(50, -50, 73);
                AdjMotor(-80, 600, 0, true);
                break;
            }
            else if (Whitepoint == 3)
            {
                dly_tsk(300000);
                GyroTurn(50, -50, 67);
                AdjMotor(-80, 1500, 0, true);
                break;
            }            
        }

        if (Blackpoint == 1)
        {
            if (Whitepoint == 0)
            {
                AdjMotor(-80, 600, 0, true);
                break;
            }
            else if (Whitepoint == 2)
            {
                dly_tsk(300000);
                GyroTurn(50, -50, 76);
                AdjMotor(-60, 330, 1, true);
                dly_tsk(300000);
                GyroTurn(50, -50, 74);
                AdjMotor(-80, 1900, 0, true);
                break;
            }
            else if (Whitepoint == 3)
            {
                dly_tsk(300000);
                GyroTurn(50, -50, 69);
                AdjMotor(-80, 1500, 0, true);
                break;
            }           
        }
        
        if (Whitepoint == 2 && Blackpoint == 3)
        {
            dly_tsk(300000);
            GyroTurn(50, -50, 76);
            AdjMotor(-60, 360, 1, true);
            dly_tsk(300000);
            GyroTurn(50, -50, 74);
            AdjMotor(-80, 1900, 0, true);
            break;
        }
        
        if (Whitepoint == 0 && Blackpoint == 3)
        {
            AdjMotor(-80, 600, 0, true);
            break;
        }    
    }
    
    pup_motor_set_power(motorA, -70);
    pup_motor_set_power(motorB, -70);
    dly_tsk(600000);
    pup_motor_set_power(motorA, -40);
    pup_motor_set_power(motorB, -40);
    dly_tsk(400000);
    pup_motor_brake(motorA);
    pup_motor_brake(motorB);
}

void Pacificput (int Whitepoint, int Blackpoint)
{
    while (true)
    {
        AngleLinetrace(30, 25, 3, 1, 200, 0);
        ColorMotor(30, 30, 12, false, 1);
        dly_tsk(300000);

        if (Whitepoint == 1 || Blackpoint == 0)
        {
            AdjMotor(-40, 30, 1, true);
            dly_tsk(300000);
            GyroTurn(30, -30, 74.5f);
            dly_tsk(300000);
            AdjMotor(50, 210, 1, true);

            if (Whitepoint == 1 && Blackpoint == 0)
            {
                MotorCEputP();
                AdjMotor(-50, 180, 1, true);
                break;                
            }
            else if (Whitepoint == 1)
            {
                MotorEputP();
                AdjMotor(-50, 180, 1, true);

                if (Blackpoint == 2)
                {
                    GyroTurn(-40, 40, 150);
                    AdjMotor(50, 190, 1, true);
                    MotorCputP();
                    break;            
                }
                else if (Blackpoint == 3)
                {
                    AdjMotor(-60, 720, 0, false);
                    AdjMotor(-40, 50, 1, false);
                    dly_tsk(300000);
                    GyroTurn(40, -40, 74);
                    dly_tsk(300000);
                    AdjMotor(-50, 370, 1, true);
                    MotorCputP();
                    break;
                }                
            }
            else if (Blackpoint == 0)
            {
                MotorCputP();
                AdjMotor(-50, 200, 1, true);

                if (Whitepoint == 2)
                {
                    AdjMotor(-60, 690, 1, true);
                    AdjMotor(-40, 50, 1, true);
                    MotorEputP();
                    break;
                }
                else if (Whitepoint == 3)
                {
                    AdjMotor(-50, 500, 1, false);
                    dly_tsk(300000);
                    GyroTurn(40, -40, 73);
                    dly_tsk(300000);
                    AdjMotor(-50, 345, 1, true);
                    MotorEputP();
                    break;                    
                }
            }            
            
            break;
        }

        if (Blackpoint == 2)
        {
            AdjMotor(-30, 45, 1, true);
            dly_tsk(300000);
            GyroTurn(-30, 30, 72);
            dly_tsk(300000);
            AdjMotor(50, 200, 1, true);
            MotorCputP();
            AdjMotor(-50, 175, 1, true);

            if (Whitepoint == 0)
            {
                AdjMotor(-60, 585, 0, true);
                AdjMotor(-40, 50, 1, true);
                dly_tsk(300000);
                GyroTurn(-40, 40, 73);
                dly_tsk(300000);
                AdjMotor(-50, 360, 1, true);
                MotorEputP();         
                break;       
            }
            else if (Whitepoint == 3)
            {   
                dly_tsk(300000);
                GyroTurn(40, -40, 73);
                dly_tsk(300000);
                AdjMotor(40, 80, 0, true);
                AngleLinetrace(30, 30, 3, 1, 120, 1);
                dly_tsk(300000);
                GyroTurn(0, 60, 142);
                dly_tsk(300000);
                AdjMotor(-40, 100, 1, true);
                MotorEputP();
                break;
            }
            
            break;
        }
        
        if (Blackpoint == 1)
        {
            GyroTurn(30, -30, 74);
            dly_tsk(300000);
            AdjMotor(50, 335, 1, true);
            dly_tsk(300000);
            GyroTurn(30, -30, 74);
            dly_tsk(300000);
            MotorCputP();
            dly_tsk(300000);
            GyroTurn(40, -40, 74);
            dly_tsk(300000);
            AdjMotor(50, 335, 1, true);
            dly_tsk(300000);
            GyroTurn(40, -40, 74);
            dly_tsk(300000);
            pup_motor_set_power(motorA, -40);
            pup_motor_set_power(motorB, -40);
            dly_tsk(1100000);
            pup_motor_brake(motorA);
            pup_motor_brake(motorB);

            if (Whitepoint == 0)
            {
                AdjMotor(50, 260, 0, true);
                AngleLinetrace(40, 40, 3, 1, 230, 1);
                dly_tsk(300000);
                GyroTurn(-40, 40, 71);
                dly_tsk(300000);
                AdjMotor(-60, 570, 0, true);
                AdjMotor(-40, 60, 1, true);
                dly_tsk(300000);
                GyroTurn(-40, 40, 73);
                AdjMotor(-30, 35, 1, true);
                MotorEputP();
                break;
            }
            else if (Whitepoint == 2)
            {
                AdjMotor(50, 200, 1, true);
                dly_tsk(300000);
                GyroTurn(40, -40, 74);
                AdjMotor(-60, 660, 0, true);
                AdjMotor(-40, 50, 1, true);
                MotorEputP();
                break;
            }
            else if (Whitepoint == 3)
            {
                AdjMotor(50, 250, 0, true);
                AngleLinetrace(30, 30, 3, 1, 130, 1);
                dly_tsk(300000);
                GyroTurn(0, 60, 142);
                dly_tsk(300000);
                AdjMotor(-50, 110, 1, true);
                MotorEputP();
                break;
            }
            
            break;
        }
        
        if (Whitepoint == 2 && Blackpoint == 3)
        {
            AdjMotor(-30, 25, 1, true);
            dly_tsk(300000);
            GyroTurn(30, -30, 74);
            AdjMotor(-60, 680, 1, true);
            AdjMotor(-40, 50, 1, true);
            MotorCEputP();
            break;     
        }        

        if (Whitepoint == 0 && Blackpoint == 3)
        {
            AdjMotor(40, 30, 0, true);
            AngleLinetrace(30, 30, 2, 1, 180, 1);
            dly_tsk(300000);
            GyroTurn(-30, 30, 73);
            dly_tsk(300000);
            AdjMotor(50, 120, 1, true);
            MotorCputP();
            AdjMotor(-60, 640, 0, true);
            AdjMotor(-40, 90, 1, true);
            dly_tsk(300000);
            GyroTurn(-40, 40, 74);
            dly_tsk(300000);
            AdjMotor(-40, 80, 1, true);
            MotorEputP();
            break;
        }        
    }   

    PacificWall(Whitepoint, Blackpoint);
}

void PacificWall(int Whitepoint, int Blackpoint)
{
    while (true)
    {
        if (Whitepoint == 1 || Blackpoint == 0)
        {
            if (Whitepoint == 1 && Blackpoint == 0)
            {
                GyroTurn(-50, 50, 74);
                AdjMotor(50, 400, 1, true);
                GyroTurn(50, -50, 74);
                AdjMotor(90, 1200, 1, true);
                break;
            }
            else if (Whitepoint == 1)
            {
                if (Blackpoint == 2)
                {
                    AdjMotor(-40, 100, 1, true);
                    dly_tsk(300000);
                    GyroTurn(50, -50, 74);
                    AdjMotor(50, 320, 1, true);
                    GyroTurn(50, -50, 74);
                    AdjMotor(90, 1200, 1, true);
                    break;
                }
                else if (Blackpoint == 3)
                {
                    dly_tsk(300000);
                    AdjMotor(40, 10, 1, true);
                    GyroTurn(-50, 50, 74);
                    AdjMotor(90, 1700, 1, true);
                    break;
                }                
            }
            else if (Blackpoint == 0)
            {
                if (Whitepoint == 2)
                {
                    dly_tsk(300000);
                    GyroTurn(-50, 50, 74);
                    AdjMotor(50, 380, 1, true);
                    dly_tsk(300000);
                    GyroTurn(50, -50, 74);
                    AdjMotor(90, 2000, 1, true);
                    break;
                }
                else if (Whitepoint == 3)
                {
                    dly_tsk(300000);
                    GyroTurn(-50, 50, 72);
                    AdjMotor(90, 1700, 1, true);
                    break;
                }                
            }            
        }
        
        if (Blackpoint == 2)
        {
            if (Whitepoint == 0)
            {
                dly_tsk(300000);
                GyroTurn(-50, 50, 72);
                AdjMotor(90, 900, 1, true);
                break;
            }
            else if (Whitepoint == 3)
            {
                dly_tsk(300000);
                GyroTurn(-50, 50, 79);
                AdjMotor(90, 1700, 1, true);
                break;
            }            
        }

        if (Blackpoint == 1)
        {
            if (Whitepoint == 0)
            {
                dly_tsk(300000);
                GyroTurn(-50, 50, 72);
                AdjMotor(90, 800, 1, true);
                break;
            }
            else if (Whitepoint == 2)
            {
                dly_tsk(300000);
                GyroTurn(-50, 50, 74);
                AdjMotor(50, 370, 1, true);
                dly_tsk(300000);
                GyroTurn(50, -50, 75);
                AdjMotor(90, 2100, 1, true);
                break;
            }
            else if (Whitepoint == 3)
            {
                dly_tsk(300000);
                GyroTurn(-50, 50, 79);
                AdjMotor(90, 1700, 1, true);
                break;
            }            
        }
        
        if (Whitepoint == 2 && Blackpoint == 3)
        {
            dly_tsk(300000);
            GyroTurn(-50, 50, 74);
            AdjMotor(50, 390, 1, true);
            dly_tsk(300000);
            GyroTurn(50, -50, 74);
            AdjMotor(90, 2100, 1, true);
            break;
        }
        
        if (Whitepoint == 0 && Blackpoint == 3)
        {
            dly_tsk(300000);
            GyroTurn(-50, 50, 72);
            AdjMotor(90, 800, 1, true);
            break;
        }    
    }    
}