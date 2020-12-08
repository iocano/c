#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

void exit_if(bool condition, const char *format, ...);

int main(void)
{
    int width = 640, height = 480;
    int pos_x = width / 2, pos_y = height / 2;
    bool done = false, redraw = true;
    float FPS = 60;
    bool keys[ALLEGRO_KEY_MAX] = {false};

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(width, height)), "Can't create display\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives addons\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");

    timer = al_create_timer(1.0 / (float)FPS);

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(255, 0, 255));
    al_flip_display();
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
            if (keys[ALLEGRO_KEY_ESCAPE] == true)
            {
                done = true;
            }
            pos_y -= keys[ALLEGRO_KEY_UP] * 10;
            pos_y += keys[ALLEGRO_KEY_DOWN] * 10;
            pos_x -= keys[ALLEGRO_KEY_LEFT] * 10;
            pos_x += keys[ALLEGRO_KEY_RIGHT] * 10;

        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(255, 0, 255));
            al_flip_display();
        }
    }
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display); //destroy our display object
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
