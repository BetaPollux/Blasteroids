#ifndef _SPACESHIP_H
#define _SPACESHIP_H

#include "blast.h"
#include "boundingbox.h"

typedef struct SpaceshipStruct Spaceship;

int Spaceship_Create(Spaceship **ship);

void Spaceship_Destroy(Spaceship *ship);

void Spaceship_Accelerate(Spaceship *ship, float accel);

void Spaceship_Rotate(Spaceship *ship, float rot_velocity);

void Spaceship_Update(Spaceship *ship);

void Spaceship_Draw(const Spaceship *ship);

int Spaceship_Fire(const Spaceship *ship, Blast **blast);

void Spaceship_GetBoundingBox(const Spaceship *ship, BoundingBox_t *out);

#endif // _SPACESHIP_H
