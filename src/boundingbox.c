#include "boundingbox.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <assert.h>

void BoundingBox_InitFromSize(float left, float top, float width, float height, BoundingBox_t *out)
{
    assert(out);

    out->left = left;
    out->top = top;
    out->right = left + width;
    out->bottom = top + height;
}

bool BoundingBox_Overlapped(const BoundingBox_t *a, const BoundingBox_t *b)
{
    assert(a);
    assert(b);

    bool inX = (a->right >= b->left) && (a->left <= b->right);
    bool inY = (a->bottom >= b->top) && (a->top <= b->bottom);

    return (inX && inY);
}

bool BoundingBox_IsInside(const BoundingBox_t *box, const Point_t *point)
{
    bool inX = (point->x >= box->left) && (point->x <= box->right);
    bool inY = (point->y >= box->top) && (point->y <= box->bottom);

    return (inX && inY);
}

void BoundingBox_Draw(const BoundingBox_t *box)
{
    assert(box);

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_use_transform(&transform);

    ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);

    al_draw_rectangle(box->left, box->top, box->right, box->bottom, color, 1.0f);
}
