#include "spaceship.h"
#include "asteroid.h"
#include "blast.h"
#include "calc.h"
#include <Containers/List.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_LIVES		4
#define NUM_ASTEROIDS 	5

typedef void (*ListFcn)(const void *item);

void GameOver(void)
{
	ALLEGRO_FONT *font = al_load_ttf_font("Freshman.ttf", 72, 0);

	if (font)
	{
		ALLEGRO_TRANSFORM transform;
    	al_identity_transform(&transform);
    	al_use_transform(&transform);

		al_draw_text(font, al_map_rgb(255, 0, 0), SCREEN_W / 2, SCREEN_H / 2, ALLEGRO_ALIGN_CENTRE, "Game Over!");
		al_flip_display();
		al_rest(10);
	}
	else
	{
		fprintf(stderr, "Failed to load font!\n");
		exit(EXIT_FAILURE);
	}
}

void ForAllItems(List_t list, ListFcn fcn)
{
	int num = List_Count(list);
	for (int idx = 0; idx < num; idx++)
	{
		const void *item = List_Item(list, idx);
		fcn(item);
	}	
}

bool IsShipCollided(Spaceship *ship, List_t asteroids)
{
	BoundingBox_t shipBox;
	Spaceship_GetBoundingBox(ship, &shipBox);

	#ifdef DRAW_BOUNDING
		BoundingBox_Draw(&shipBox);
	#endif

	// Check Ship Collision
	int numAsteroids = List_Count(asteroids);
	for (int astIdx = 0; astIdx < numAsteroids; astIdx++)
	{
		Asteroid *asteroid = List_Item(asteroids, astIdx);

		BoundingBox_t astBox;
		Asteroid_GetBoundingBox(asteroid, &astBox);
	
		#ifdef DRAW_BOUNDING
			BoundingBox_Draw(&astBox);
		#endif

		if (BoundingBox_Overlapped(&astBox, &shipBox))
		{
			return true;
		}
	}

	return false;
}

int CreateNewShip(List_t lives, Spaceship **ship)
{
	if (List_Count(lives) > 0)
	{
		Spaceship *usedLife = List_RemoveAt(lives, List_Count(lives) - 1);
		Spaceship_Destroy(usedLife);

		return Spaceship_Create(ship, SCREEN_W / 2, SCREEN_H / 2);
	}

	return 1;
}

void CheckBlastsCollision(List_t blasts, List_t asteroids)
{
	int destroyAstIndex = -1;
	int destroyBlastIndex = -1;
	int numAsteroids = List_Count(asteroids);
	for (int astIdx = 0; astIdx < numAsteroids; astIdx++)
	{
		Asteroid *asteroid = List_Item(asteroids, astIdx);

		BoundingBox_t astBox;
		Asteroid_GetBoundingBox(asteroid, &astBox);

		int numBlasts = List_Count(blasts);
		for (int blastIdx = 0; blastIdx < numBlasts; blastIdx++)
		{
			Blast *blast = List_Item(blasts, blastIdx);
			Point_t p;
			Blast_GetLocation(blast, &p);

			if (BoundingBox_IsInside(&astBox, &p))
			{
				destroyAstIndex = astIdx;
				destroyBlastIndex = blastIdx;

				Asteroid *ast1, *ast2;
				if (!Asteroid_SpawnSplit(asteroid, &ast1, &ast2))
				{
					List_Add(asteroids, ast1);
					List_Add(asteroids, ast2);
				}
			}
			else if (Blast_IsOffScreen(blast))
			{
				destroyBlastIndex = blastIdx;
			}
		}
	}

	if (destroyAstIndex >= 0)
	{
		Asteroid *destAst = List_RemoveAt(asteroids, destroyAstIndex);
		Asteroid_Destroy(destAst);
	}

	if (destroyBlastIndex >= 0)
	{
		Blast *destBlast = List_RemoveAt(blasts, destroyBlastIndex);
		Blast_Destroy(destBlast);
	}
}

