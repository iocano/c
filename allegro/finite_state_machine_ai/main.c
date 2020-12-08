#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/display.h>
#include <allegro5/keyboard.h>
#include <allegro5/mouse.h>
#include <math.h>
#include <stdio.h>

typedef enum game_state{IDLE, CHASING, RETREATING} game_state_t;

typedef struct point
{
    float x, y;
}point_t;

typedef struct area
{
    float width, height;
}area_t;

typedef struct enemy
{
    point_t position;
    game_state_t state;
    int threshold;
}enemy_t;

void change_state(game_state_t *state, game_state_t newState);

float check_distance(int x1, int y1, int x2, int y2);

float angle_to_target(int x1, int y1, int x2, int y2);

void exit_if(bool condition, const char *format, ...);

int main()
{
    area_t window = {800, 600};
    bool keys[ALLEGRO_KEY_MAX]= {false};
    bool done = false, render = false;

    float game_time = 0;
    int frames = 0;
    int game_fps = 0;

    point_t player_position = {0, 0};
    point_t cave_position = {window.width - 100, window.height / 2};
    enemy_t enemy;
    enemy.position.x = cave_position.x;
    enemy.position.y = cave_position.y;
    enemy.state = -1;
    enemy.threshold = 175;

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
    exit_if(!al_install_mouse(), "Can't install mouse\n");
    exit_if(!(font18 = al_load_font("resources/arial.ttf", 18, 0)), "Can't load arial.ttf\n");

    change_state(&enemy.state, IDLE);

    timer = al_create_timer(1.0/60);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
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
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            keys[ev.keyboard.keycode] = false;
        }
        else if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            render = true;
            frames++;
            if(al_current_time() - game_time >= 1)
            {
                game_time = al_current_time();
                game_fps = frames;
                frames = 0;
            }

            if(enemy.state == IDLE)
            {
                if(enemy.threshold > check_distance (player_position.x, player_position.y, enemy.position.x, enemy.position.y))
                {
                    change_state(&enemy.state, CHASING);
                }
            }
            else if(enemy.state == CHASING)
            {
                if(enemy.threshold < check_distance(enemy.position.x, enemy.position.y, cave_position.x, cave_position.y))
                {
                    change_state(&enemy.state, RETREATING);
                }
                else
                {
                    float angle = angle_to_target(enemy.position.x, enemy.position.y, player_position.x, player_position.y);
                    enemy.position.x += (cos(angle) * 2);
                    enemy.position.y += (sin(angle) * 2);

                    if(enemy.threshold < check_distance(enemy.position.x, enemy.position.y, player_position.x, player_position.y))
                    {
                        change_state(&enemy.state, RETREATING);
                    }
                }
            }
            else if(enemy.state == RETREATING)
            {
                if(5 >= check_distance(enemy.position.x, enemy.position.y, cave_position.x, cave_position.y))
                {
                    enemy.position.x = cave_position.x;
                    enemy.position.y = cave_position.y;
                    change_state(&enemy.state, IDLE);
                }
                else
                {
                    float angle = angle_to_target(enemy.position.x, enemy.position.y, cave_position.x, cave_position.y);
                    enemy.position.x += (cos(angle) * 2);
                    enemy.position.y += (sin(angle) * 2);

                    if(enemy.threshold > check_distance(enemy.position.x, enemy.position.y, player_position.x, player_position.y))
                    {
                        change_state(&enemy.state, CHASING);
                    }
                }
            }

        }
        else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            player_position.x = ev.mouse.x;
            player_position.y = ev.mouse.y;
        }

        if(render && al_is_event_queue_empty(event_queue))
        {
            render = false;

            al_draw_textf(font18, al_map_rgb(255, 255, 255), 5, 5, 0, "FPS: %i", game_fps);

            al_draw_circle(cave_position.x, cave_position.y, enemy.threshold, al_map_rgba_f(.5, .5, .5, .5), 1);
            al_draw_circle(enemy.position.x, enemy.position.y, enemy.threshold, al_map_rgba_f(.5, 0, .5, .5), 1);

            al_draw_filled_rectangle(player_position.x - 10, player_position.y - 10, player_position.x + 10, player_position.y + 10, al_map_rgb(255, 255, 255));
            al_draw_filled_rectangle(enemy.position.x - 10, enemy.position.y - 10, enemy.position.x + 10, enemy.position.y + 10, al_map_rgb(255, 0, 255));

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    al_destroy_font(font18);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}

void change_state(game_state_t *state, game_state_t new_state)
{
    if(*state == IDLE)
    {
        printf("Leaving the IDLE state\n");
    }
    else if(*state == CHASING)
    {
        printf("Leaving the CHASING state\n");
    }
    else if(*state == RETREATING)
    {
        printf("Leaving the RETREATING state\n");
    }

    *state = new_state;

    if(*state == IDLE)
    {
        printf("Now IDLING\n");
    }
    else if(*state == CHASING)
    {
        printf("Now CHASING the player\n");
    }
    else if(*state == RETREATING)
    {
        printf("Now RETREATING back to my cave\n");
    }
}

float check_distance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((float)x1-x2, 2) + pow ((float)y1-y2,2));
}

float angle_to_target(int x1, int y1, int x2, int y2)
{
    float deltaX = (x2-x1);
    float deltaY = (y2-y1);
    return atan2(deltaY, deltaX);
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
