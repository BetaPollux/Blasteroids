#ifndef _ASTEROID_H
#define _ASTEROID_H

typedef struct AsteroidStruct Asteroid;

int Asteroid_Create(Asteroid **asteroid, float x, float y);

void Asteroid_Destroy(Asteroid *asteroid);

void Asteroid_Update(Asteroid *asteroid);

void Asteroid_Draw(const Asteroid *asteroid);

#endif // _ASTEROID_H
