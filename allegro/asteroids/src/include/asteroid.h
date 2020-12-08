#ifndef ASTEROID_H
#define ASTEROID_H

#include <allegro5/allegro.h>
#include "../linked_list/linked_list.h"
#include "object.h"
#include "ship.h"

typedef enum asteroid_size
{
    ASTEROID_SIZE_SMALL = 70,
    ASTEROID_SIZE_MEDIUM = 80,
    ASTEROID_SIZE_LARGE = 100
} asteroid_size_t;

typedef struct asteroid
{
    bool is_alive;
    asteroid_size_t size;
    object_direction_t direction;
    object_speed_t speed;
    object_limit_t limit;
    object_size_t bound;
    object_point_t position;
    object_point_t displacement;
    ALLEGRO_COLOR color;
}asteroid_t;

linked_list_t* asteroid_init(int asteroid_count, object_limit_t limit);

void asteroid_reset(linked_list_t *asteroid_list, int asteroid_count);

void asteroid_add(int asteroid_count, linked_list_t *asteroid_list, object_limit_t limit);

void asteroid_update(linked_list_t *asteroid_list);

object_collision_result_t asteroid_is_collided(linked_list_t *asteroid_list, linked_list_t *bullet_list, ship_t *ship);

void asteroid_draw(linked_list_t *asteroid_list);

void asteroid_destroy(linked_list_t *asteroid_list);

#endif /* ASTEROID_H */
