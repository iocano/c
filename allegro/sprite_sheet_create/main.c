#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>

typedef struct point
{
  float x;
  float y;
}point_t;

void exit_if(bool condition, const char *format, ...);

int main()
{
    point_t window_size = {640, 480};
    point_t image_size = {0, 0};

    const int sprite_count = 9;

    ALLEGRO_BITMAP *image[sprite_count];
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_BITMAP *sprite_sheet = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(window_size.x, window_size.y)), "Can't create display\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");

    exit_if(!(image[0] = al_load_bitmap("ship/ship_1.png")), "Can't load ship/ship_1.png\n");
    exit_if(!(image[1] = al_load_bitmap("ship/ship_2.png")), "Can't load ship/ship_2.png\n");
    exit_if(!(image[2] = al_load_bitmap("ship/ship_3.png")), "Can't load ship/ship_3.png\n");

    exit_if(!(image[3] = al_load_bitmap("ship/ship_1.png")), "Can't load ship/ship_1.png\n");
    exit_if(!(image[4] = al_load_bitmap("ship/ship_2.png")), "Can't load ship/ship_2.png\n");
    exit_if(!(image[5] = al_load_bitmap("ship/ship_3.png")), "Can't load ship/ship_3.png\n");

    exit_if(!(image[6] = al_load_bitmap("ship/ship_1.png")), "Can't load ship/ship_1.png\n");
    exit_if(!(image[7] = al_load_bitmap("ship/ship_2.png")), "Can't load ship/ship_2.png\n");
    exit_if(!(image[8] = al_load_bitmap("ship/ship_3.png")), "Can't load ship/ship_3.png\n");

    image_size.x = al_get_bitmap_width(image[0]);
    image_size.y = al_get_bitmap_height(image[0]);
    sprite_sheet = al_create_bitmap(image_size.x * sprite_count, image_size.y);

    al_set_target_bitmap(sprite_sheet);
    al_clear_to_color(al_map_rgba(0, 0, 0, 0));
    for (int i = 0; i < sprite_count; i++)
    {
        al_draw_bitmap(image[i], i * image_size.x, 0, 0);
    }
    al_save_bitmap("sprite_sheet.png", sprite_sheet);

    // Return to display buffer
    al_set_target_bitmap(al_get_backbuffer(display));

    al_clear_to_color(al_map_rgb(30, 30, 30));
    al_draw_bitmap(sprite_sheet, image_size.x, window_size.y / 2 - image_size.y / 2, 0);
    al_flip_display();
    al_rest(5.0);

    // Release resources
    for (int i = 0; i < sprite_count; i++)
    {
        al_destroy_bitmap(image[i]);
    }
    al_destroy_bitmap(sprite_sheet);
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
