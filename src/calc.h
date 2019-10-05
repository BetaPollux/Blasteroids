#ifndef _CALC_H
#define _CALC_H

#include <math.h>
#include <stdbool.h>

#define SCREEN_W    640
#define SCREEN_H    480
#define FPS         30.0f
#define PI          3.14159f

float Random(float min, float max);
void WrapPosition(float *x, float *y);
bool IsOffScreen(float x, float y);

inline float PerSecond(float value)
{
    return value / FPS;
}

inline float ToRadians(float degrees)
{
    return (degrees * PI / 180.0f);
}

inline float ToDegrees(float radians)
{
    return (180.0f / PI * radians);
}

inline float Displacement_X(float heading, float speed)
{
    return (float)sin((double)heading) * speed;
}

inline float Displacement_Y(float heading, float speed)
{
    return -1.0f * (float)cos((double)heading) * speed;
}

#endif // _CALC_H