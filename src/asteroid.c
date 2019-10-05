#include "asteroid.h"
#include "calc.h"
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#define ASTEROID_W          40.0f
#define ASTEROID_H          40.0f
#define ASTEROID_MIN_SCALE  0.25f
#define ASTEROID_MAX_SCALE  2.0f

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

int createAsteroid(Asteroid **asteroid, float x, float y, float heading, float speed, float scale)
{
    assert(asteroid);

    if (scale < ASTEROID_MIN_SCALE)
    {
        return 1;
    }

    Asteroid *newAsteroid = malloc(sizeof(Asteroid));
    if (newAsteroid)
    {
        newAsteroid->sx = x;
        newAsteroid->sy = y;
        newAsteroid->heading = heading;
        newAsteroid->twist = Random(0.0f, ToRadians(360.0f));
        newAsteroid->speed = speed;
        newAsteroid->rot_velocity = PerSecond(Random(ToRadians(5.0f), ToRadians(30.0f)));
        newAsteroid->scale = scale;
        newAsteroid->gone = 0;
        newAsteroid->color = al_map_rgb(255, 255, 255);

        *asteroid = newAsteroid;
    }

    return (newAsteroid == NULL);
}

int Asteroid_Create(Asteroid **asteroid, float x, float y)
{
    assert(asteroid);

    return createAsteroid(asteroid, x, y,
            Random(0.0f, ToRadians(360.0f)),
            PerSecond(Random(10.0f, 50.0f)),
            Random(ASTEROID_MIN_SCALE, ASTEROID_MAX_SCALE)
        );
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

    al_draw_line(-20.0f, 20.0f, -20.0f, 5.0f, asteroid->color, 2.0f);
    al_draw_line(-20.0f, 5.0f, -20.0f, -10.0f, asteroid->color, 2.0f);
    al_draw_line(-20.0f, -10.0f, -5.0f, -10.0f, asteroid->color, 2.0f);
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

void Asteroid_GetBoundingBox(const Asteroid *asteroid, BoundingBox_t *out)
{
    assert(asteroid);
    assert(out);

#ifdef ROTATE_BOUNDING
    float width = 0.5f * ASTEROID_W * asteroid->scale;
    float height = 0.5f * ASTEROID_H * asteroid->scale;

    float adjWidth = width * fabs(cos(asteroid->twist)) + height * fabs(sin(asteroid->twist));
    float adjHeight = width * fabs(sin(asteroid->twist)) + height * fabs(cos(asteroid->twist));
#else
    float adjWidth = 0.5f * ASTEROID_W * asteroid->scale;
    float adjHeight = 0.5f * ASTEROID_H * asteroid->scale;
#endif

    out->left = asteroid->sx - adjWidth;
    out->right = asteroid->sx + adjWidth;
    out->top = asteroid->sy - adjHeight;
    out->bottom = asteroid->sy + adjHeight;
}

int Asteroid_SpawnSplit(const Asteroid *asteroid, Asteroid **one, Asteroid **two)
{
    assert(asteroid);
    assert(one);
    assert(two);

    float newScale = asteroid->scale * 0.5f;
    float headingDelta = ToRadians(30.0f);

    *one = NULL;
    *two = NULL;

    if (createAsteroid(one, asteroid->sx, asteroid->sy, asteroid->heading + headingDelta, asteroid->speed, newScale))
    {
        return (*one == NULL);
    }

    if (createAsteroid(two, asteroid->sx, asteroid->sy, asteroid->heading - headingDelta, asteroid->speed, newScale))
    {
        Asteroid_Destroy(*one);
        return (*two == NULL);
    }

    return (*one == NULL) || (*two == NULL);
}
