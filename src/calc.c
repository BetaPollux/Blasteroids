#include "calc.h"
#include <stdlib.h>
#include <math.h>

float Random(float min, float max)
{
    int i = rand() % 1000;

    return (min + (i * 0.001f * max));
}

void WrapPosition(float *x, float *y)
{
    if (*x < 0.0f)
    {
        *x = SCREEN_W;
    }
    else if (*x > SCREEN_W)
    {
        *x = 0.0f;
    }

    if (*y < 0.0f)
    {
        *y = SCREEN_H;
    }
    else if (*y > SCREEN_H)
    {
        *y = 0.0f;
    }
}

bool IsOffScreen(float x, float y)
{
    bool inX = (x < 0.0f) || (x > SCREEN_W);
    bool inY = (y < 0.0f) || (y > SCREEN_W);

    return (inX || inY);
}
