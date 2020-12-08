#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

void exit_if(bool condition, const char *format, ...);

int main()
{
    int width = 640,
        height = 480,
        image_width = 0,
        image_height = 0;

    bool done = false;

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_BITMAP *image = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(width, height)), "Can't create display\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");

    image = al_load_bitmap("ship.png");
    image_width = al_get_bitmap_width(image);
    image_height = al_get_bitmap_height(image);

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
        {
            done = true;
        }
        // al_draw_bitmap(image, width / 2 - image_width / 2, height / 2 - image_height / 2, 0);
        float position_x = width / 2 - (image_width * 3.0) / 2;
        float position_y = height / 2 - (image_height * 3.0) / 2;
        al_draw_scaled_bitmap(image, 0, 0, image_width, image_height, position_x, position_y, image_width * 3.0, image_height * 3.0 ,0);
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }

    al_destroy_bitmap(image);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
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
