#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include "include/ship.h"
#include "include/bullet.h"

const object_coordinate_t window_size = {800,400};

const int NUM_BULLETS = 5;
const int NUM_COMETS = 10;

void exit_if(bool condition, const char *format, ...);

int main()
{
    bool is_done = false;
    bool redraw = true;
    bool is_game_over = false;
    const int FPS = 60;
    bool keys[ALLEGRO_KEY_MAX] = {false};

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font18 = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window_size.x, window_size.y)), "Can't create display\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives addon\n");
    exit_if(!al_init_font_addon(), "Can't initialize font addon\n");
    exit_if(!al_init_ttf_addon(), "Can't initialize ttf addon\n");
    exit_if(!al_install_keyboard(), "Can't install keyborad\n");
    exit_if(!(font18 = al_load_font("DejaVuSans.ttf", 18, 0)), "Can't load font\n");
    exit_if(!(timer = al_create_timer(1.0 / FPS)), "Can't create timer\n");
    exit_if(!(event_queue = al_create_event_queue()), "Can't create event queue\n");

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    srand(time(NULL));

    object_coordinate_t position = {20, window_size.y / 2};
    object_limit_t ship_limit = {{0, 0}, {300, window_size.y}};
    ship_t *ship = ship_init(position, ship_limit, display);

    object_limit_t bullet_limit = {{0, 0}, {window_size.x, window_size.y}};
    bullet_t **bullets = bullet_array_init(NUM_BULLETS, bullet_limit);

    object_limit_t comet_limit = {{0, 0}, {window_size.x, window_size.y}};
    comet_t **comets = comet_array_init(NUM_COMETS, comet_limit);

    al_clear_to_color(al_map_rgb(25, 29, 30));
    ship_draw(ship);
    al_flip_display();

    al_start_timer(timer);
    while (!is_done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            if(keys[ALLEGRO_KEY_ESCAPE] == true)
            {
                is_done = true;
            }

            if(keys[ALLEGRO_KEY_UP] == true)
            {
                ship_move_up(ship);
            }
            else if(keys[ALLEGRO_KEY_DOWN] == true)
            {
                ship_move_down(ship);
            }

            if(keys[ALLEGRO_KEY_LEFT] == true)
            {
                ship_move_left(ship);
            }
            else if(keys[ALLEGRO_KEY_RIGHT] == true)
            {
                ship_move_right(ship);
            }

            if(keys[ALLEGRO_KEY_SPACE] == true)
            {
                bullet_array_fire((void*)bullets, NUM_BULLETS, ship);
                keys[ALLEGRO_KEY_SPACE] = false;
            }

            if (!is_game_over)
            {
                bullet_array_update(bullets, NUM_BULLETS);
                comet_array_start(comets, NUM_COMETS);
                comet_array_update(comets, NUM_COMETS);
                bullet_array_collide(bullets, NUM_BULLETS, comets, NUM_COMETS, ship);
                comet_array_collide(comets, NUM_COMETS, ship);
            }

            if (ship->lives <= 0)
            {
                is_game_over = true;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            is_done = true;
            break;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            keys[ev.keyboard.keycode] = true;
        }
        else if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            keys[ev.keyboard.keycode] = false;
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;

            if (!is_game_over)
            {
                ship_draw(ship);
                bullet_array_draw(bullets, NUM_BULLETS);
                comet_array_draw(comets, NUM_COMETS);
                al_draw_textf(font18, al_map_rgb(255, 0,255),5, 5, 0, "Player has %i lives left. Player has destroyed %i objects", ship->lives, ship->score);
            }
            else
            {
                al_draw_textf(font18, al_map_rgb(0, 255, 255), window_size.x / 2, window_size.y / 2, ALLEGRO_ALIGN_CENTRE, "Game Over. Final Score: %i", ship->score);
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(25, 29, 30));
        }
    }

    ship_destroy(ship);
    bullet_array_destroy(bullets, NUM_BULLETS);
    commet_array_free(comets, NUM_COMETS);
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
