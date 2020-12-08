#ifndef COMET_H
#define COMET_H

#include "object.h"
#include "ship.h"
#include "explosion.h"

typedef struct comet
{
    bool live;
    int speed;
    object_point_t position;
    object_size_t bound;
    object_limit_t movement_limit;
    object_sprite_sheet_t sheet;
} comet_t;

void comet_init(comet_t comets[], int comet_count, ALLEGRO_BITMAP *sprite_sheet, object_limit_t movement_limit);

void comet_draw(comet_t comets[], int comet_count);

void comet_start(comet_t comets[], int comet_count);

void comet_update(comet_t comets[], int comet_count, int timerCount);

void comet_collide(comet_t comets[], int comet_count, ship_t *ship, explosion_t explosions[], int explosion_count);

#endif /* COMET_H */
