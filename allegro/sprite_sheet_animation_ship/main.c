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
    point_t frame_size;
    point_t position;
    int frame_current;
    int frame_count;
}animation_t;

void exit_if(bool condition, const char *format, ...);

int main()
{
    bool done = false;
    point_t window_size = {640, 480};

    animation_t animation =
    {
        .frame_size = {.x = 61, .y = 41},
        .position = {.x = 0, .y = window_size.y / 2},
        .frame_current = 0,
        .frame_count = 9
    };

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_BITMAP *image;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window_size.x, window_size.y)), "Can't create display\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");

    exit_if(!(image = al_load_bitmap("sprite_sheet.png")), "Can't load sprite_ship.png\n");
    al_convert_mask_to_alpha(image, al_map_rgb(106, 76, 48));

    timer = al_create_timer(1.0 / 60);

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
            if (al_get_timer_count(timer) % 5 == 0)
            {
                animation.frame_current++;
                if (animation.frame_current >= animation.frame_count)
                {
                    animation.frame_current = 0;
                }
            }
            animation.position.x += 5;
            if (animation.position.x >= window_size.x)
            {
                animation.position.x = 0 - animation.frame_size.x;
            }
        }

        al_draw_bitmap_region(
                image,
                animation.frame_current * animation.frame_size.x,   // image x offeset
                0,                                                  // image y offeset
                animation.frame_size.x,                             // region (frame) size on x
                animation.frame_size.y,                             // region (frame) size on y
                animation.position.x,
                animation.position.y,
                0
        );
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }
    al_destroy_bitmap(image);
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
