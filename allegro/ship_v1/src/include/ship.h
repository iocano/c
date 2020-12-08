#ifndef SHIP_H
#define SHIP_H

#include "object.h"
#include <allegro5/allegro_primitives.h>

typedef struct ship
{
    object_coordinate_t position;
    object_coordinate_t bound;
    object_limit_t limit;
    ALLEGRO_BITMAP *image;
    int lives;
    int speed;
    int score;
} ship_t;

ship_t* ship_init(object_coordinate_t position, object_limit_t limit, ALLEGRO_DISPLAY * display);

void ship_draw(ship_t *ship);

void ship_move_up(ship_t *ship);

void ship_move_down(ship_t *ship);

void ship_move_left(ship_t *ship);

void ship_move_right(ship_t *ship);

void ship_destroy(ship_t *ship);

#endif /* SHIP_H */
