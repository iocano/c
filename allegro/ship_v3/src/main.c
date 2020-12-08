#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <string.h>

#include "include/object.h"
#include "include/background.h"
#include "include/bullet.h"
#include "include/comet.h"
#include "include/explosion.h"
#include "include/ship.h"

#define NUM_BULLETS  50
#define NUM_COMETS 10
#define NUM_EXPLOSIONS  5

typedef enum game_state {GAME_STATE_TITLE, GAME_STATE_PLAYING, GAME_STATE_LOST} game_state_t;

ALLEGRO_SAMPLE_INSTANCE *song_sample_instance = NULL;
background_t background, middleground, foreground;
ship_t ship;
bullet_t bullets[NUM_BULLETS];
comet_t comets[NUM_COMETS];
explosion_t explosions[NUM_EXPLOSIONS];

void change_state(game_state_t *game_state, game_state_t new_state);

void exit_if(bool condition, const char *format, ...);

int main(int argc, char **argv)
{
    object_size_t window = {800, 600};
    bool done = false, redraw = true;
    bool keys[ALLEGRO_KEY_MAX] = {false};
    game_state_t game_state = -1;

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font18 = NULL;
    ALLEGRO_BITMAP *ship_image, *comet_image, *explosion_image;
    ALLEGRO_BITMAP *background_image, *middleground_image, *foreground_image;
    ALLEGRO_BITMAP *title_image, *lost_image, *icon_image;
    ALLEGRO_SAMPLE *shot_sample, *explosion_sample, *song_sample;
    ALLEGRO_VOICE *voice = NULL;
    ALLEGRO_MIXER *mixer = NULL;

    // initialize
    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window.width, window.height)), "Can't create display\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!al_install_audio(), "Can't install audio\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives addon\n");
    exit_if(!al_init_font_addon(), "Can't initialize font addon\n");
    exit_if(!al_init_ttf_addon(), "Can't initialize ttf addon\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");
    exit_if(!al_init_acodec_addon(), "Can't initialize acodec addon\n");

    // load images
    exit_if(!(font18 = al_load_font("resources/DejaVuSans.ttf" , 18, 0)), "Can't load DejaVuSans.ttf\n");
    exit_if(!(ship_image = al_load_bitmap("resources/ship.png")), "Can't load ship.png\n");
    exit_if(!(comet_image = al_load_bitmap("resources/comet.png")), "Can't load comet.png\n");
    exit_if(!(explosion_image = al_load_bitmap("resources/explosion.png")), "Can't load explosion.png\n");
    exit_if(!(background_image = al_load_bitmap("resources/background.png")), "Can't load background.png\n");
    exit_if(!(middleground_image= al_load_bitmap("resources/middleground.png")), "Can't load middleground.png\n");
    exit_if(!(foreground_image = al_load_bitmap("resources/foreground.png")), "Can't load foreground.png\n");
    exit_if(!(title_image = al_load_bitmap("resources/screen_title.png")), "Can't load screen_title.png\n");
    exit_if(!(lost_image = al_load_bitmap("resources/screen_lost.png")), "Can't load screen_lost.png\n");
    exit_if(!(icon_image = al_load_bitmap("resources/icon.png")), "Can't load icon.png\n");

    // load audio
    exit_if(!(shot_sample = al_load_sample("resources/shot.ogg")), "Can't load shoot.ogg\n");
    exit_if(!(explosion_sample = al_load_sample("resources/explosion.ogg")), "Can't load explosion.ogg\n");
    exit_if(!(song_sample = al_load_sample("resources/song.ogg")), "Can't load song.ogg");

    // remove color to ship image
    al_convert_mask_to_alpha(ship_image, al_map_rgb(255, 0, 255));

    al_set_display_icon(display, icon_image);

    // audio output
    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    al_attach_mixer_to_voice(mixer, voice);
    al_set_default_mixer(mixer);
    al_reserve_samples(NUM_BULLETS + NUM_EXPLOSIONS);

    // Background song
    song_sample_instance = al_create_sample_instance(song_sample);
    al_set_sample_instance_playmode(song_sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(song_sample_instance, al_get_default_mixer());

    timer = al_create_timer(1.0 / 60);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));

    srand(time(NULL));
    change_state(&game_state, GAME_STATE_PLAYING);

    object_limit_t background_limit = {{0, 0}, {window.width, window.height}};
    background_init(&background,0, 0, 1, 0, 800, 600, -1, 1, background_limit, background_image);
    background_init(&middleground, 0, 0, 3, 0, 1600, 600, -1, 1, background_limit, middleground_image);
    background_init(&foreground, 0, 0, 5, 0, 800, 600, -1, 1, background_limit, foreground_image);

    object_limit_t ship_limit = {{0, 0}, {300, window.height}};
    ship_init(&ship, ship_limit, ship_image);
    object_limit_t bullet_limit = {{0, 0}, {window.width, window.height}};
    bullet_init(bullets, NUM_BULLETS, bullet_limit, shot_sample);
    object_limit_t comet_limit = {{0, 0}, {window.width, window.height}};
    comet_init(comets, NUM_COMETS, comet_image, comet_limit);
    explosion_init(explosions, NUM_EXPLOSIONS, explosion_image, explosion_sample);

    al_start_timer(timer);
    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            if(game_state == GAME_STATE_TITLE)
            {
                if(keys[ALLEGRO_KEY_SPACE])
                {
                    change_state(&game_state, GAME_STATE_PLAYING);
                    keys[ALLEGRO_KEY_SPACE] = false;
                }

            }
            else if(game_state == GAME_STATE_PLAYING)
            {
                if(keys[ALLEGRO_KEY_ESCAPE])
                {
                    done = true;
                }
                if(keys[ALLEGRO_KEY_SPACE])
                {
                    bullet_fire(bullets, NUM_BULLETS, &ship);
                    keys[ALLEGRO_KEY_SPACE] = false;
                }

                if(keys[ALLEGRO_KEY_UP])
                {
                    ship_move_up(&ship);
                }
                else if(keys[ALLEGRO_KEY_DOWN])
                {
                    ship_move_down(&ship);
                }
                else
                {
                    ship_reset_animation(&ship);
                }

                if(keys[ALLEGRO_KEY_LEFT])
                {
                    ship_move_left(&ship);
                }
                else if(keys[ALLEGRO_KEY_RIGHT])
                {
                    ship_move_right(&ship);
                }

                background_update(&background);
                background_update(&middleground);
                background_update(&foreground);

                explosion_update(explosions, NUM_EXPLOSIONS, al_get_timer_count(timer));
                bullet_update(bullets, NUM_BULLETS);
                comet_start(comets, NUM_COMETS);
                comet_update(comets, NUM_COMETS, al_get_timer_count(timer));
                bullet_collide(bullets, NUM_BULLETS, comets, NUM_COMETS, &ship, explosions, NUM_EXPLOSIONS);
                comet_collide(comets, NUM_COMETS, &ship, explosions, NUM_EXPLOSIONS);

                if (ship.lives <= 0)
                {
                    change_state(&game_state, GAME_STATE_LOST);
                }
            }
            else if(game_state == GAME_STATE_LOST)
            {
                if(keys[ALLEGRO_KEY_SPACE])
                {
                    change_state(&game_state, GAME_STATE_PLAYING);
                    keys[ALLEGRO_KEY_SPACE] = false;
                }
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
            if(game_state == GAME_STATE_TITLE)
            {
                al_draw_bitmap(title_image, 0, 0, 0);
            }
            else if(game_state == GAME_STATE_PLAYING)
            {
                background_draw(&background);
                background_draw(&middleground);
                background_draw(&foreground);
                ship_draw(&ship);
                bullet_draw(bullets, NUM_BULLETS);
                comet_draw(comets, NUM_COMETS);
                explosion_draw(explosions, NUM_EXPLOSIONS);
                al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "Player has %i left. Player has destroyed %i objects", ship.lives, ship.score);
            }
            else if(game_state == GAME_STATE_LOST)
            {
                al_draw_bitmap(lost_image, 0, 0, 0);
                al_draw_textf(font18, al_map_rgb(0, 255, 255), window.width - 10, 20, ALLEGRO_ALIGN_RIGHT, "Final Score: %i", ship.score);
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(25, 29, 30));
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }

    al_destroy_sample(song_sample);
    al_destroy_sample(explosion_sample);
    al_destroy_sample(shot_sample);
    al_destroy_sample_instance(song_sample_instance);

    al_destroy_bitmap(background_image);
    al_destroy_bitmap(middleground_image);
    al_destroy_bitmap(foreground_image);
    al_destroy_bitmap(title_image);
    al_destroy_bitmap(lost_image);
    al_destroy_bitmap(explosion_image);
    al_destroy_bitmap(comet_image);
    al_destroy_bitmap(ship_image);

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_font(font18);
    al_destroy_display(display);
    return 0;
}

void change_state(game_state_t *game_state, game_state_t new_state)
{
    if(*game_state == GAME_STATE_TITLE)
    {}
    else if(*game_state == GAME_STATE_PLAYING)
    {
        al_stop_sample_instance(song_sample_instance);
    }
    else if(*game_state == GAME_STATE_LOST)
    {}

    *game_state = new_state;

    if(*game_state == GAME_STATE_TITLE)
    {}
    else if(*game_state == GAME_STATE_PLAYING)
    {
        ship_reset(&ship);
        bullet_reset(bullets, NUM_BULLETS);
        comet_reset(comets, NUM_COMETS);
        explosion_reset(explosions, NUM_EXPLOSIONS);
        al_play_sample_instance(song_sample_instance);
    }
    else if(*game_state == GAME_STATE_LOST)
    {}
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
