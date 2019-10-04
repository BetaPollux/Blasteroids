#ifndef _SPACESHIP_H
#define _SPACESHIP_H

#include <allegro5/allegro.h>

typedef struct SpaceshipStruct Spaceship;

int Spaceship_Create(Spaceship **ship);

void Spaceship_Destroy(Spaceship *ship);

void Spaceship_Draw(const Spaceship *ship);

#endif // _SPACESHIP_H
