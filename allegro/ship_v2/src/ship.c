#include "include/ship.h"

void ship_init(ship_t *ship, ALLEGRO_BITMAP *sprite_sheet, object_limit_t movement_limit)
{
    ship->position = (object_point_t){movement_limit.lower.x + 20, movement_limit.upper.y / 2};
    ship->movement_limit = movement_limit;
    ship->bound = (object_size_t){10, 10};
    ship->lives = 3;
    ship->speed = 7;
    ship->score = 0;

    ship->sheet.sprite.current = 3;
    ship->sheet.sprite.width = 46;
    ship->sheet.sprite.height = 41;
    ship->sheet.direction = 1;
    ship->sheet.columns = 3;
    ship->sheet.rows = 3;
    ship->sheet.image = sprite_sheet;
}

void ship_draw(ship_t *ship)
{
    int bitmap_region_x = (ship->sheet.sprite.current % ship->sheet.columns) * ship->sheet.sprite.width;
    int bitmap_region_y = (ship->sheet.sprite.current / ship->sheet.columns) * ship->sheet.sprite.height;

    al_draw_bitmap_region(
        ship->sheet.image,
        bitmap_region_x,
        bitmap_region_y,
        ship->sheet.sprite.width,
        ship->sheet.sprite.height,
        ship->position.x - ship->sheet.sprite.width / 2,
        ship->position.y - ship->sheet.sprite.height / 2,
        0);

    // al_draw_filled_rectangle(
    //     ship->x - ship->boundx,
    //     ship->y - ship->boundy,
    //     ship->x + ship->boundx,
    //     ship->y + ship->boundy,
    //     al_map_rgba(255, 0, 255, 0));
}

void ship_move_up(ship_t *ship)
{
    int row = 0;
    int column = ship->sheet.sprite.current % ship->sheet.columns;
    ship->sheet.sprite.current = row * ship->sheet.rows + column;

    if (ship->position.y - ship->speed - ship->sheet.sprite.height / 3 >= ship->movement_limit.lower.y)
    {
        ship->position.y -= ship->speed;
    }
}

void ship_move_down(ship_t *ship)
{
    int row = 2;
    int column = ship->sheet.sprite.current % ship->sheet.columns;
    ship->sheet.sprite.current = row * ship->sheet.rows + column;

    if (ship->position.y + ship->speed + ship->sheet.sprite.height / 3 <= ship->movement_limit.upper.y)
    {
        ship->position.y += ship->speed;
    }
}

void ship_move_left(ship_t *ship)
{
    int row = ship->sheet.sprite.current / ship->sheet.columns;
    int column = 2;
    ship->sheet.sprite.current = row * ship->sheet.rows + column;

    if (ship->position.x - ship->sheet.sprite.width / 2 > ship->movement_limit.lower.x)
    {
        ship->position.x -= ship->speed;
    }
}

void ship_move_right(ship_t *ship)
{
    int row = ship->sheet.sprite.current / ship->sheet.columns;
    int column = 1;
    ship->sheet.sprite.current = row * ship->sheet.rows + column;

    if (ship->position.x + ship->speed <= ship->movement_limit.upper.x)
    {
        ship->position.x += ship->speed;
    }
}

void ship_reset_animation(ship_t *ship)
{
    int row = 1;
    int column = ship->sheet.sprite.current % ship->sheet.columns;
    ship->sheet.sprite.current = row * ship->sheet.rows + column;
}
