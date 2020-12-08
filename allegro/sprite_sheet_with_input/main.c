#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

typedef struct point
{
    float x;
    float y;
}point_t;

typedef struct animation
{
    point_t position;
    point_t speed;
    point_t movement_direction;
    point_t frame_direction;
    point_t frame_size;
    int flip_vertical;
    int frame_count;
    int frame_current;
    ALLEGRO_BITMAP *sprite_sheet;
} animation_t;

void exit_if(bool condition, const char *format, ...);

int main()
{
    int done = false;
    point_t window_size = {640, 480};
    animation_t ship =
    {
        .position = { window_size.x / 2, window_size.y /2 },
        .speed = {5, 1},
        .movement_direction = {1, 1},
        .frame_size = {61, 41},
        .flip_vertical = 0,
        .frame_count = 9,
        .frame_current= 0,
    };

    bool keys[ALLEGRO_KEY_MAX] = {false};

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window_size.x, window_size.y)), "Can't create display\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!(ship.sprite_sheet = al_load_bitmap("sprite_sheet.png")), "Can't load sprite_sheet.png\n");

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
            if (keys[ALLEGRO_KEY_ESCAPE])
            {
                done = true;
            }

            if (keys[ALLEGRO_KEY_LEFT])
            {
                ship.frame_direction.x = 1;
                ship.movement_direction.x = -1;
                ship.flip_vertical = ALLEGRO_FLIP_HORIZONTAL;
            }
            else if (keys[ALLEGRO_KEY_RIGHT])
            {
                ship.frame_direction.x = -1;
                ship.movement_direction.x = 1;
                ship.flip_vertical = 0;
            }

            if (al_get_timer_count(timer) % 5 == 0)
            {
                ship.frame_current += ship.frame_direction.x;
                if(ship.frame_current >= ship.frame_count)
                {
                    ship.frame_current = 0;
                }
                else if(ship.frame_current <= 0)
                {
                    ship.frame_current = ship.frame_count - 1;
                }
            }

            ship.position.x += ship.speed.x * ship.movement_direction.x;
            if(ship.position.x >= window_size.x)
            {
                ship.position.x = 0 - ship.frame_size.x;
            }
            else if(ship.position.x < 0 - ship.frame_size.x)
            {
                ship.position.x = window_size.y;
            }
        }

        al_draw_bitmap_region(ship.sprite_sheet, ship.frame_current * ship.frame_size.x , 0, ship.frame_size.x, ship.frame_size.y, ship.position.x, ship.position.y, ship.flip_vertical);
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    al_destroy_bitmap(ship.sprite_sheet);
    al_destroy_event_queue(event_queue);
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