int main(int argc, char **argv) 
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
    bool redraw = true;

	(void)argc;
	(void)argv;

	srand(time(0));

	if (!al_init())
	{
		fprintf(stderr, "Failed to initialize allegro!\n");
        return EXIT_FAILURE;
	}

    if (!al_init_primitives_addon())
	{
		fprintf(stderr, "Failed to initialize primitive-addons!\n");
        return EXIT_FAILURE;
	}

	if (!al_init_font_addon() || !al_init_ttf_addon())
	{
		fprintf(stderr, "Failed to initialize font-addons!\n");
        return EXIT_FAILURE;
	}

    if(!al_install_keyboard()) {
        fprintf(stderr, "Failed to initialize the keyboard!\n");
        return EXIT_FAILURE;
    }

	timer = al_create_timer(1.0f / FPS);
	if (!timer)
	{
		fprintf(stderr, "Failed to create timer!\n");
		return EXIT_FAILURE;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display)
	{
		fprintf(stderr, "Failed to create display!\n");
		al_destroy_timer(timer);
		return EXIT_FAILURE;
	}

	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		fprintf(stderr, "Failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return EXIT_FAILURE;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

	al_start_timer(timer);

	List_t shipLives = List_Create();
	if (!shipLives)
	{
		fprintf(stderr, "Failed to create ship lives!\n");
	}

	for (int i = 0; i < NUM_LIVES; i++)
	{
		Spaceship *life;
		if (!Spaceship_Create(&life, 50.0f + i * 50.0f, 50.0f))
		{
			List_Add(shipLives, life);
		}
	}

	Spaceship *ship;
	if (CreateNewShip(shipLives, &ship))
	{
		fprintf(stderr, "Failed to create ship!\n");
	}

	List_t asteroids = List_Create();
	if (!asteroids)
	{
		fprintf(stderr, "Failed to create asteroid list!\n");
	}

	List_t blasts = List_Create();
	if (!blasts)
	{
		fprintf(stderr, "Failed to create blast list!\n");
	}

	for (int i = 0; i < NUM_ASTEROIDS; i++)
	{
		Asteroid *ast;
		if (Asteroid_Create(&ast, Random(0.0f, SCREEN_W), Random(0.0f, SCREEN_H)))
		{
			fprintf(stderr, "Failed to create asteroid!\n");
			break;
		}
		List_Add(asteroids, ast);
	}

	while(1)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			Spaceship_Update(ship);
			ForAllItems(blasts, (ListFcn)Blast_Update);
			ForAllItems(asteroids, (ListFcn)Asteroid_Update);

			if (IsShipCollided(ship, asteroids))
			{
				Spaceship_Destroy(ship);
				if (CreateNewShip(shipLives, &ship))
				{
					GameOver();
				}
			}
			CheckBlastsCollision(blasts, asteroids);

			redraw = true;
		}
        else if (ev.type == ALLEGRO_EVENT_KEY_CHAR)
        {
            switch (ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_UP:
					Spaceship_Accelerate(ship, PerSecond(15.0f));
                    break;

                case ALLEGRO_KEY_DOWN:
					Spaceship_Accelerate(ship, PerSecond(-15.0f));
                    break;

                case ALLEGRO_KEY_LEFT:
					Spaceship_Rotate(ship, ToRadians(PerSecond(-180.0f)));
                    break;

                case ALLEGRO_KEY_RIGHT:
					Spaceship_Rotate(ship, ToRadians(PerSecond(180.0f)));
                    break;
            }
        }
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
			switch (ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_SPACE:
				{
					Blast *newBlast;
					if (!Spaceship_Fire(ship, &newBlast))
					{
						List_Add(blasts, newBlast);
					}
					else
					{
						fprintf(stderr, "Failed to create blast!\n");
					}
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			printf("Closing!\n");
			break;
		}

        if(redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(0,0,0));

            Spaceship_Draw(ship);
			ForAllItems(shipLives, (ListFcn)Spaceship_Draw);
			ForAllItems(blasts, (ListFcn)Blast_Draw);
			ForAllItems(asteroids, (ListFcn)Asteroid_Draw);

            al_flip_display();
        }
	}

    Spaceship_Destroy(ship);

	int numAsteroids = List_Count(asteroids);
	while(numAsteroids--)
	{
		Asteroid *ast = List_RemoveAt(asteroids, numAsteroids);
		Asteroid_Destroy(ast);
	}
	List_Destroy(asteroids);

	int numBlasts = List_Count(blasts);
	while(numBlasts--)
	{
		Blast *blast = List_RemoveAt(blasts, numBlasts);
		Blast_Destroy(blast);
	}
	List_Destroy(blasts);

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return EXIT_SUCCESS;
}