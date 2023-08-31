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

int Max(int *data)
{
    int total, max;

    total = sizeof(data);
    
    for (int i = 0; i < total; i++)
    {
        if (data[i] > max)
        {
            max = i;
        }
    }

    return max;
}

int Min(int *data)
{
    int total, min;

    total = sizeof(data);

    for (int i = 0; i < total; i++)
    {
        if (data[i] < min)
        {
            min = i;
        }
    }
    
    return min;
}

