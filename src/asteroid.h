#ifndef _ASTEROID_H
#define _ASTEROID_H

#include <allegro5/allegro.h>

typedef struct {
    float sx;
    float sy;
    float heading;
    float twist;
    float speed;
    float rot_velocity;
    float scale;
    int gone;
    ALLEGRO_COLOR color;
} Asteroid;

#endif // _ASTEROID_H
