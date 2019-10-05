#include "spaceship.h"
#include "asteroid.h"
#include "blast.h"
#include "calc.h"
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
        return -1;
	}

    if (!al_init_primitives_addon())
	{
		fprintf(stderr, "Failed to initialize primitive-addons!\n");
        return -1;
	}

    if(!al_install_keyboard()) {
        fprintf(stderr, "Failed to initialize the keyboard!\n");
        return -1;
    }

	timer = al_create_timer(1.0f / FPS);
	if (!timer)
	{
		fprintf(stderr, "Failed to create timer!\n");
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display)
	{
		fprintf(stderr, "Failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue)
	{
		fprintf(stderr, "Failed to create event_queue!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
		return -1;
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

	Asteroid *asteroid;
	if (Asteroid_Create(&asteroid, Random(0.0f, SCREEN_W), Random(0.0f, SCREEN_H)))
    {
        fprintf(stderr, "Failed to create asteroid!\n");
    }

	Asteroid *ast1 = NULL;
	Asteroid *ast2 = NULL;
	Blast *blast = NULL;

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

                case ALLEGRO_KEY_SPACE:
					if (!blast && Spaceship_Fire(ship, &blast))
					{
						fprintf(stderr, "Failed to create blast!\n");
						blast = NULL;
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

			Asteroid_Update(asteroid);
			Asteroid_Draw(asteroid);

			BoundingBox_t astBox;
			Asteroid_GetBoundingBox(asteroid, &astBox);
			
			BoundingBox_t shipBox;
			Spaceship_GetBoundingBox(ship, &shipBox);
			
#ifdef DRAW_BOUNDING
			BoundingBox_Draw(&astBox);
			BoundingBox_Draw(&shipBox);
#endif

			if (BoundingBox_Overlapped(&astBox, &shipBox))
			{
				Spaceship_Destroy(ship);
				Spaceship_Create(&ship);
			}

			if (blast)
			{
				Blast_Update(blast);
				
				Point_t p;
				Blast_GetLocation(blast, &p);
				
				bool destroyBlast = false;

				if (BoundingBox_IsInside(&astBox, &p))
				{
					destroyBlast = true;
					Asteroid_SpawnSplit(asteroid, &ast1, &ast2);
					Asteroid_Destroy(asteroid);
					Asteroid_Create(&asteroid, Random(0.0f, SCREEN_W), Random(0.0f, SCREEN_H));
				}

				if (destroyBlast || Blast_IsOffScreen(blast))
				{
					Blast_Destroy(blast);
					blast = NULL;
				}
				else
				{
					Blast_Draw(blast);
				}
			}

			if (ast1)
			{
				Asteroid_Update(ast1);
				Asteroid_Draw(ast1);
			}

			if (ast2)
			{
				Asteroid_Update(ast2);
				Asteroid_Draw(ast2);
			}

            al_flip_display();
        }
	}

    Spaceship_Destroy(ship);
	Asteroid_Destroy(asteroid);
	if (blast)
	{
		Blast_Destroy(blast);
	}

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}