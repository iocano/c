#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>

typedef struct point
{
    int x, y;
}point_t;

void exit_if(bool condition, const char *format, ...);

int main()
{
    int width = 640, height = 480;
    bool done = false, redraw = true;
    point_t position = {width / 2, height / 2};
    bool keys[ALLEGRO_KEY_MAX] = {false};

    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_SAMPLE *sample = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(width, height)), "Can't create display\n");
    exit_if(!(al_init_acodec_addon()), "Can't initialize acoded addon\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!al_install_audio(), "Can't install audio\n");
    exit_if(!al_reserve_samples(1), "Can't reserve samples\n");
    exit_if(!(sample = al_load_sample("step.ogg")), "Can't load step.ogg\n");

    timer = al_create_timer(1.0 / 60);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            keys[ev.keyboard.keycode] = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            keys[ev.keyboard.keycode] = false;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;

            point_t previous_position = position;
            if (keys[ALLEGRO_KEY_UP]) position.y -= 5;
            if (keys[ALLEGRO_KEY_DOWN]) position.y += 5;
            if (keys[ALLEGRO_KEY_LEFT]) position.x -= 5;
            if (keys[ALLEGRO_KEY_RIGHT]) position.x += 5;

            if (previous_position.x != position.x || previous_position.y != position.y)
            {
                al_play_sample(sample, 1, 0, 2, ALLEGRO_PLAYMODE_ONCE, NULL);
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            al_draw_filled_rectangle(position.x - 10,
                    position.y - 10,
                    position.x + 10,
                    position.y + 10,
                    al_map_rgb(255, 0, 255)
            );
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    al_destroy_sample(sample);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
    return 0;
}

void exit_if(bool condition, const char *format, ...)
{
    if(condition)
    {
        va_list argument_list;
        va_start(argument_list, format);
        vfprintf(stderr, format, argument_list);
        va_end(argument_list);
        exit(EXIT_FAILURE);
    }
}
