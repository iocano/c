#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <math.h>
#include "ppcd.h"

typedef enum collision_detection{BOUNDING_BOX, DISTANCE_BASED, PIXEL_PERFECT}collision_detection_t;

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
    ALLEGRO_BITMAP *image;
    // Bouding box collision detection vars
    area_t bound;
    // Distance based collision detection var
    int r;
    area_t area;
    // Pixel perfect collision detection var
    mask_t *mask;
} object_t;

void exit_if(bool condition, const char *format, ...);

int main()
{
    collision_detection_t collision_detection = BOUNDING_BOX;
    area_t window = {800, 600};
    bool keys[ALLEGRO_KEY_MAX] = {false};
    bool done = false, render = false, bound = false, collision = false;
    float game_time = 0;
    int frames = 0;
    int game_fps = 0;

    const int NUM_SPRITES = 9;
    object_t objects[NUM_SPRITES];

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font_18 = NULL;
    ALLEGRO_BITMAP *image = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window.width, window.height)), "Can't create display\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");
    exit_if(!al_init_font_addon(), "Can't initialize font addon\n");
    exit_if(!al_init_ttf_addon(), "Can't initialize ttf addon\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!(font_18 = al_load_font("resources/arial.ttf", 18, 0)), "Can't load arial.ttf\n");
    exit_if(!(image = al_load_bitmap("resources/object.png")), "Can't load object.png\n");

    for (int i = 0; i < NUM_SPRITES; i++)
    {
        objects[i].image = image;
        objects[i].position.x = 100 + (i % 3) * 110;
        objects[i].position.y = 100 + (i / 3) * 110;
        objects[i].area.width  = al_get_bitmap_width(objects[i].image);
        objects[i].area.height  = al_get_bitmap_height(objects[i].image);
        objects[i].bound.width = objects[i].area.width / 2 - 10;
        objects[i].bound.height = objects[i].area.height / 2 - 10;
        objects[i].r = objects[i].area.width / 2 - 10;
        objects[i].mask = mask_create(objects[i].image, objects[i].area.width, objects[i].area.height);
    }

    timer = al_create_timer(1.0 / 60);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    game_time = al_current_time();
    while(!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            keys[ev.keyboard.keycode] = true;
            // arrow, space k1,2,3
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            keys[ev.keyboard.keycode] = false;
        }
        else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            if(keys[ALLEGRO_KEY_ESCAPE])
            {
                done = true;
            }
            render = true;
            frames++;
            if(al_current_time() - game_time >= 1)
            {
                game_time = al_current_time();
                game_fps = frames;
                frames = 0;
            }
            collision = false;
            objects[0].position.y -= 5 * keys[ALLEGRO_KEY_UP];
            objects[0].position.y += 5 * keys[ALLEGRO_KEY_DOWN];
            objects[0].position.x -= 5 * keys[ALLEGRO_KEY_LEFT];
            objects[0].position.x += 5 * keys[ALLEGRO_KEY_RIGHT];

            if (keys[ALLEGRO_KEY_1])
            {
                collision_detection = BOUNDING_BOX;
            }
            else if (keys[ALLEGRO_KEY_2])
            {
                collision_detection = DISTANCE_BASED;
            }
            else if (keys[ALLEGRO_KEY_3])
            {
                collision_detection = PIXEL_PERFECT;
            }

            bound = keys[ALLEGRO_KEY_SPACE];

            if(collision_detection == BOUNDING_BOX)
            {
                for(int i = 1; i < NUM_SPRITES; i++)
                {
                    bool object0_inside_left_objectn_bound = objects[0].position.x + objects[0].bound.width > objects[i].position.x - objects[i].bound.width;
                    bool object0_inside_right_objectn_bound = objects[0].position.x - objects[0].bound.width < objects[i].position.x + objects[i].bound.width;
                    bool object0_inside_lower_objectn_bound = objects[0].position.y + objects[0].bound.height > objects[i].position.y - objects[i].bound.height;
                    bool object0_inside_upper_objectn_bound = objects[0].position.y - objects[0].bound.height < objects[i].position.y + objects[i].bound.height;
                    if(object0_inside_left_objectn_bound && object0_inside_right_objectn_bound && object0_inside_lower_objectn_bound && object0_inside_upper_objectn_bound)
                    {
                        collision = true;
                    }
                }
            }
            else if (collision_detection == DISTANCE_BASED)
            {
                for (int i = 1; i < NUM_SPRITES; i++)
                {
                    float distance = sqrt(pow(objects[0].position.x - objects[i].position.x, 2) + pow(objects[0].position.y - objects[i].position.y, 2));
                    if(distance < objects[0].r + objects[i].r)
                    {
                        collision = true;
                        break;
                    }
                }
            }
            else
            {
                for (int i = 1; i < NUM_SPRITES; i++)
                {
                    if(mask_collide(objects[0].mask, objects[i].mask, objects[0].position.x - objects[i].position.x, objects[0].position.y - objects[i].position.y))
                    {
                        collision = true;
                        break;
                    }
                }
            }
        }

        if(render && al_is_event_queue_empty(event_queue))
        {
            render = false;
            al_draw_textf(font_18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", game_fps);
            for (int i = 0; i < NUM_SPRITES; i++)
            {
                al_draw_bitmap(objects[i].image, objects[i].position.x - objects[i].area.width / 2, objects[i].position.y - objects[i].area.height / 2, 0);
            }

            if(bound)
            {
                if(collision_detection == BOUNDING_BOX)
                {
                    for (int i = 0; i < NUM_SPRITES; i++)
                    {
                        al_draw_filled_rectangle(objects[i].position.x - objects[i].bound.width,
                                objects[i].position.y - objects[i].bound.height,
                                objects[i].position.x + objects[i].bound.width,
                                objects[i].position.y + objects[i].bound.height,
                                al_map_rgba_f(0, .3, .3, .1));
                    }
                }
                else if (collision_detection == DISTANCE_BASED)
                {
                    for (int i = 0; i < NUM_SPRITES; i++)
                    {
                        al_draw_filled_circle(objects[i].position.x, objects[i].position.y, objects[i].r, al_map_rgba_f(0, .3, .3, .1));
                    }
                }
                else
                {
                    for (int i = 0; i < NUM_SPRITES; i++)
                    {
                        mask_draw(objects[i].mask, objects[i].position.x, objects[i].position.y, display);
                    }
                }
            }
            if(collision)
            {
                al_draw_text(font_18, al_map_rgb(255, 255, 255), window.width / 2, 20, ALLEGRO_ALIGN_CENTRE, "Collision!");
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }

    }
    for(int i = 0; i < NUM_SPRITES; i++)
    {
        mask_destroy(objects[i].mask);
    }
    al_destroy_bitmap(image);
    al_destroy_font(font_18);
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
