
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <math.h>
#include <stdio.h>

typedef struct coordinate
{
    float x;
    float y;
}coordinate_t;

void exit_if(bool condition, const char *format, ...);

int main()
{

    bool keys[ALLEGRO_KEY_MAX] = {false};
    coordinate_t window_size = {640, 480};
    coordinate_t image_size = {0, 0};
    coordinate_t image_with_background_size = {0, 0};
    coordinate_t image_position = {0, 0};

    bool done = false;
    float image_angle = 0;
    float image_scale = 1;

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_BITMAP *image = NULL;
    ALLEGRO_BITMAP *image_with_background = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window_size.x, window_size.y)), "Can't create display\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");

    image = al_load_bitmap("ship.png");
    exit_if(!image, "Can't load ship.png\n");
    image_size.x = al_get_bitmap_width(image);
    image_size.y = al_get_bitmap_height(image);

    image_with_background = al_load_bitmap("ship_with_background.png");
    exit_if(!image_with_background, "Can't load ship_with_background.png\n");
    image_with_background_size.x = al_get_bitmap_width(image_with_background);
    image_with_background_size.y = al_get_bitmap_height(image_with_background);

    // remove background color (background color is rgb(50, 50, 50))
    al_convert_mask_to_alpha(image_with_background, al_map_rgb(50, 50, 50));

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

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

        if(keys[ALLEGRO_KEY_ESCAPE] == true)
        {
            done = true;
        }
        else
        {
            if(keys[ALLEGRO_KEY_LEFT])
            {
                image_angle -= 10;
                if (image_angle <= 0)
                {
                    image_angle = 360;
                }
            }
            else if(keys[ALLEGRO_KEY_RIGHT])
            {
                image_angle += 10;
                if (image_angle >= 360)
                {
                    image_angle = 0;
                }
            }

            if(keys[ALLEGRO_KEY_UP])
            {
                image_scale -= .05f;
            }
            else if(keys[ALLEGRO_KEY_DOWN])
            {
                image_scale += .05f;
            }
        }

        // Tint image
        al_draw_tinted_bitmap(image, al_map_rgb(255, 100, 255), window_size.x - image_size.x, 0, 0);

        // Calculate new scaled image center
        image_position.x  = window_size.x / 2 - image_size.x * image_scale / 2;
        image_position.y = window_size.y / 2 - image_size.y * image_scale / 2;
        // Draw scaled image
        al_draw_scaled_bitmap(
                image,
                0,                              // start x coord
                0,                              // start y coord (image center)
                image_size.x,                   // image x width (image center)
                image_size.y,                   // image y height
                image_position.x,               // destination x coord
                image_position.y,               // destination y coord
                image_size.x * image_scale,     // destination x width
                image_size.y * image_scale,     // destination y height
                0                               // flags
                );

        // Rotate image
        float radians = image_angle * M_PI / 180;
        al_draw_rotated_bitmap(
                image,
                image_size.x / 2,     // center x (spin axis)
                image_size.y / 2,    // center y (spin axis)
                0 + image_size.x / 2,          // destination x
                window_size.y - image_size.y / 2,         // destination y
                radians,            // angle
                0);                 // flags

        // Draw image wiht background
        al_draw_scaled_bitmap(
                image_with_background,             // image to load
                0,                               // start x coord
                0,                               // start y coord
                image_with_background_size.x,        // image x width
                image_with_background_size.y,       // image y height
                0,                               // destination x coord
                0,                               // destination y coord
                image_with_background_size.x * 1.25,  // destination x width
                image_with_background_size.y * 1.25, // destination y height
                0                                // flags
                );

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
