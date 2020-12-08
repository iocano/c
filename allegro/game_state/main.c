#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

typedef enum game_state{MENU, PLAYING, GAMEOVER} game_state_t;

typedef struct area
{
    float width, height;
}area_t;

void exit_if(bool condition, const char *format, ...);

void change_state(game_state_t *state, game_state_t new_state);

int main(int argc, char **argv)
{
    area_t window = {800, 600};
    bool keys[ALLEGRO_KEY_MAX] = {false};
    bool done = false, render = false;
    int frames = 0, game_fps = 0;
    float game_time = 0;
    game_state_t state = -1;

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
    exit_if(!(font18 = al_load_font("resources/arial.ttf", 18, 0)), "Can't load resources/arial.ttf");

    change_state(&state, MENU);

    timer = al_create_timer(1.0 / 60);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);
    while(!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            keys[ev.keyboard.keycode] = true;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            keys[ev.keyboard.keycode] = false;
        }
        else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            render = true;
            if(state == MENU)
            {
                if(keys[ALLEGRO_KEY_SPACE])
                {
                    change_state(&state, PLAYING);
                    keys[ALLEGRO_KEY_SPACE] = false;
                }
                if(keys[ALLEGRO_KEY_ESCAPE])
                {
                    done = true;
                }
            }
            else if(state == PLAYING)
            {
                if(keys[ALLEGRO_KEY_SPACE])
                {
                    change_state(&state, GAMEOVER);
                    keys[ALLEGRO_KEY_SPACE] = false;
                }
                if(keys[ALLEGRO_KEY_ESCAPE])
                {
                    change_state(&state, MENU);
                    keys[ALLEGRO_KEY_ESCAPE] = false;
                }
            }
            else if(state == GAMEOVER)
            {
                if(keys[ALLEGRO_KEY_SPACE])
                {
                    change_state(&state, PLAYING);
                    keys[ALLEGRO_KEY_SPACE] = false;
                }
                if(keys[ALLEGRO_KEY_ESCAPE])
                {
                    done = true;
                }
            }
        }

        if(render && al_is_event_queue_empty(event_queue))
        {
            render = false;
            if(state == MENU)
            {
                al_draw_text(font18, al_map_rgb(255, 255, 255), window.width / 2, 20, ALLEGRO_ALIGN_CENTRE, "Menu");
                al_draw_text(font18, al_map_rgb(255, 255, 255), window.width / 2, window.height / 2, ALLEGRO_ALIGN_CENTRE, "Press SPACE to play, ESC to exit");
            }
            else if(state == PLAYING)
            {
                al_draw_text(font18, al_map_rgb(255, 255, 255), window.width / 2, 20, ALLEGRO_ALIGN_CENTRE, "Playing");
                al_draw_text(font18, al_map_rgb(255, 255, 255), window.width / 2, window.height / 2, ALLEGRO_ALIGN_CENTRE, "Press ESC to menu, SPACE to game over");
            }
            else if(state == GAMEOVER)
            {
                al_draw_text(font18, al_map_rgb(255, 255, 255), window.width / 2, 20, ALLEGRO_ALIGN_CENTRE, "Game over");
                al_draw_text(font18, al_map_rgb(255, 255, 255), window.width / 2, window.height / 2, ALLEGRO_ALIGN_CENTRE, "Press ESC to exit, SPACE to play again");
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }
    al_destroy_font(font18);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}

void change_state(game_state_t *state, game_state_t newState)
{
    if(*state == MENU)
    {
        printf("Leaving the MENU state\n");
    }
    else if(*state == PLAYING)
    {
        printf("Leaving the PLAYING state\n");
    }
    else if(*state == GAMEOVER)
    {
        printf("Leaving the GAMEOVER state\n");
    }

    *state = newState;

    if(*state == MENU)
    {
        printf("Entering the MENU state\n");
    }
    else if(*state == PLAYING)
    {
        printf("Entering the PLAYING state\n");
    }
    else if(*state == GAMEOVER)
    {
        printf("Entering the GAMEOVER state\n");
    }
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
