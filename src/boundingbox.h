#ifndef _BOUNDINGBOX_H
#define _BOUNDINGBOX_H

#include <stdbool.h>

typedef struct PointStruct
{
    float x;
    float y;
} Point_t;

typedef struct BoundingBoxStruct
{
    float left;
    float top;
    float right;
    float bottom;
} BoundingBox_t;

void BoundingBox_InitFromSize(float left, float top, float width, float height, BoundingBox_t *out);

bool BoundingBox_Overlapped(const BoundingBox_t *a, const BoundingBox_t *b);

bool BoundingBox_IsInside(const BoundingBox_t *box, const Point_t *point);

void BoundingBox_Draw(const BoundingBox_t *box);

#endif // _BOUNDINGBOX_H