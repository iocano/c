#ifndef SHIP_H
#define SHIP_H

#include <allegro5/allegro.h>
#include <stdbool.h>
#include "../linked_list/linked_list.h"
#include "object.h"

typedef struct ship
{
   int score;
   int lives;
   object_timer_t inmunity;
   bool is_accelerating;
   object_size_t bound;
   object_limit_t limit;
   object_speed_t speed;
   object_direction_t direction;
   object_point_t displacement;
   object_point_t position;
   ALLEGRO_COLOR color;
}ship_t;

ship_t* ship_init(object_limit_t limit);

void ship_reset(ship_t *ship);

void ship_accelerate(ship_t *ship);

void ship_inertia(ship_t *ship);

void ship_turn_right(ship_t *ship);

void ship_turn_left(ship_t *ship);

void ship_update(ship_t *ship);

void ship_draw(ship_t *ship);

object_collision_result_t ship_is_collided(ship_t *ship, linked_list_t *asteroid_list);

void ship_destroy(ship_t *ship);

#endif /* SHIP_H */
