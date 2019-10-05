#ifndef _BLAST_H
#define _BLAST_H

#include "boundingbox.h"
#include <stdbool.h>

typedef struct BlastStruct Blast;

int Blast_Create(Blast **blast, float x, float y, float heading);

void Blast_Destroy(Blast *blast);

void Blast_Update(Blast *blast);

void Blast_Draw(const Blast *blast);

bool Blast_IsOffScreen(const Blast *blast);

void Blast_GetLocation(const Blast *blast, Point_t *out);

#endif // _BLAST_H
