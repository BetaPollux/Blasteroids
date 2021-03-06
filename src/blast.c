#include "blast.h"
#include "calc.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>

#define BLAST_SPEED     400.0f
#define BLAST_DAMAGE    10.0f

struct BlastStruct {
    float sx;
    float sy;
    float heading;
    float speed;
    float damage;
    int gone;
    ALLEGRO_COLOR color;
};

int Blast_Create(Blast **blast, float x, float y, float heading)
{
    assert(blast);

    Blast *newBlast = malloc(sizeof(Blast));
    if (newBlast)
    {
        newBlast->sx = x;
        newBlast->sy = y;
        newBlast->heading = heading;
        newBlast->speed = PerSecond(BLAST_SPEED);
        newBlast->damage = BLAST_DAMAGE;
        newBlast->gone = 0;
        newBlast->color = al_map_rgb(255, 0, 0);

        *blast = newBlast;
    }
    
    return (newBlast == NULL);
}

int Blast_Configure(Blast *blast, float damageMod, float speedMod)
{
    assert(blast);

    blast->damage *= damageMod;
    blast->speed *= speedMod;

    return 0;
}

void Blast_Destroy(Blast *blast)
{
    assert(blast);

    free(blast);
}

void Blast_Update(Blast *blast)
{
    assert(blast);

    blast->sx += Displacement_X(blast->heading, blast->speed);
    blast->sy += Displacement_Y(blast->heading, blast->speed);
}

void Blast_Draw(const Blast *blast)
{
    assert(blast);

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, blast->heading);
    al_translate_transform(&transform, blast->sx, blast->sy);
    al_use_transform(&transform);

    float thickness = blast->damage / BLAST_DAMAGE;
    al_draw_line(0.0f, 6.0f,  0.0f, 4.0f, blast->color, thickness);
    al_draw_line(0.0f, 2.0f, 0.0f, -2.0f, blast->color, thickness);
    al_draw_line(0.0f, -4.0f, 0.0f, -6.0f, blast->color, thickness);
}

bool Blast_IsOffScreen(const Blast *blast)
{
    assert(blast);
    
    return IsOffScreen(blast->sx, blast->sy);
}

void Blast_GetLocation(const Blast *blast, Point_t *out)
{
    assert(blast);
    assert(out);

    out->x = blast->sx;
    out->y = blast->sy;
}

float Blast_GetDamage(const Blast *blast)
{
    assert(blast);

    return blast->damage;
}
