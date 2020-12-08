#include <allegro5/allegro.h>
#include "../linked_list/linked_list.h"
#include "object.h"

typedef struct explosion_particles
{
    int count;
    object_point_t *points;
}explosion_particles_t;

typedef struct explosion
{
    bool is_alive;
    object_radius_t radius;
    object_point_t position;
    object_direction_t angle;
    object_timer_t live;
    explosion_particles_t particle;
    object_color_t color;
}explosion_t;

linked_list_t* explosion_init(int explosion_count);

void explosion_reset(linked_list_t *explosion_list);

void explosion_start(linked_list_t *explosion_list, object_point_t initial_point);

void explosion_update(linked_list_t *explosion_list);

void explosion_draw(linked_list_t *explosion_list);

void explosion_destroy(linked_list_t *explosion_list);
