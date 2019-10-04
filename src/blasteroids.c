#include "spaceship.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

static const float FPS = 30.0f;
static const int SCREEN_W = 640;
static const int SCREEN_H = 480;

int main(int argc, char **argv) 
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
    bool redraw = true;

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

	unsigned char cnt = 0;

    Spaceship *ship;
    if (Spaceship_Create(&ship))
    {
        fprintf(stderr, "Failed to create spaceship!\n");
		al_destroy_display(display);
		al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
		return -1;
    }

	while(1)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
            printf(".");

            if (cnt++ == 20)
                printf("\n"), cnt = 0;
		}
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch (ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_UP:
                    break;

                case ALLEGRO_KEY_DOWN:
                    break;

                case ALLEGRO_KEY_LEFT:
                    break;

                case ALLEGRO_KEY_RIGHT:
                    break;

                case ALLEGRO_KEY_SPACE:
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

            al_flip_display();
        }
	}

    Spaceship_Destroy(ship);

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}