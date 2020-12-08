#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "ppcd.h"

typedef struct point
{
    float x, y;
}point_t;

typedef struct area
{
    float width, height;
}area_t;

typedef struct object
{
    point_t position;
    point_t speed;
    point_t direction;
    area_t area;
    ALLEGRO_BITMAP *image;
    mask_t *mask;
} object_t;

void exit_if(bool condition, const char *format, ...);

int main()
{
    area_t window = {800, 600};
    bool done = false, render = false, bound = false, collision = false;
    object_t object_1, object_2;
    bool keys[ALLEGRO_KEY_MAX] = {false};

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font18 = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window.width, window.height)), "Can't create display\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");
    exit_if(!al_init_font_addon(), "Can't initialize font addon\n");
    exit_if(!al_init_ttf_addon(), "Can't initialize ttf addon\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives addon\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!(font18 = al_load_font("resources/arial.ttf", 18, 0)), "Can't load resources/arial.ttf\n");

    exit_if(!(object_1.image = al_load_bitmap("resources/object.png")), "Can't load resources/object.png");
    object_1.area.width = al_get_bitmap_width(object_1.image);
    object_1.area.height = al_get_bitmap_height(object_1.image);
    object_1.position.x = 0;
    object_1.position.y = 0;
    object_1.mask = mask_create(object_1.image, object_1.area.width, object_1.area.height);

    exit_if(!(object_2.image = al_load_bitmap("resources/object.png")), "Can't load resources/object.png");
    object_2.area.width = al_get_bitmap_width(object_2.image);
    object_2.area.height = al_get_bitmap_height(object_2.image);
    object_2.position.x = window.width / 2;
    object_2.position.y = window.height / 2;
    object_2.mask = mask_create(object_2.image, object_2.area.width, object_2.area.height);

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
            if(keys[ALLEGRO_KEY_ESCAPE])
            {
                done = true;
            }
            render = true;
            object_1.position.y -= 5 * keys[ALLEGRO_KEY_UP];
            object_1.position.y += 5 * keys[ALLEGRO_KEY_DOWN];
            object_1.position.x -= 5 * keys[ALLEGRO_KEY_LEFT];
            object_1.position.x += 5 * keys[ALLEGRO_KEY_RIGHT];
            bound = keys[ALLEGRO_KEY_SPACE];

            int x_offset = object_1.position.x - object_2.position.x;
            int y_offset = object_1.position.y - object_2.position.y;
            if (mask_collide(object_1.mask, object_2.mask, x_offset, y_offset))
            {
                collision = true;
            }
            else
            {
                collision = false;
            }
        }

        if (render && al_is_event_queue_empty(event_queue))
        {
            render = false;
            al_draw_bitmap(object_2.image, object_2.position.x - object_2.area.width / 2, object_2.position.y - object_2.area.height / 2, 0);
            al_draw_bitmap(object_1.image, object_1.position.x - object_1.area.width / 2, object_1.position.y - object_1.area.height / 2, 0);
            if (bound)
            {
                mask_draw(object_1.mask, object_1.position.x, object_1.position.y, display);
                mask_draw(object_2.mask, object_2.position.x, object_2.position.y, display);
                al_draw_text(font18, al_map_rgb(255, 0, 0), 0, 20, 0, "Bound");
            }

            if (collision)
            {
                al_draw_textf(font18, al_map_rgb(255, 255, 255), window.width / 2, 20, ALLEGRO_ALIGN_CENTRE, "Collision! at: %.0f, %.0f", object_1.position.x , object_1.position.y);
                al_draw_filled_circle(object_1.position.x, object_1.position.y , 3, al_map_rgb(255, 255,255));
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    mask_destroy(object_1.mask);
    mask_destroy(object_2.mask);
    al_destroy_bitmap(object_1.image);
    al_destroy_bitmap(object_2.image);
    al_destroy_font(font18);
    al_destroy_timer(timer);
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
