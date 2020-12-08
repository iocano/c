#ifndef SHIP_H
#define SHIP_H

#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include "object.h"

typedef struct ship
{
    int lives;
    int speed;
    int score;
    object_point_t position;
    object_point_t bound;
    object_limit_t limit;
    object_sprite_sheet_t sheet;
    ALLEGRO_SAMPLE *sample;
} ship_t;

void ship_init(ship_t *ship, object_limit_t limit, ALLEGRO_BITMAP *image);

void ship_reset(ship_t *ship);

void ship_reset_animation(ship_t *ship);

void ship_draw(ship_t *ship);

void ship_move_up(ship_t *ship);

void ship_move_down(ship_t *ship);

void ship_move_left(ship_t *ship);

void ship_move_right(ship_t *ship);

#endif /* SHIP_H */
