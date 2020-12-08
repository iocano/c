#ifndef COMET_H
#define COMET_H

#include "object.h"
#include "ship.h"

typedef struct comet
{
    object_coordinate_t position;
    object_coordinate_t bound;
    object_limit_t limit;
    bool live;
    int speed;
}comet_t;

comet_t** comet_array_init(int comet_count, object_limit_t limit);

void comet_array_draw(comet_t **comets, int comet_count);

void comet_array_start(comet_t **comets, int comet_count);

void comet_array_update(comet_t **comets, int comet_count);

void comet_array_collide(comet_t **comets, int comet_count, ship_t *ship);

void commet_array_free(comet_t **comets, int comet_count);
#endif /* COMET_H */
