#ifndef _BLAST_H
#define _BLAST_H

typedef struct BlastStruct Blast;

int Blast_Create(Blast **blast, float x, float y, float heading);

void Blast_Destroy(Blast *blast);

void Blast_Update(Blast *blast);

void Blast_Draw(const Blast *blast);

#endif // _BLAST_H
