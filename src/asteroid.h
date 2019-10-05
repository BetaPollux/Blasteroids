#ifndef _ASTEROID_H
#define _ASTEROID_H

#include "boundingbox.h"

typedef struct AsteroidStruct Asteroid;

int Asteroid_Create(Asteroid **asteroid, float x, float y);

void Asteroid_Destroy(Asteroid *asteroid);

void Asteroid_Update(Asteroid *asteroid);

void Asteroid_Draw(const Asteroid *asteroid);

void Asteroid_GetBoundingBox(const Asteroid *asteroid, BoundingBox_t *out);

#endif // _ASTEROID_H
