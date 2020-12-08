#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

void exit_if(bool condition, const char *format, ...);

int main()
{
    int width = 800, height = 600;
    bool keys[ALLEGRO_KEY_MAX] = {false};
    bool done = false, render = false;

    int offset_x = 0, offset_y = 0;
    int map_columns = 10;
    int map_size = 100;
    int tile_size = 128;

    int map[] = {
        0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
        0, 1, 0, 0, 1, 1, 0, 0, 1, 0,
        1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
        0, 1, 0, 0, 1, 1, 0, 0, 1, 0,
        0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
        0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
        0, 1, 0, 0, 1, 1, 0, 0, 1, 0,
        1, 0, 0, 0, 1, 1, 0, 0, 0, 1,
        0, 1, 0, 0, 1, 1, 0, 0, 1, 0,
        0, 0, 1, 1, 0, 0, 1, 1, 0, 0
    };

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *tile_background = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(width, height)), "Can't create display\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!(tile_background = al_load_bitmap("background.png")), "Can't load background.png\n");

    timer = al_create_timer(1.0 / 60);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

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
        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            render = true;
            offset_x -= keys[ALLEGRO_KEY_RIGHT] * 5;
            offset_x += keys[ALLEGRO_KEY_LEFT] * 5;
            offset_y -= keys[ALLEGRO_KEY_DOWN] * 5;
            offset_y += keys[ALLEGRO_KEY_UP] * 5;

            if(keys[ALLEGRO_KEY_ESCAPE])
            {
                done = true;
            }
        }

        if (render && al_is_event_queue_empty(event_queue))
        {
            render = false;
            for (int i = 0; i < map_size; i++)
            {
                int dx = offset_x + tile_size * (i % map_columns);
                int dy = offset_y + tile_size * (i / map_columns);
                al_draw_bitmap_region( tile_background, tile_size * map[i], 0, tile_size, tile_size, dx, dy, 0);
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    al_destroy_display(display);
    al_destroy_bitmap(tile_background);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
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
