#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include "include/object.h"
#include "include/ship.h"
#include "include/bullet.h"
#include "include/comet.h"
#include "include/explosion.h"

const int BULLET_COUNT = 5;
const int COMET_COUNT = 10;
const int EXPLOSION_COUNT = 5;

void exit_if(bool condition, const char *format, ...);

int main()
{
    object_size_t window = {800, 400};
    bool done = false, redraw = true, is_game_over = false;
    bool keys[ALLEGRO_KEY_MAX] = {false};

    ship_t ship;
    bullet_t bullets[BULLET_COUNT];
    comet_t comets[COMET_COUNT];
    explosion_t explosions[EXPLOSION_COUNT];

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font_18 = NULL;
    ALLEGRO_BITMAP *ship_sprite_sheet;
    ALLEGRO_BITMAP *comet_sprite_sheet;
    ALLEGRO_BITMAP *explosion_sprite_sheet;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window.width, window.height)), "Can't create display\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives addon\n");
    exit_if(!al_init_font_addon(), "Can't initialize font addon\n");
    exit_if(!al_init_ttf_addon(), "Can't initialize ttf addon");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");

    exit_if(!(font_18 = al_load_font("resources/DejaVuSans.ttf", 18, 0)), "Can't load font\n");
    exit_if(!(ship_sprite_sheet = al_load_bitmap("resources/ship.png")), "Can't load ship.png\n");
    // remove background color
    al_convert_mask_to_alpha(ship_sprite_sheet, al_map_rgb(255, 0, 255));
    exit_if(!(comet_sprite_sheet = al_load_bitmap("resources/comet.png")), "Can't load comet.png\n");
    exit_if(!(explosion_sprite_sheet = al_load_bitmap("resources/explosion.png")), "Can't load explosion.png\n");

    timer = al_create_timer(1.0 / 60.0);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    srand(time(NULL));

    object_limit_t ship_movement_limit = {{0, 0}, {300, window.height}};
    ship_init(&ship, ship_sprite_sheet, ship_movement_limit);
    object_limit_t bullet_movement_limit = {{0, 0}, {window.width, window.height}};
    bullet_init(bullets, BULLET_COUNT, bullet_movement_limit);
    object_limit_t comet_movement_limit = {{0, 0}, {window.width, window.height}};
    comet_init(comets, COMET_COUNT, comet_sprite_sheet, comet_movement_limit);
    explosion_init(explosions, EXPLOSION_COUNT, explosion_sprite_sheet);

    al_start_timer(timer);
    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_TIMER)
        {

            redraw = true;
            if (keys[ALLEGRO_KEY_ESCAPE])
            {
                done = true;
            }

            if (!is_game_over)
            {
                if (keys[ALLEGRO_KEY_UP])
                {
                    ship_move_up(&ship);
                }
                else if (keys[ALLEGRO_KEY_DOWN])
                {
                    ship_move_down(&ship);
                }
                else
                {
                    ship_reset_animation(&ship);
                }

                if (keys[ALLEGRO_KEY_LEFT])
                {
                    ship_move_left(&ship);
                }
                else if (keys[ALLEGRO_KEY_RIGHT])
                {
                    ship_move_right(&ship);
                }

                if (keys[ALLEGRO_KEY_SPACE])
                {
                    bullet_fire(bullets, BULLET_COUNT, &ship);
                    keys[ALLEGRO_KEY_SPACE] = false;
                }
                explosion_update(explosions, EXPLOSION_COUNT, al_get_timer_count(timer));
                bullet_update(bullets, BULLET_COUNT);
                comet_start(comets, COMET_COUNT);
                comet_update(comets, COMET_COUNT, al_get_timer_count(timer));
                bullet_collide(bullets, BULLET_COUNT, comets, COMET_COUNT, &ship, explosions, EXPLOSION_COUNT);
                comet_collide(comets, COMET_COUNT, &ship, explosions, EXPLOSION_COUNT);
            }

            if (ship.lives <= 0)
            {
                is_game_over = true;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
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
                ship_draw(&ship);
                bullet_draw(bullets, BULLET_COUNT);
                comet_draw(comets, COMET_COUNT);
                explosion_draw(explosions, EXPLOSION_COUNT);
                al_draw_textf(font_18, al_map_rgb(255, 0, 255), 5, 5, 0, "Player has %i lives left. Objects destroyed: %i", ship.lives, ship.score);
            }
            else
            {
                al_draw_textf(font_18, al_map_rgb(0, 255, 255), window.width / 2, window.height / 2, ALLEGRO_ALIGN_CENTRE, "Game Over. Final Score: %i", ship.score);
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(25, 29, 30));
        }
    }

    al_destroy_bitmap(explosion_sprite_sheet);
    al_destroy_bitmap(comet_sprite_sheet);
    al_destroy_bitmap(ship_sprite_sheet);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_font(font_18);
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
