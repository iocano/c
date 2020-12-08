#ifndef SHIP_H
#define SHIP_H

#include <allegro5/allegro5.h>
#include "object.h"

typedef struct ship
{
    int lives;
    int speed;
    int score;
    object_point_t position;
    object_size_t bound;
    object_limit_t movement_limit;
    object_sprite_sheet_t sheet;
} ship_t;

void ship_init(ship_t *ship, ALLEGRO_BITMAP *sprite_sheet, object_limit_t movement_limit);

void ship_draw(ship_t *ship);

void ship_move_up(ship_t *ship);

void ship_move_down(ship_t *ship);

void ship_move_left(ship_t *ship);

void ship_move_right(ship_t *ship);

void ship_reset_animation(ship_t *ship);

#endif /* SHIP_H */
