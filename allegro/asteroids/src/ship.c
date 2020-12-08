#include <allegro5/allegro_primitives.h>
#include <math.h>
#include "include/ship.h"
#include "include/asteroid.h"

static void ship_check_limit(ship_t *ship);

ship_t* ship_init(object_limit_t limit)
{
    ship_t *ship = malloc(sizeof(ship_t));
    ship->limit = limit;
    ship->inmunity.limit = 5;
    ship->speed.maximum = 5;
    ship->speed.acceleration = .098;
    ship->speed.deceleration = .99;
    ship->bound.width = 8;
    ship->bound.height = 8;
    ship->direction.increment = M_PI / 48;
    ship->color = al_map_rgb(200, 200, 200);
    ship_reset(ship);
    return ship;
}

void ship_reset(ship_t *ship)
{
    ship->lives = 3;
    ship->score = 0;
    ship->inmunity.start = 0;
    ship->speed.current = 0;
    ship->is_accelerating = false;
    ship->displacement.x = 0;
    ship->displacement.y = 0;
    ship->direction.current = 0;
    ship->position.x = (ship->limit.upper.x + ship->limit.lower.x) / 2;
    ship->position.y = (ship->limit.upper.y + ship->limit.lower.y) / 2;
}

void ship_accelerate(ship_t *ship)
{
    ship->is_accelerating = true;
    ship->displacement.x += sin(ship->direction.current) * ship->speed.acceleration;
    ship->displacement.y += cos(ship->direction.current) * ship->speed.acceleration;
    ship->speed.current = sqrtf(powf(ship->displacement.x, 2) + powf(ship->displacement.y, 2));
    if (ship->speed.current > ship->speed.maximum)
    {
        ship->displacement.x *= ship->speed.maximum / ship->speed.current;
        ship->displacement.y *= ship->speed.maximum / ship->speed.current;
    }
}

void ship_inertia(ship_t *ship)
{
    ship->is_accelerating = false;
    ship->displacement.x *= ship->speed.deceleration;
    ship->displacement.y *= ship->speed.deceleration;
}

void ship_turn_right(ship_t *ship)
{
    ship->direction.current += ship->direction.increment;
}

void ship_turn_left(ship_t *ship)
{
    ship->direction.current -= ship->direction.increment;
}

void ship_update(ship_t *ship)
{
    ship->position.x += ship->displacement.x;
    ship->position.y -= ship->displacement.y;
    ship_check_limit(ship);
    if(al_current_time() - ship->inmunity.start > ship->inmunity.limit)
    {
        ship->inmunity.start = 0;
    }
}

void ship_draw(ship_t *ship)
{
    /* ALLEGRO_TRANSFORM transform1; */
    /* al_identity_transform(&transform1); */
    /* al_use_transform(&transform1); */
    /* int upper_left_corner_x = ship->position.x - ship->bound.width; */
    /* int upper_left_corner_y = ship->position.y - ship->bound.height; */
    /* int lower_right_corner_x = ship->position.x + ship->bound.width; */
    /* int lower_right_corner_y = ship->position.y + ship->bound.height; */
    /* al_draw_filled_rectangle(upper_left_corner_x, upper_left_corner_y, lower_right_corner_x, lower_right_corner_y, al_map_rgb(255, 0, 0)); */

    // blink show ship for .2 seconds every .1 seconds
    if (ship->inmunity.start == 0 || (ship->inmunity.start > 0 && fmod(al_get_time(), .20) < .1 ))
    {
        ALLEGRO_TRANSFORM transform2;
        al_identity_transform(&transform2);
        al_rotate_transform(&transform2, ship->direction.current);
        al_translate_transform(&transform2, ship->position.x, ship->position.y);
        al_use_transform(&transform2);
        ALLEGRO_COLOR color = ship->color;
        al_draw_line(-8, 9, 0, -11, color, 2.0f);
        al_draw_line(0, -11, 8, 9, color, 2.0f);
        al_draw_line(-6, 4, -1, 4, color, 2.0f);
        al_draw_line(6, 4, 1, 4, color, 2.0f);

        if(ship->is_accelerating)
        {
            float radio = 1;
            float row = 11;
            if(ship->speed.current > 1.25)
            {
                al_draw_filled_circle(-6, row, radio, color);
                al_draw_filled_circle(-2, row, radio, color);
                al_draw_filled_circle(2, row, radio, color);
                al_draw_filled_circle(6, row, radio, color);
            }
            if(ship->speed.current > 2.50)
            {
                row = 13;
                al_draw_filled_circle(-4, row, radio, color);
                al_draw_filled_circle(0, row, radio, color);
                al_draw_filled_circle(4, row, radio, color);
            }
            if(ship->speed.current > 3.75)
            {
                row = 15;
                al_draw_filled_circle(-2, row, radio, color);
                al_draw_filled_circle(2, row, radio, color);
            }
            if(ship->speed.current >=5)
            {
                row = 17;
                al_draw_filled_circle(0, row, radio, color);
            }
        }
    }
}

object_collision_result_t ship_is_collided(ship_t *ship, linked_list_t *asteroid_list)
{
    object_collision_result_t result;
    result.is_collided = false;
    if (ship->inmunity.start > 0)
    {
        return result;
    }

    for (node_t *current = asteroid_list->head; current != NULL; current = current->next)
    {
        asteroid_t *asteroid = current->data;
        if (asteroid->is_alive == true)
        {
            bool asteroid_is_inside_left_ship_bound = (asteroid->position.x + asteroid->bound.width) > (ship->position.x - ship->bound.width);
            bool asteroid_is_inside_right_ship_bound = (asteroid->position.x - asteroid->bound.width) < (ship->position.x + ship->bound.width);
            bool asteroid_is_inside_lower_ship_bound = (asteroid->position.y + asteroid->bound.height) > (ship->position.y - ship->bound.height);
            bool asteroid_is_inside_upper_ship_bound = (asteroid->position.y - asteroid->bound.height) < (ship->position.y + ship->bound.height);

            if (asteroid_is_inside_left_ship_bound && asteroid_is_inside_right_ship_bound && asteroid_is_inside_lower_ship_bound && asteroid_is_inside_upper_ship_bound)
            {
                ship->lives--;
                ship->inmunity.start = al_current_time();
                asteroid->is_alive = false;
                result.is_collided = true;
                result.position = ship->position;
                return result;
            }
        }
    }
    return result;
}

void ship_destroy(ship_t *ship)
{
    free(ship);
}

static void ship_check_limit(ship_t *ship)
{
    if(ship->position.x + ship->bound.width < ship->limit.lower.x)
    {
        ship->position.x = ship->limit.upper.x + ship->bound.width;
    }
    else if(ship->position.x - ship->bound.width > ship->limit.upper.x)
    {
        ship->position.x = ship->limit.lower.x - ship->bound.width;
    }

    if(ship->position.y + ship->bound.height < ship->limit.lower.y )
    {
        ship->position.y = ship->limit.upper.y + ship->bound.height;
    }
    else if(ship->position.y - ship->bound.height > ship->limit.upper.y)
    {
        ship->position.y = ship->limit.lower.y - ship->bound.height;
    }
}

