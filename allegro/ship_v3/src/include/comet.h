#ifndef COMET_H
#define COMET_H

#include <allegro5/allegro.h>
#include "object.h"
#include "explosion.h"
#include "ship.h"

typedef struct comet
{
    bool live;
    int speed;
    object_limit_t limit;
    object_point_t position;
    object_point_t bound;
    object_sprite_sheet_t sheet;
} comet_t;

void comet_init(comet_t comets[], int comet_count, ALLEGRO_BITMAP *image, object_limit_t limit);

void comet_draw(comet_t comets[], int comet_count);

void comet_start(comet_t comets[], int comet_count);

void comet_update(comet_t comets[], int comet_count, int timerCount);

void comet_collide(comet_t comets[], int comet_count, ship_t *ship, explosion_t explosions[], int explosion_count);

void comet_reset(comet_t comets[], int comet_count);

#endif /* COMET_H */
