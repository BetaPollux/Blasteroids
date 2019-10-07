#include "spaceship.h"
#include "calc.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <assert.h>

#define SHIP_W              16.0f
#define SHIP_H              16.0f
#define SHIP_MAX_SPEED      200.0f

 struct SpaceshipStruct {
    float sx;
    float sy;
    float heading;
    float speed;
    int gone;
    ALLEGRO_COLOR color;
};

int Spaceship_Create(Spaceship **ship)
{
    assert(ship);

    Spaceship *newShip = malloc(sizeof(Spaceship));
    if (newShip)
    {
        newShip->sx = SCREEN_W / 2;
        newShip->sy = SCREEN_H / 2;
        newShip->heading = 0.0f;
        newShip->speed = 0.0;
        newShip->gone = 0;
        newShip->color = al_map_rgb(0, 255, 0);

        *ship = newShip;
    }
    
    return (newShip == NULL);
}

void Spaceship_Destroy(Spaceship *ship)
{
    assert(ship);

    free(ship);
}

void Spaceship_Accelerate(Spaceship *ship, float accel)
{
    assert(ship);

    ship->speed += accel;

    if (ship->speed < 0.0f)
    {
        ship->speed = 0.0f;
    }
    else if (ship->speed > PerSecond(SHIP_MAX_SPEED))
    {
        ship->speed = PerSecond(SHIP_MAX_SPEED);
    }
}

void Spaceship_Rotate(Spaceship *ship, float step)
{
    assert(ship);

    ship->heading += step;
}

void Spaceship_Update(Spaceship *ship)
{
    assert(ship);

    ship->sx += Displacement_X(ship->heading, ship->speed);
    ship->sy += Displacement_Y(ship->heading, ship->speed);

    WrapPosition(&ship->sx, &ship->sy);
}

void Spaceship_Draw(const Spaceship *ship)
{
    assert(ship);

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_rotate_transform(&transform, ship->heading);
    al_translate_transform(&transform, ship->sx, ship->sy);
    al_use_transform(&transform);

    al_draw_line(-8.0f, 9.0f, 0.0f, -11.0f, ship->color, 3.0f);
    al_draw_line(0.0f, -11.0f, 8.0f, 9.0f, ship->color, 3.0f);
    al_draw_line(-6.0f, 4.0f, -1.0f, 4.0f, ship->color, 3.0f);
    al_draw_line(6.0f, 4.0f, 1.0f, 4.0f, ship->color, 3.0f);
}

int Spaceship_Fire(const Spaceship *ship, Blast **blast)
{
    assert(ship);
    assert(blast);

    return Blast_Create(blast, ship->sx, ship->sy, ship->heading);
}

void Spaceship_GetBoundingBox(const Spaceship *ship, BoundingBox_t *out)
{
    assert(ship);
    assert(out);

#ifdef ROTATE_BOUNDING
    float width = 0.5f * SHIP_W;
    float height = 0.5f * SHIP_H;

    float adjWidth = width * fabs(cos(ship->heading)) + height * fabs(sin(ship->heading));
    float adjHeight = width * fabs(sin(ship->heading)) + height * fabs(cos(ship->heading));
#else
    float adjWidth = 0.5f * SHIP_W;
    float adjHeight = 0.5f * SHIP_H;
#endif

    out->left = ship->sx - adjWidth;
    out->right = ship->sx + adjWidth;
    out->top = ship->sy - adjHeight;
    out->bottom = ship->sy + adjHeight;
}
