#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include "include/object.h"
#include "include/ship.h"
#include "include/bullet.h"
#include "include/asteroid.h"
#include "include/utils.h"
#include "include/explosion.h"

typedef enum game_state{GAME_STATE_INITIAL, GAME_STATE_PLAYING, GAME_STATE_PAUSE, GAME_STATE_LOST}game_state_t;
void draw_player_single_live(object_point_t position);
void hud_draw(ship_t *ship, ALLEGRO_FONT *font);
void change_game_state(game_state_t *game_state, game_state_t new_state);
ALLEGRO_SAMPLE_INSTANCE* create_sample_instance(ALLEGRO_SAMPLE *sample, ALLEGRO_PLAYMODE playmode, float gain);

int bullet_count = 5;
int asteroid_count = 5;
int explosion_count = 10;

ship_t *ship;
linked_list_t *bullet_list;
linked_list_t *asteroid_list;
linked_list_t *explosion_list;

ALLEGRO_SAMPLE_INSTANCE *background_instance, *thrust_instance;
ALLEGRO_SAMPLE *shot_sample, *thrust_sample, *explosion_sample, *background_sample;

int main()
{
    object_timer_t add_asteroids_timer = {.start = 0, .limit = 30};
    bool done = false, redraw = false;
    bool keys[ALLEGRO_KEY_MAX] = {false};
    object_size_t window = {800 , 600};

    game_state_t game_state = -1;

    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font_30, *font_18;
    ALLEGRO_VOICE *voice = NULL;
    ALLEGRO_MIXER *mixer = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives add-on\n");
    exit_if(!al_init_font_addon(), "Can't initialize font addon\n");
    exit_if(!al_init_ttf_addon(), "Can't initialize ttf addon\n");
    exit_if(!al_init_acodec_addon(), "Can't initialize acodec addon\n");
    exit_if(!al_install_audio(), "Can't install audio drive\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard driver\n");
    exit_if(!(display = al_create_display(window.width, window.height)), "Can't create display\n");

    exit_if(!(font_30 = al_load_font("resources/space.otf", 30, 0)), "Can't load space.otf(30)\n");
    exit_if(!(font_18 = al_load_font("resources/space.otf", 18, 0)), "Can't load space.otf(18)\n");
    exit_if(!(shot_sample = al_load_sample("resources/shot.ogg")), "Can't load shot.ogg\n");
    exit_if(!(thrust_sample = al_load_sample("resources/thrust.ogg")), "Can't load thrust.ogg\n");
    exit_if(!(explosion_sample = al_load_sample("resources/explosion.ogg")), "Can't load explosion.ogg\n");
    exit_if(!(background_sample = al_load_sample("resources/background.ogg")), "Can't load background.ogg\n");

    timer = al_create_timer(1.0 / 60);
    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    al_attach_mixer_to_voice(mixer, voice);
    al_set_default_mixer(mixer);
    al_reserve_samples(bullet_count + explosion_count);
    ALLEGRO_SAMPLE_ID sample_ids[bullet_count + explosion_count];

    thrust_instance = create_sample_instance(thrust_sample, ALLEGRO_PLAYMODE_ONCE, 1);
    background_instance = create_sample_instance(background_sample, ALLEGRO_PLAYMODE_LOOP, 1);

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));

    object_limit_t ship_limit = {{0, 0}, {window.width, window.height}};
    object_limit_t bullet_limit = {{0, 0}, {window.width, window.height}};
    object_limit_t asteroid_limit = {{0, 0}, {window.width, window.height}};

    ship = ship_init(ship_limit);
    bullet_list = bullet_init(bullet_count, bullet_limit);
    asteroid_list = asteroid_init(asteroid_count, asteroid_limit);
    explosion_list = explosion_init(explosion_count);

    change_game_state(&game_state, GAME_STATE_INITIAL);
    add_asteroids_timer.start = al_current_time();
    al_start_timer(timer);
    while(!done)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if(event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            keys[event.keyboard.keycode] = true;
        }
        else if(event.type == ALLEGRO_EVENT_KEY_UP)
        {
            keys[event.keyboard.keycode] = false;
        }
        else if(event.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            if (game_state == GAME_STATE_INITIAL)
            {
                if (keys[ALLEGRO_KEY_SPACE])
                {
                    keys[ALLEGRO_KEY_SPACE] = false;
                    change_game_state(&game_state, GAME_STATE_PLAYING);
                }
                if (keys[ALLEGRO_KEY_ESCAPE])
                {
                    keys[ALLEGRO_KEY_ESCAPE] = false;
                    done = true;
                }
            }
            else if (game_state == GAME_STATE_PLAYING)
            {
                if (keys[ALLEGRO_KEY_ESCAPE])
                {
                    keys[ALLEGRO_KEY_ESCAPE] = false;
                    change_game_state(&game_state, GAME_STATE_PAUSE);
                }

                if (keys[ALLEGRO_KEY_SPACE])
                {
                    keys[ALLEGRO_KEY_SPACE] = false;
                    bullet_fire(bullet_list, ship->position, ship->direction);
                    al_play_sample(shot_sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }

                if (keys[ALLEGRO_KEY_LEFT])
                {
                    ship_turn_left(ship);
                }
                else if(keys[ALLEGRO_KEY_RIGHT])
                {
                    ship_turn_right(ship);
                }

                if (keys[ALLEGRO_KEY_UP] == true)
                {
                    ship_accelerate(ship);
                    al_play_sample_instance(thrust_instance);
                }
                else if (keys[ALLEGRO_KEY_UP] == false)
                {
                    ship_inertia(ship);
                    al_stop_sample_instance(thrust_instance);
                }
                object_collision_result_t asteroid_collision_result = asteroid_is_collided(asteroid_list, bullet_list, ship);
                if (asteroid_collision_result.is_collided == true)
                {
                    explosion_start(explosion_list, asteroid_collision_result.position);
                    al_play_sample(explosion_sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                object_collision_result_t ship_collision_result = ship_is_collided(ship, asteroid_list);
                if (ship_collision_result.is_collided == true)
                {
                    explosion_start(explosion_list, ship_collision_result.position);
                    al_play_sample(explosion_sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
                }
                ship_update(ship);
                bullet_update(bullet_list);
                asteroid_update(asteroid_list);
                explosion_update(explosion_list);

                if(ship->lives == 0)
                {
                    change_game_state(&game_state, GAME_STATE_LOST);
                }

                float current_time = al_current_time();
                if(current_time - add_asteroids_timer.start > add_asteroids_timer.limit)
                {
                    add_asteroids_timer.start = current_time;
                    asteroid_add(5, asteroid_list, asteroid_limit);
                }
            }
            else if (game_state == GAME_STATE_PAUSE)
            {
                if (keys[ALLEGRO_KEY_SPACE])
                {
                    keys[ALLEGRO_KEY_SPACE] = false;
                    change_game_state(&game_state, GAME_STATE_PLAYING);
                }
                if (keys[ALLEGRO_KEY_ESCAPE])
                {
                    keys[ALLEGRO_KEY_ESCAPE] = false;
                    done = true;
                }
            }
            else if (game_state == GAME_STATE_LOST)
            {
                if (keys[ALLEGRO_KEY_SPACE])
                {
                    keys[ALLEGRO_KEY_SPACE] = false;
                    change_game_state(&game_state, GAME_STATE_PLAYING);
                }
                if (keys[ALLEGRO_KEY_ESCAPE])
                {
                    keys[ALLEGRO_KEY_ESCAPE] = false;
                    done = true;
                }
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            if (game_state == GAME_STATE_INITIAL)
            {
                al_draw_text(font_30, al_map_rgb(200, 200, 200), window.width / 2, window.height / 2 - 35, ALLEGRO_ALIGN_CENTER , "ASTEROIDS!");
                al_draw_text(font_18, al_map_rgb(200, 200, 200), window.width / 2, window.height / 2, ALLEGRO_ALIGN_CENTER , "ESCAPE to exit | SPACE to play");
            }
            else if (game_state == GAME_STATE_PLAYING)
            {
                ship_draw(ship);
                bullet_draw(bullet_list);
                asteroid_draw(asteroid_list);
                explosion_draw(explosion_list);
                hud_draw(ship, font_18);
            }
            else if (game_state == GAME_STATE_PAUSE)
            {
                ALLEGRO_TRANSFORM transform;
                al_identity_transform(&transform);
                al_use_transform(&transform);
                al_draw_text(font_30, al_map_rgb(200, 200, 200), window.width / 2, window.height / 2 - 35, ALLEGRO_ALIGN_CENTER , "PAUSE");
                al_draw_text(font_18, al_map_rgb(200, 200, 200), window.width / 2, window.height / 2, ALLEGRO_ALIGN_CENTER , "ESCAPE to exit | SPACE to resume");
            }
            else if (game_state == GAME_STATE_LOST)
            {
                ALLEGRO_TRANSFORM transform;
                al_identity_transform(&transform);
                al_use_transform(&transform);
                al_draw_text(font_30, al_map_rgb(200, 200, 200), window.width / 2, window.height / 2 - 35, ALLEGRO_ALIGN_CENTER , "GAME OVER");
                al_draw_text(font_18, al_map_rgb(200, 200, 200), window.width / 2, window.height / 2, ALLEGRO_ALIGN_CENTER , "ESCAPE to exit | SPACE to play again");
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(25, 29, 30));
        }
    }
    ship_destroy(ship);
    bullet_destroy(bullet_list);
    asteroid_destroy(asteroid_list);
    explosion_destroy(explosion_list);
    al_destroy_sample(shot_sample);
    al_destroy_sample(explosion_sample);
    al_destroy_sample(thrust_sample);
    al_destroy_sample(background_sample);
    al_destroy_sample_instance(thrust_instance);
    al_destroy_sample_instance(background_instance);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    al_destroy_font(font_30);
    al_destroy_font(font_18);
    al_destroy_timer(timer);
    return 0;
}

ALLEGRO_SAMPLE_INSTANCE* create_sample_instance(ALLEGRO_SAMPLE *sample, ALLEGRO_PLAYMODE playmode, float gain)
{
    ALLEGRO_SAMPLE_INSTANCE *sample_instance = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(sample_instance, playmode);
    al_attach_sample_instance_to_mixer(sample_instance, al_get_default_mixer());
    return sample_instance;
}

void hud_draw(ship_t *ship, ALLEGRO_FONT *font)
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_use_transform(&transform);
    al_draw_textf(font, al_map_rgb(200, 200, 200), 5, 5, 0, "SCORE: %i", ship->score);

    for (int i = 0, right_screen_edge = ship->limit.upper.x - 15; i < ship->lives; i++, right_screen_edge -= 15)
    {
        object_point_t location = {.x = right_screen_edge, .y = 15};
        draw_player_single_live(location);
    }
}

void draw_player_single_live(object_point_t position)
{
    ALLEGRO_TRANSFORM transform;
    al_identity_transform(&transform);
    al_scale_transform(&transform, .75, .75);
    al_translate_transform(&transform, position.x, position.y);
    al_use_transform(&transform);

    ALLEGRO_COLOR color = al_map_rgb(200, 200, 200);
    al_draw_line(-8, 9, 0, -11, color, 1.5f);
    al_draw_line(0, -11, 8, 9, color, 1.5f);
    al_draw_line(-6, 4, -1, 4, color, 1.5f);
    al_draw_line(6, 4, 1, 4, color, 1.5f);
}

void change_game_state(game_state_t *game_state, game_state_t new_state)
{
    if(*game_state == GAME_STATE_INITIAL) {}
    else if(*game_state == GAME_STATE_PLAYING)
    {
        al_stop_sample_instance(background_instance);
    }
    else if(*game_state == GAME_STATE_LOST)
    {
        ship_reset(ship);
        bullet_reset(bullet_list);
        explosion_reset(explosion_list);
        asteroid_reset(asteroid_list, asteroid_count);
    }
    else if(*game_state == GAME_STATE_PAUSE) {}

    *game_state = new_state;

    if(game_state == GAME_STATE_INITIAL) { }
    else if(*game_state == GAME_STATE_PLAYING)
    {
        al_play_sample_instance(background_instance);
    }
    else if(*game_state  == GAME_STATE_LOST) { }
    else if(*game_state == GAME_STATE_PAUSE) { }
}
