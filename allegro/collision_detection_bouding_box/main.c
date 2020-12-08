#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

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
    area_t bound;
    area_t size;
    ALLEGRO_BITMAP *image;
} object_t;

void exit_if(bool condition, const char *format, ...);

int main()
{
    area_t window = {800, 600};
    bool done = false, render = false, bound = false, collision = false;
    bool keys[ALLEGRO_KEY_MAX] = {false};
    object_t ball1, ball2;

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
    exit_if(!(font18 = al_load_font("resources/arial.ttf", 18, 0)), "Can't load arial.ttf\n");

    exit_if(!(ball1.image = al_load_bitmap("resources/object.png")), "Can't load object.png\n");
    ball1.size.width = al_get_bitmap_width(ball1.image);
    ball1.size.height = al_get_bitmap_height(ball1.image);
    ball1.position.x = 0;
    ball1.position.y = 0;
    ball1.bound.width = ball1.size.width / 2 - 9;
    ball1.bound.height = ball1.size.height / 2 - 13;

    exit_if(!(ball2.image = al_load_bitmap("resources/object.png")), "Can't load object.png\n");
    ball2.size.width = al_get_bitmap_width(ball2.image);
    ball2.size.height = al_get_bitmap_height(ball2.image);
    ball2.position.x = window.width / 2;
    ball2.position.y = window.height / 2;
    ball2.bound.width = ball2.size.width / 2 - 9;
    ball2.bound.height = ball2.size.height / 2 - 13;

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
            ball1.position.y -= 5 * keys[ALLEGRO_KEY_UP];
            ball1.position.y += 5 * keys[ALLEGRO_KEY_DOWN];
            ball1.position.x -= 5 * keys[ALLEGRO_KEY_LEFT];
            ball1.position.x += 5 * keys[ALLEGRO_KEY_RIGHT];
            bound = keys[ALLEGRO_KEY_SPACE];

            bool ball1_inside_left_ball2_bound = ball1.position.x + ball1.bound.width > ball2.position.x - ball2.bound.width;
            bool ball1_inside_right_ball2_bound = ball1.position.x - ball1.bound.width < ball2.position.x + ball2.bound.width;
            bool ball1_inside_upper_ball2_bound = ball1.position.y + ball1.bound.height > ball2.position.y - ball2.bound.height;
            bool ball1_inside_lower_ball2_bound = ball1.position.y - ball1.bound.height < ball2.position.y + ball2.bound.height;
            if (ball1_inside_left_ball2_bound && ball1_inside_right_ball2_bound && ball1_inside_upper_ball2_bound && ball1_inside_lower_ball2_bound)
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
            al_draw_bitmap(ball2.image, ball2.position.x - ball2.size.width/ 2, ball2.position.y - ball2.size.height / 2, 0);
            al_draw_bitmap(ball1.image, ball1.position.x - ball1.size.height / 2, ball1.position.y - ball1.size.height / 2, 0);
            if (bound)
            {
                al_draw_filled_rectangle(ball1.position.x - ball1.bound.width, ball1.position.y - ball1.bound.height, ball1.position.x + ball1.bound.width, ball1.position.y + ball1.bound.height, al_map_rgba_f(0, .3, .3, .1));
                al_draw_filled_rectangle(ball2.position.x - ball2.bound.width, ball2.position.y - ball2.bound.height, ball2.position.x + ball2.bound.width, ball2.position.y + ball2.bound.height, al_map_rgba_f(0, .3, .3, .1));
            }
            if (collision)
            {
                al_draw_text(font18, al_map_rgb(255, 255, 255), window.width / 2, 20, ALLEGRO_ALIGN_CENTRE, "Collision!");
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    al_destroy_bitmap(ball1.image);
    al_destroy_bitmap(ball2.image);
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
