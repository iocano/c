#ifndef BULLET_H
#define BULLET_H

#include <allegro5/allegro_audio.h>
#include "object.h"
#include "ship.h"
#include "explosion.h"
#include "comet.h"

typedef struct bullet
{
    bool live;
    int speed;
    object_point_t position;
    object_limit_t limit;
    ALLEGRO_SAMPLE *sample;
} bullet_t;

void bullet_init(bullet_t bullets[], int bullet_count, object_limit_t limit, ALLEGRO_SAMPLE *sample);

void bullet_draw(bullet_t bullets[], int cullet_count);

void bullet_fire(bullet_t bullets[], int bullet_count, ship_t *ship);

void bullet_update(bullet_t bullets[], int bullet_count);

void bullet_collide(bullet_t bullets[], int bullet_count, comet_t comets[], int comet_count, ship_t *ship, explosion_t explosions[], int explosion_count);

void bullet_reset(bullet_t bullets[], int bullet_count);

#endif /* BULLET_H */
