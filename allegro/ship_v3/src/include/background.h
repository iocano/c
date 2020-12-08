#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <allegro5/allegro5.h>
#include "object.h"

typedef struct background
{
    object_point_t position;
    object_point_t speed;
    object_point_t direction;
    object_point_t size;
    object_limit_t limit;
    ALLEGRO_BITMAP *image;
}background_t;

void background_init(background_t *background, int x, int y, int speed_x, int speed_y, int width, int height, int direction_x, int direction_y, object_limit_t limit, ALLEGRO_BITMAP *image);

void background_update(background_t *background);

void background_draw(background_t *background);

#endif /* BACKGROUND_H */
