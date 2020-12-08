#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdint.h>
#include <stdio.h>
#include <error.h>
#include <string.h>

typedef struct point
{
    float x;
    float y;
}point_t;

void exit_if(bool condition, const char *format, ...);

int main()
{
    int width = 640, height = 480;
    bool done = false;

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(width, height)), "Can't create display\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");

    int current_frame = 0;
    int frame_count = 3;
    ALLEGRO_BITMAP *frames[frame_count];

    exit_if(!(frames[0] = al_load_bitmap("ship/ship_1.png")), "Can't load ship_1.png\n");
    exit_if(!(frames[1] = al_load_bitmap("ship/ship_2.png")), "Can't load ship_2.png\n");
    exit_if(!(frames[2] = al_load_bitmap("ship/ship_3.png")), "Can't load ship_3.png\n");

    point_t animation_position = { 0 - al_get_bitmap_width(frames[0]), height / 2 };

    timer = al_create_timer(1.0 / 60.0);
    event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
        {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            if(al_get_timer_count(timer) % 5 == 0)
            {
                current_frame++;
                if(current_frame >= frame_count)
                {
                    current_frame = 0;
                }
            }
            animation_position.x += 5;
            if (animation_position.x >= width )
            {
                animation_position.x = 0 - al_get_bitmap_width(frames[0]);
            }
        }
        al_draw_bitmap(frames[current_frame], animation_position.x, animation_position.y, 0);
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    for (int i = 0; i < frame_count; i++)
    {
        al_destroy_bitmap(frames[i]);
    }
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
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
