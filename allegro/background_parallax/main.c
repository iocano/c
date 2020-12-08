#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

const int WIDTH = 800;
const int HEIGHT = 600;

typedef struct point
{
    int x, y;
}point_t;

typedef struct area
{
    int width, height;
}area_t;

typedef struct background
{
    point_t position;
    point_t speed;
    point_t direction;
    area_t area;
    ALLEGRO_BITMAP *image;
} background_t;

void background_init(background_t *background, float x, float y, float speed_x, float speed_y, int direcction_x, int direcction_y, int width, int height, ALLEGRO_BITMAP *image);

void background_update(background_t *background);

void background_draw(background_t *background);

void exit_if(bool condition, const char *format, ...);

int main()
{
    bool done = false, render = false;
    background_t background, middleground, foreground;

    ALLEGRO_DISPLAY *display;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *background_image, *middleground_image, *foreground_image;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(WIDTH, HEIGHT)), "Can't create display\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");
    exit_if(!al_install_keyboard(), "Can;t install keyboard\n");

    exit_if(!(background_image = al_load_bitmap("resources/star_background.png")), "Can't load star_background.png\n");
    exit_if(!(middleground_image = al_load_bitmap("resources/star_middleground.png")), "Can't load star_middleground.png\n");
    exit_if(!(foreground_image = al_load_bitmap("resources/star_foreground.png")), "Can't load star_foreground.png\n");

    background_init(&background, 0, 0, 1, 0, -1, 1, 800, 600, background_image);
    background_init(&middleground, 0, 0, 3, 0, -1, 1, 1600, 600, middleground_image);
    background_init(&foreground, 0, 0, 5, 0, -1, 1, 800, 600, foreground_image);

    timer = al_create_timer(1.0 / 60);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

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
            render = true;
            background_update(&background);
            background_update(&middleground);
            background_update(&foreground);
        }

        if (render && al_is_event_queue_empty(event_queue))
        {
            render = false;
            background_draw(&background);
            background_draw(&middleground);
            background_draw(&foreground);
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    al_destroy_bitmap(background_image);
    al_destroy_bitmap(middleground_image);
    al_destroy_bitmap(foreground_image);
    al_destroy_timer(timer);
    al_destroy_display(display);
}

void background_init(background_t *background, float x, float y, float speed_x, float speed_y, int direction_x, int direction_y, int width, int height, ALLEGRO_BITMAP *image)
{
    background->position.x = x;
    background->position.y = y;
    background->speed.x = speed_x;
    background->speed.y = speed_y;
    background->direction.x = direction_x;
    background->direction.y = direction_y;
    background->area.width = width;
    background->area.height = height;
    background->image = image;
}

void background_update(background_t *background)
{
    background->position.x += background->speed.x * background->direction.x;
    if (background->position.x + background->area.width <= 0)
    {
        background->position.x = 0;
    }
}

void background_draw(background_t *background)
{
    al_draw_bitmap(background->image, background->position.x, background->position.y, 0);

    if (background->position.x + background->area.width < WIDTH)
    {
        al_draw_bitmap(background->image, background->position.x + background->area.width, background->position.y, 0);
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
