#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

void exit_if(bool condition, const char *format, ...);

int main()
{
    int width = 640, height = 480;
    int pos_x = width / 2, pos_y = height / 2;

    bool done = false;
    bool keys[ALLEGRO_KEY_MAX];

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *queue = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    display = al_create_display(640, 480);
    exit_if(!display, "Can't initialize display\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives addon\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard");

    queue = al_create_event_queue();
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(255, 0, 255));
    al_flip_display();
    while (!done)
    {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        if (event.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            keys[event.keyboard.keycode] = true;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_UP)
        {
            keys[event.keyboard.keycode] = false;
        }
        else if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        if (keys[ALLEGRO_KEY_ESCAPE] == true)
        {
            done = true;
        }
        pos_y -= keys[ALLEGRO_KEY_UP] * 5;
        pos_y += keys[ALLEGRO_KEY_DOWN] * 5;
        pos_x -= keys[ALLEGRO_KEY_LEFT] * 5;
        pos_x += keys[ALLEGRO_KEY_RIGHT] * 5;

        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(255, 0, 255));
        al_flip_display();
    }
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
