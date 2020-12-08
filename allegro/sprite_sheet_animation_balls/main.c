#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

typedef struct point
{
    int x;
    int y;
}point_t;

typedef struct animation
{
    point_t position;
    point_t speed;
    point_t direction;
    point_t sprite_current;
    point_t sprite_size;
    point_t sprite_sheet_matrix_size;
    ALLEGRO_BITMAP *sprite_sheet;
    point_t movement_limit;
} animation_t;

void animation_init(animation_t *animation, ALLEGRO_BITMAP *sprite_sheet, point_t sprite_sheet_matrix_size, point_t sprite_size, point_t window_size);

void animation_update(animation_t* animation, int timer_count);

void animation_draw(animation_t *animation);

void exit_if(bool condition, const char *format, ...);

int main()
{
    point_t window_size = { 640, 480 };
    bool done = false, render = false;
    const int orb_count = 100;
    animation_t orbs[orb_count];

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;
    ALLEGRO_BITMAP *sprite_sheet;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window_size.x, window_size.y)), "Can't create display\n");
    exit_if(!al_init_image_addon(), "Can't initialize addon\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!(sprite_sheet = al_load_bitmap("orb.png")), "Can't load orb.png\n");

    point_t sprite_sheet_matrix_size = {5, 4};
    point_t sprite_size =
    {
        al_get_bitmap_width(sprite_sheet) / sprite_sheet_matrix_size.x,
        al_get_bitmap_height(sprite_sheet) / sprite_sheet_matrix_size.y
    };

    srand(time(NULL));
    for (int i = 0; i < orb_count; i++)
    {
        animation_init(&orbs[i], sprite_sheet, sprite_sheet_matrix_size, sprite_size, window_size);
    }

    timer = al_create_timer(1.0 / 60);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

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
            for (int i = 0; i < orb_count; i++)
            {
                animation_update(&orbs[i], al_get_timer_count(timer));
            }
        }

        if (render && al_is_event_queue_empty(event_queue))
        {
            render = false;
            for (int i = 0; i < orb_count; i++)
            {
                animation_draw(&orbs[i]);
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    al_destroy_bitmap(sprite_sheet);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
    return 0;
}

void animation_init(animation_t *animation, ALLEGRO_BITMAP *sprite_sheet, point_t sprite_sheet_matrix_size, point_t sprite_size, point_t window_size)
{
    animation->position.x = rand() % (window_size.x - sprite_size.x) + sprite_size.x;
    animation->position.y = rand() % (window_size.y - sprite_size.y) + sprite_size.y;
    animation->speed.x = rand() % 7 + 2;
    animation->speed.y = rand() % 7 + 2;
    int values[] = {-1, 1};
    animation->direction.x = values[rand() % 1];
    animation->direction.y = values[rand() % 1];
    animation->sprite_current.x = rand() % 5;
    animation->sprite_current.y = rand() % 4;
    animation->sprite_size = sprite_size;
    animation->sprite_sheet_matrix_size = sprite_sheet_matrix_size;
    animation->sprite_sheet = sprite_sheet;
    animation->movement_limit = window_size;
}

void animation_update(animation_t* animation, int timer_count)
{
    if (timer_count% 5 == 0)
    {
        // move to next sprite column
        animation->sprite_current.x++;
        if (animation->sprite_current.x >= animation->sprite_sheet_matrix_size.x)
        {
            animation->sprite_current.x = 0;
        }
        // move to next sprite row
        animation->sprite_current.y++;
        if (animation->sprite_current.y >= animation->sprite_sheet_matrix_size.y)
        {
            animation->sprite_current.y = 0;
        }
    }

    animation->position.x += animation->speed.x * animation->direction.x;
    // Orb is inside movement limits at x
    if ((animation->position.x <= 0 && animation->direction.x == -1) || (animation->position.x >= animation->movement_limit.x - animation->sprite_size.x && animation->direction.x == 1))
    {
        animation->direction.x *= -1;
    }

    animation->position.y += animation->speed.y * animation->direction.y;
    // Orb is inside movement limits at y
    if((animation->position.y <= 0 && animation->direction.y == -1) || (animation->position.y >= animation->movement_limit.y - animation->sprite_size.y && animation->direction.y == 1))
    {
        animation->direction.y *= -1;
    }
}

void animation_draw(animation_t *animation)
{
    int fx = animation->sprite_current.x * animation->sprite_size.x;
    int fy = animation->sprite_current.y * animation->sprite_size.y;

    al_draw_bitmap_region(
            animation->sprite_sheet,    // image
            fx,                         // image offset at x
            fy,                         // image offset at y
            animation->sprite_size.x,   // frame width
            animation->sprite_size.y,   // frame height
            animation->position.x,      // display offset at x
            animation->position.y,      // display offset at y
            0);
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
