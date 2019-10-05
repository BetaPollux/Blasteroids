#include "asteroid.h"
#include "calc.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

struct AsteroidStruct {
    float sx;
    float sy;
    float heading;
    float twist;
    float speed;
    float rot_velocity;
    float scale;
    int gone;
    ALLEGRO_COLOR color;
};

int Asteroid_Create(Asteroid **asteroid, float x, float y)
{
    assert(asteroid);

    Asteroid *newAsteroid = malloc(sizeof(Asteroid));
    if (newAsteroid)
    {
        newAsteroid->sx = x;
        newAsteroid->sy = y;
        newAsteroid->heading = Random(0.0f, ToRadians(360.0f));
        newAsteroid->twist = Random(0.0f, ToRadians(360.0f));
        newAsteroid->speed = PerSecond(Random(10.0f, 50.0f));
        newAsteroid->rot_velocity = PerSecond(Random(ToRadians(5.0f), ToRadians(30.0f)));
        newAsteroid->scale = Random(0.5f, 2.0f);
        newAsteroid->gone = 0;
        newAsteroid->color = al_map_rgb(255, 255, 255);

        *asteroid = newAsteroid;
    }
    
    return (newAsteroid == NULL);
}

void Asteroid_Destroy(Asteroid *asteroid)
{
    assert(asteroid);

    free(asteroid);
}

void Asteroid_Update(Asteroid *asteroid)
{
    assert(asteroid);

    asteroid->sx += Displacement_X(asteroid->heading, asteroid->speed);
    asteroid->sy += Displacement_Y(asteroid->heading, asteroid->speed);
    asteroid->twist += asteroid->rot_velocity;

    WrapPosition(&asteroid->sx, &asteroid->sy);
}

void Asteroid_Draw(const Asteroid *asteroid)
{
    assert(asteroid);

    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, asteroid->scale, asteroid->scale);
    al_rotate_transform(&transform, asteroid->twist);
    al_translate_transform(&transform, asteroid->sx, asteroid->sy);
    al_use_transform(&transform);

    al_draw_line(-20.0f, 20.0f, -25.0f, 5.0f, asteroid->color, 2.0f);
    al_draw_line(-25.0f, 5.0f, -25.0f, -10.0f, asteroid->color, 2.0f);
    al_draw_line(-25.0f, -10.0f, -5.0f, -10.0f, asteroid->color, 2.0f);
    al_draw_line(-5.0f, -10.0f, -10.0f, -20.0f, asteroid->color, 2.0f);
    al_draw_line(-10.0f, -20.0f, 5.0f, -20.0f, asteroid->color, 2.0f);
    al_draw_line(5.0f, -20.0f, 20.0f, -10.0f, asteroid->color, 2.0f);
    al_draw_line(20.0f, -10.0f, 20.0f, -5.0f, asteroid->color, 2.0f);
    al_draw_line(20.0f, -5.0f, 0.0f, 0.0f, asteroid->color, 2.0f);
    al_draw_line(0.0f, 0.0f, 20.0f, 10.0f, asteroid->color, 2.0f);
    al_draw_line(20.0f, 10.0f, 10.0f, 20.0f, asteroid->color, 2.0f);
    al_draw_line(10.0f, 20.0f, 0.0f, 15.0f, asteroid->color, 2.0f);
    al_draw_line(0.0f, 15.0f, -20.0f, 20.0f, asteroid->color, 2.0f);
}
