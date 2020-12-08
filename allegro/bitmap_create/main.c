#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

void exit_if(bool condition, const char *format, ...);

void draw_pixel(float x_start, float x_end, float y_start, float y_end, ALLEGRO_COLOR color);

int main()
{
    int width = 640, height = 480, image_side_length = 40;
    bool done = false;

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_BITMAP *image_half_ship = NULL;
    ALLEGRO_BITMAP *image_full_ship = NULL;
    ALLEGRO_TIMER *timer = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(width, height)), "Can't create display\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!al_init_primitives_addon(), "Cant initialize primitives addon\n");


    // Draw half ship
    {
        image_half_ship = al_create_bitmap(image_side_length, image_side_length / 2);

        // Set target where are you going to draw
        al_set_target_bitmap(image_half_ship);

        // Draw to bitmap
        al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));

        ALLEGRO_COLOR color1 = al_map_rgb(255, 255, 255);
        draw_pixel(1, 9, 14, 14, color1);
        draw_pixel(11, 12, 19, 19, color1);
        draw_pixel(13, 16, 17, 17, color1);
        draw_pixel(17, 17, 18, 18, color1);
        draw_pixel(18, 35, 19, 19, color1);
        draw_pixel(12, 13, 18, 18, color1);

        ALLEGRO_COLOR color2 = al_map_rgb(230, 230, 230);
        draw_pixel(2, 4, 13, 13, color2);
        draw_pixel(12, 12, 17, 17, color2);
        draw_pixel(13, 16, 16, 16, color2);
        draw_pixel(17, 17, 17, 17, color2);
        draw_pixel(14, 15, 18, 18, color2);
        draw_pixel(18, 18, 18, 18, color2);
        draw_pixel(20, 36, 18, 18, color2);
        draw_pixel(13, 16, 19, 19, color2);
        draw_pixel(36, 37, 19, 19, color2);
        draw_pixel(7, 7, 18, 18, color2);
        draw_pixel(8, 8, 17, 17, color2);
        draw_pixel(9, 9, 16, 16, color2);
        draw_pixel(18, 19, 14, 14, color2);
        draw_pixel(18, 19, 15, 15, color2);
        draw_pixel(18, 19, 16, 16, color2);

        ALLEGRO_COLOR color3 = al_map_rgb(190, 190, 190);
        draw_pixel(11, 11, 12, 12, color3);
        draw_pixel(13, 15, 12, 12, color3);
        draw_pixel(5, 7, 13, 13, color3);
        draw_pixel(10, 10, 13, 13, color3);
        draw_pixel(15, 15, 13, 13, color3);
        draw_pixel(15, 17, 14, 14, color3);
        draw_pixel(5, 7, 15, 15, color3);
        draw_pixel(17, 17, 15, 15, color3);
        draw_pixel(19, 35, 17, 17, color3);

        ALLEGRO_COLOR color4 = al_map_rgb(100, 100, 100);
        draw_pixel(12, 15, 11, 11, color4);
        draw_pixel(10, 10, 12, 12, color4);
        draw_pixel(16, 16, 12, 12, color4);
        draw_pixel(19, 19, 12, 12, color4);
        draw_pixel(8, 9, 13, 13, color4);
        draw_pixel(16, 19, 13, 13, color4);
        draw_pixel(8, 9, 15, 15, color4);
        draw_pixel(13, 16, 15, 15, color4);
        draw_pixel(12, 12, 16, 16, color4);
        draw_pixel(17, 17, 16, 16, color4);
        draw_pixel(11, 11, 17, 17, color4);
        draw_pixel(18, 18, 17, 17, color4);
        draw_pixel(11, 11, 18, 18, color4);
        draw_pixel(19, 19, 18, 18, color4);
        draw_pixel(8, 10, 19, 19, color4);
        draw_pixel(20, 20, 13, 16, color4);
        draw_pixel(21, 31, 16, 16, color4);

        ALLEGRO_COLOR color5 = al_map_rgb(0, 0, 0);
        draw_pixel(16, 16, 18, 18, color5);
        draw_pixel(16, 16, 18, 18, color5);
        draw_pixel(17, 17, 19, 19, color5);

        ALLEGRO_COLOR color6 = al_map_rgb(60, 60, 60);
        draw_pixel(21, 21, 14, 14, color5);
    }

    // Join half ship with its mirror version
    {
        image_full_ship = al_create_bitmap(image_side_length, image_side_length);

        al_set_target_bitmap(image_full_ship);

        al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
        al_draw_bitmap(image_half_ship, 0, 0, 0);
        al_draw_bitmap(image_half_ship, 0, image_side_length / 2, ALLEGRO_FLIP_VERTICAL);
    }

    // Return to display bitmap
    al_set_target_bitmap(al_get_backbuffer(display));

    timer = al_create_timer(1.0 / 60.0);
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

        float new_scale = 10;
        float x = (width / 2) - (image_side_length / 2 * new_scale);
        float y = (height / 2) - (image_side_length / 2 * new_scale);

        ALLEGRO_TRANSFORM transform;
        al_identity_transform(&transform);
        al_scale_transform(&transform, new_scale, new_scale);
        al_translate_transform(&transform, x, y);
        al_use_transform(&transform);

        al_draw_bitmap(image_full_ship, 0, 0, 0);
        al_flip_display();
        al_clear_to_color(al_map_rgb(25, 29, 30));
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(image_half_ship);
    al_destroy_bitmap(image_full_ship);
    al_destroy_timer(timer);
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

void draw_pixel(float x_start, float x_end, float y_start, float y_end, ALLEGRO_COLOR color)
{
    for (float i = x_start; i <= x_end; i++)
    {
        for(float j = y_start; j <= y_end; j++)
        {
            al_draw_pixel(i, j, color);
        }
    }
}
