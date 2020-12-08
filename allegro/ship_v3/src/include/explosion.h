#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <allegro5/allegro_audio.h>
#include "object.h"

typedef struct explosion
{
    bool live;
    object_point_t position;
    object_sprite_sheet_t sheet;
    ALLEGRO_SAMPLE *sample;
} explosion_t;

void explosion_init(explosion_t explosions[], int explosion_count, ALLEGRO_BITMAP *image, ALLEGRO_SAMPLE *sample);

void explosion_draw(explosion_t explosions[], int explosion_count);

void explosion_start(explosion_t explosions[], int explosion_count, object_point_t position);

void explosion_update(explosion_t explosions[], int explosion_count, int timerCount);

void explosion_reset(explosion_t explosions[], int explosion_count);
#endif /* EXPLOSION_H */
