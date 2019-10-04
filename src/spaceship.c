#include "spaceship.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <assert.h>

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
    newShip->sx = 300.0f;
    newShip->sy = 300.0f;
    newShip->heading = 0.0f;
    newShip->speed = 0.0;
    newShip->gone = 0;
    newShip->color = al_map_rgb(0, 255, 0);

    *ship = newShip;
    
    return (newShip == NULL);
}

void Spaceship_Destroy(Spaceship *ship)
{
    assert(ship);

    free(ship);
}

void Spaceship_Draw(const Spaceship *ship)
{
    assert(ship);

    al_draw_line(ship->sx - 8.0f, ship->sy + 9.0f, 
                ship->sx + 0.0f, ship->sy - 11.0f, ship->color, 3.0f);
    al_draw_line(ship->sx + 0.0f, ship->sy - 11.0f, 
                ship->sx + 8.0f, ship->sy  + 9.0f, ship->color, 3.0f);
    al_draw_line(ship->sx - 6.0f, ship->sy + 4.0f, 
                ship->sx - 1.0f, ship->sy + 4.0f, ship->color, 3.0f);
    al_draw_line(ship->sx + 6.0f, ship->sy + 4.0f,
                ship->sx + 1.0f, ship->sy + 4.0f, ship->color, 3.0f);
}
