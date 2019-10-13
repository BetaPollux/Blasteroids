#ifndef _ASTEROID_H
#define _ASTEROID_H

#include "boundingbox.h"

typedef struct AsteroidStruct Asteroid;

int Asteroid_Create(Asteroid **asteroid, float x, float y);

void Asteroid_Destroy(Asteroid *asteroid);

void Asteroid_Update(Asteroid *asteroid);

void Asteroid_Draw(const Asteroid *asteroid);

void Asteroid_GetBoundingBox(const Asteroid *asteroid, BoundingBox_t *out);

int Asteroid_SpawnSplit(const Asteroid *asteroid, Asteroid **one, Asteroid **two);

void Asteroid_Hit(Asteroid *asteroid, float damage);

bool Asteroid_IsDead(const Asteroid *asteroid);

int Asteroid_GetScore(const Asteroid *asteroid);

#endif // _ASTEROID_H
