#include "spaceship.h"
#include "asteroid.h"
#include "blast.h"
#include "calc.h"
#include <Containers/List.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdlib.h>

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

    Spaceship *ship;
    if (Spaceship_Create(&ship))
    {
        fprintf(stderr, "Failed to create spaceship!\n");
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

	for (int i = 0; i < 3; i++)
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

			Spaceship_Update(ship);
            Spaceship_Draw(ship);

			BoundingBox_t shipBox;
			Spaceship_GetBoundingBox(ship, &shipBox);

			#ifdef DRAW_BOUNDING
				BoundingBox_Draw(&shipBox);
			#endif

			int destroyAstIndex = -1;
			int destroyBlastIndex = -1;
			int numAsteroids = List_Count(asteroids);
			for (int astIdx = 0; astIdx < numAsteroids; astIdx++)
			{
				Asteroid *asteroid = List_Item(asteroids, astIdx);
				Asteroid_Update(asteroid);
				Asteroid_Draw(asteroid);

				BoundingBox_t astBox;
				Asteroid_GetBoundingBox(asteroid, &astBox);
			
				#ifdef DRAW_BOUNDING
					BoundingBox_Draw(&astBox);
				#endif

				if (BoundingBox_Overlapped(&astBox, &shipBox))
				{
					Spaceship_Destroy(ship);
					Spaceship_Create(&ship);
				}

				int numBlasts = List_Count(blasts);
				for (int blastIdx = 0; blastIdx < numBlasts; blastIdx++)
				{
					Blast *blast = List_Item(blasts, blastIdx);
					Blast_Update(blast);
					
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

						Asteroid *newAst;
						Asteroid_Create(&newAst, Random(0.0f, SCREEN_W), Random(0.0f, SCREEN_H));
						List_Add(asteroids, newAst);
					}
					else if (Blast_IsOffScreen(blast))
					{
						destroyBlastIndex = blastIdx;
					}
					else
					{
						Blast_Draw(blast);
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