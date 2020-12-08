#include "include/ship.h"

void ship_init(ship_t *ship, object_limit_t limit, ALLEGRO_BITMAP *image)
{
    ship->speed = 7;
    ship->bound.x = 10;
    ship->bound.y = 10;
    ship->limit = limit;
    ship_reset(ship);

    ship->sheet.columns = 3;
    ship->sheet.rows = 3;
    ship->sheet.sprite.current = 0;
    ship->sheet.sprite.width = 46;
    ship->sheet.sprite.height = 41;
    ship->sheet.direction = 1;
    ship->sheet.image = image;
}

void ship_draw(ship_t *ship)
{
    int fx = (ship->sheet.sprite.current % ship->sheet.columns) * ship->sheet.sprite.width;
    int fy = (ship->sheet.sprite.current / ship->sheet.columns) * ship->sheet.sprite.height;

    al_draw_bitmap_region(
            ship->sheet.image,
            fx,
            fy,
            ship->sheet.sprite.width,
            ship->sheet.sprite.height,
            ship->position.x - ship->sheet.sprite.width / 2,
            ship->position.y - ship->sheet.sprite.height / 2,
            0);
}

void ship_move_up(ship_t *ship)
{
    int row = 0;
    int column = ship->sheet.sprite.current % ship->sheet.columns;
    ship->sheet.sprite.current = row * ship->sheet.rows + column;

    if (ship->position.y - ship->speed - ship->sheet.sprite.height / 3 >= ship->limit.lower.y)
    {
        ship->position.y -= ship->speed;
    }
}

void ship_move_down(ship_t *ship)
{
    int row = 2;
    int column = ship->sheet.sprite.current % ship->sheet.columns;
    ship->sheet.sprite.current = row * ship->sheet.rows + column;

    if (ship->position.y + ship->speed + ship->sheet.sprite.height / 3 <= ship->limit.upper.y)
    {
        ship->position.y += ship->speed;
    }
}

void ship_move_left(ship_t *ship)
{
    int row = ship->sheet.sprite.current / ship->sheet.columns;
    int column = 2;
    ship->sheet.sprite.current = row * ship->sheet.rows + column;

    if (ship->position.x - ship->sheet.sprite.width / 2 > ship->limit.lower.x)
    {
        ship->position.x -= ship->speed;
    }
}

void ship_move_right(ship_t *ship)
{
    int row = ship->sheet.sprite.current / ship->sheet.columns;
    int column = 1;
    ship->sheet.sprite.current = row * ship->sheet.rows + column;

    if (ship->position.x + ship->speed <= ship->limit.upper.x)
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

void ship_reset(ship_t *ship)
{
    ship->position.x = 20;
    ship->position.y = ship->limit.upper.y / 2;
    ship->lives = 3;
    ship->score = 0;
}
