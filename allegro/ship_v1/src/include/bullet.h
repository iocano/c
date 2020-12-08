#ifndef BULLET_H
#define BULLET_H

#include "object.h"
#include "ship.h"
#include "comet.h"

typedef struct bullet
{
    object_coordinate_t position;
    object_limit_t limit;
    bool live;
    int speed;
}bullet_t;

bullet_t** bullet_array_init(int bullet_count, object_limit_t limit);

void bullet_array_draw(bullet_t **bullets, int bullet_count);

void bullet_array_fire(bullet_t **bullets, int bullet_count, ship_t *ship);

void bullet_array_update(bullet_t **bullets, int bullet_count);

void bullet_array_collide(bullet_t **bullets, int bullet_count, comet_t **comets, int comet_count, ship_t *ship);

void bullet_array_destroy(bullet_t **bullets, int bullet_count);

#endif /* BULLET_H */
