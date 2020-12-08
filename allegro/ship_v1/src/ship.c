#include <stdlib.h>
#include "include/ship.h"

static void ship_draw_pixel(float x_start, float x_end, float y_start, float y_end, ALLEGRO_COLOR color);

static ALLEGRO_BITMAP* ship_render_image(ALLEGRO_DISPLAY *display);

ship_t* ship_init(object_coordinate_t position, object_limit_t limit, ALLEGRO_DISPLAY * display)
{
    ship_t *ship = malloc(sizeof(ship_t));
    ship->position = position;
    ship->limit = limit;
    ship->bound = (object_coordinate_t){28, 10};
    ship->lives = 3;
    ship->speed = 7;
    ship->score = 0;
    ship->image = ship_render_image(display);
    return ship;
}

void ship_draw(ship_t *ship)
{
    al_draw_scaled_bitmap(ship->image, 0, 0, al_get_bitmap_width(ship->image), al_get_bitmap_height(ship->image), ship->position.x - 30, ship->position.y - 30, 60, 60, 0);
    /* al_draw_rectangle(ship->position.x - ship->bound.x, ship->position.y - ship->bound.y, ship->position.x + ship->bound.x, ship->position.y + ship->bound.y, al_map_rgb(255, 0, 0), 0); */
}

void ship_move_up(ship_t *ship)
{
    ship->position.y -= ship->speed;
    if (ship->position.y < ship->limit.lower.y)
    {
        ship->position.y = ship->limit.lower.y;
    }
}

void ship_move_down(ship_t *ship)
{
    ship->position.y += ship->speed;
    if (ship->position.y > ship->limit.upper.y)
    {
        ship->position.y = ship->limit.upper.y;
    }
}

void ship_move_left(ship_t *ship)
{
    ship->position.x -= ship->speed;
    if (ship->position.x < ship->limit.lower.x)
    {
        ship->position.x = ship->limit.lower.x;
    }
}

void ship_move_right(ship_t *ship)
{
    ship->position.x += ship->speed;
    if (ship->position.x > ship->limit.upper.x)
    {
        ship->position.x = ship->limit.upper.x;
    }
}

void ship_destroy(ship_t *ship)
{
    al_destroy_bitmap(ship->image);
    free(ship);
}

static void ship_draw_pixel(float x_start, float x_end, float y_start, float y_end, ALLEGRO_COLOR color)
{
    for (float i = x_start; i <= x_end; i++)
    {
        for(float j = y_start; j <= y_end; j++)
        {
            al_draw_pixel(i, j, color);
        }
    }
}

static ALLEGRO_BITMAP* ship_render_image(ALLEGRO_DISPLAY *display)
{
    int image_side_length = 40;
    ALLEGRO_BITMAP *image_half_ship = NULL;
    ALLEGRO_BITMAP *image_full_ship = NULL;
    // Draw half ship
    {
        image_half_ship = al_create_bitmap(image_side_length, image_side_length / 2);

        // Set target where are you going to draw
        al_set_target_bitmap(image_half_ship);

        // Draw to bitmap
        al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));

        ALLEGRO_COLOR color1 = al_map_rgb(255, 255, 255);
        ship_draw_pixel(1, 9, 14, 14, color1);
        ship_draw_pixel(11, 12, 19, 19, color1);
        ship_draw_pixel(13, 16, 17, 17, color1);
        ship_draw_pixel(17, 17, 18, 18, color1);
        ship_draw_pixel(18, 35, 19, 19, color1);
        ship_draw_pixel(12, 13, 18, 18, color1);

        ALLEGRO_COLOR color2 = al_map_rgb(230, 230, 230);
        ship_draw_pixel(2, 4, 13, 13, color2);
        ship_draw_pixel(12, 12, 17, 17, color2);
        ship_draw_pixel(13, 16, 16, 16, color2);
        ship_draw_pixel(17, 17, 17, 17, color2);
        ship_draw_pixel(14, 15, 18, 18, color2);
        ship_draw_pixel(18, 18, 18, 18, color2);
        ship_draw_pixel(20, 36, 18, 18, color2);
        ship_draw_pixel(13, 16, 19, 19, color2);
        ship_draw_pixel(36, 37, 19, 19, color2);
        ship_draw_pixel(7, 7, 18, 18, color2);
        ship_draw_pixel(8, 8, 17, 17, color2);
        ship_draw_pixel(9, 9, 16, 16, color2);
        ship_draw_pixel(18, 19, 14, 14, color2);
        ship_draw_pixel(18, 19, 15, 15, color2);
        ship_draw_pixel(18, 19, 16, 16, color2);

        ALLEGRO_COLOR color3 = al_map_rgb(190, 190, 190);
        ship_draw_pixel(11, 11, 12, 12, color3);
        ship_draw_pixel(13, 15, 12, 12, color3);
        ship_draw_pixel(5, 7, 13, 13, color3);
        ship_draw_pixel(10, 10, 13, 13, color3);
        ship_draw_pixel(15, 15, 13, 13, color3);
        ship_draw_pixel(15, 17, 14, 14, color3);
        ship_draw_pixel(5, 7, 15, 15, color3);
        ship_draw_pixel(17, 17, 15, 15, color3);
        ship_draw_pixel(19, 35, 17, 17, color3);

        ALLEGRO_COLOR color4 = al_map_rgb(100, 100, 100);
        ship_draw_pixel(12, 15, 11, 11, color4);
        ship_draw_pixel(10, 10, 12, 12, color4);
        ship_draw_pixel(16, 16, 12, 12, color4);
        ship_draw_pixel(19, 19, 12, 12, color4);
        ship_draw_pixel(8, 9, 13, 13, color4);
        ship_draw_pixel(16, 19, 13, 13, color4);
        ship_draw_pixel(8, 9, 15, 15, color4);
        ship_draw_pixel(13, 16, 15, 15, color4);
        ship_draw_pixel(12, 12, 16, 16, color4);
        ship_draw_pixel(17, 17, 16, 16, color4);
        ship_draw_pixel(11, 11, 17, 17, color4);
        ship_draw_pixel(18, 18, 17, 17, color4);
        ship_draw_pixel(11, 11, 18, 18, color4);
        ship_draw_pixel(19, 19, 18, 18, color4);
        ship_draw_pixel(8, 10, 19, 19, color4);
        ship_draw_pixel(20, 20, 13, 16, color4);
        ship_draw_pixel(21, 31, 16, 16, color4);

        ALLEGRO_COLOR color5 = al_map_rgb(0, 0, 0);
        ship_draw_pixel(16, 16, 18, 18, color5);
        ship_draw_pixel(16, 16, 18, 18, color5);
        ship_draw_pixel(17, 17, 19, 19, color5);

        ALLEGRO_COLOR color6 = al_map_rgb(60, 60, 60);
        ship_draw_pixel(21, 21, 14, 14, color5);
    }

    // Join half ship with its mirror version
    {
        image_full_ship = al_create_bitmap(image_side_length, image_side_length);

        al_set_target_bitmap(image_full_ship);

        al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));
        al_draw_bitmap(image_half_ship, 0 , 0, 0);
        al_draw_bitmap(image_half_ship, 0 , image_side_length / 2, ALLEGRO_FLIP_VERTICAL);
    }

    al_destroy_bitmap(image_half_ship);
    al_set_target_bitmap(al_get_backbuffer(display));
    return image_full_ship;
}
