#include <math.h>
#include <allegro5/allegro_primitives.h>
#include "include/asteroid.h"
#include "include/bullet.h"
#include "include/utils.h"

static void asteroid_check_limit(asteroid_t *asteroid);

static void asteroid_initial_position(asteroid_t *asteroid);

void asteroid_split(asteroid_t *parent, linked_list_t *asteroid_list, bullet_t *bullet)
{
    int children_count = 2;
    for (int i = 0; i < children_count; i++)
    {
        asteroid_t *child = malloc(sizeof(asteroid_t));
        if (parent->size == ASTEROID_SIZE_LARGE)
        {
            child->size = ASTEROID_SIZE_MEDIUM;
        }
        else if (parent->size == ASTEROID_SIZE_MEDIUM)
        {
            child->size = ASTEROID_SIZE_SMALL;
        }
        child->is_alive = true;
        child->speed = parent->speed;
        child->limit = parent->limit;
        child->bound.width = 20 * child->size / 100;
        child->bound.height = 20 * child->size / 100;
        child->position = parent->position;
        child->color = parent->color;

        float slope = bullet->displacement.y / bullet->displacement.x;
        child->direction.current = atan(slope);
        child->direction.increment = parent->direction.increment;
        //use parent maximum speed to make the children go in different direction
        parent->speed.maximum *= -1;
        child->displacement.x = sin(child->direction.current) * parent->speed.maximum;
        child->displacement.y = cos(child->direction.current) * parent->speed.maximum;

        linked_list_add_last(child, sizeof(asteroid_t), asteroid_list);
    }
}

linked_list_t* asteroid_init(int asteroid_count, object_limit_t limit)
{
    linked_list_t *list = linked_list_create();
    for (int i = 0; i < asteroid_count; i++)
    {
        asteroid_t *asteroid = malloc(sizeof(asteroid_t));
        asteroid->speed.maximum = 3;
        asteroid->direction.increment = M_PI / 64;
        asteroid->limit = limit;
        asteroid->size = ASTEROID_SIZE_LARGE;
        asteroid->color = al_map_rgb(200, 200, 200);
        asteroid->is_alive = true;
        asteroid->direction.current = random_float_inclusive(0, M_PI * 2);
        asteroid->bound.width = 20 * asteroid->size / 100;
        asteroid->bound.height = 20 * asteroid->size / 100;
        int sign[] = {-1, 1};
        asteroid->displacement.x = sin(asteroid->direction.current) * asteroid->speed.maximum * sign[random_int_inclusive(0, 1)];
        asteroid->displacement.y = cos(asteroid->direction.current) * asteroid->speed.maximum * sign[random_int_inclusive(0, 1)];
        asteroid_initial_position(asteroid);
        linked_list_add_last(asteroid, sizeof(asteroid), list);
    }
    return list;
}

void asteroid_add(int asteroid_count, linked_list_t *asteroid_list, object_limit_t limit)
{
    for (int i = 0; i < asteroid_count; i++)
    {
        asteroid_t *asteroid = malloc(sizeof(asteroid_t));
        asteroid->speed.maximum = 3;
        asteroid->direction.increment = M_PI / 64;
        asteroid->limit = limit;
        asteroid->size = ASTEROID_SIZE_LARGE;
        asteroid->color = al_map_rgb(200, 200, 200);
        asteroid->is_alive = true;
        asteroid->direction.current = random_float_inclusive(0, M_PI * 2);
        asteroid->bound.width = 20 * asteroid->size / 100;
        asteroid->bound.height = 20 * asteroid->size / 100;
        int sign[] = {-1, 1};
        asteroid->displacement.x = sin(asteroid->direction.current) * asteroid->speed.maximum * sign[random_int_inclusive(0, 1)];
        asteroid->displacement.y = cos(asteroid->direction.current) * asteroid->speed.maximum * sign[random_int_inclusive(0, 1)];
        asteroid_initial_position(asteroid);
        linked_list_add_last(asteroid, sizeof(asteroid), asteroid_list);
    }
}

void asteroid_reset(linked_list_t *asteroid_list, int asteroid_count)
{
    while (asteroid_list->count > asteroid_count)
    {
        linked_list_remove_node(asteroid_list->tail, asteroid_list, NULL);
    }
    for (node_t *current = asteroid_list->head; current != NULL; current = current->next)
    {
        asteroid_t *asteroid = current->data;
        asteroid->size = ASTEROID_SIZE_LARGE;
        asteroid->is_alive = true;
        asteroid->direction.current = random_float_inclusive(0, M_PI * 2);
        int sign[] = {-1, 1};
        asteroid->displacement.x = sin(asteroid->direction.current) * asteroid->speed.maximum * sign[random_int_inclusive(0, 1)];
        asteroid->displacement.y = cos(asteroid->direction.current) * asteroid->speed.maximum * sign[random_int_inclusive(0, 1)];
        asteroid_initial_position(asteroid);
    }
}

void asteroid_update(linked_list_t *asteroid_list)
{
    for (node_t *current = asteroid_list->head; current != NULL; current = current->next)
    {
        asteroid_t *asteroid = current->data;
        if (asteroid->is_alive == true)
        {
            asteroid->direction.current += asteroid->direction.increment;
            asteroid->position.x += asteroid->displacement.x;
            asteroid->position.y += asteroid->displacement.y;
            asteroid_check_limit(asteroid);
        }
    }
}

object_collision_result_t asteroid_is_collided(linked_list_t *asteroid_list, linked_list_t *bullet_list, ship_t *ship)
{
    object_collision_result_t result = {false, 0};
    for (node_t *asteroid_list_node = asteroid_list->head; asteroid_list_node != NULL; asteroid_list_node = asteroid_list_node->next )
    {
        asteroid_t *asteroid = asteroid_list_node->data;
        if (asteroid->is_alive == true)
        {
            for (node_t *bullet_list_node = bullet_list->head; bullet_list_node != NULL; bullet_list_node = bullet_list_node->next)
            {
                bullet_t *bullet = bullet_list_node->data;
                if (bullet->is_alive == true)
                {
                    bool bullet_inside_left_asteroid_bound = bullet->position.x > (asteroid->position.x - asteroid->bound.width);
                    bool bullet_inside_right_asteroid_bound = bullet->position.x < (asteroid->position.x + asteroid->bound.width);
                    bool bullet_inside_upper_asteroid_bound = bullet->position.y > (asteroid->position.y - asteroid->bound.height);
                    bool bullet_inside_lower_asteroid_bound = bullet->position.y < (asteroid->position.y + asteroid->bound.height);
                    if (bullet_inside_left_asteroid_bound && bullet_inside_right_asteroid_bound && bullet_inside_upper_asteroid_bound && bullet_inside_lower_asteroid_bound)
                    {
                        ship->score++;
                        bullet->is_alive = false;
                        asteroid->is_alive = false;
                        if (asteroid->size == ASTEROID_SIZE_LARGE || asteroid->size == ASTEROID_SIZE_MEDIUM)
                        {
                            asteroid_split(asteroid, asteroid_list, bullet);
                        }
                        /* linked_list_remove_node(asteroid_list_node, asteroid_list, NULL); */
                        result.is_collided = true;
                        result.position = asteroid->position;
                        return result;
                    }
                }
            }
        }
    }
    return result;
}

void asteroid_draw(linked_list_t *asteroid_list)
{
    for (node_t *current = asteroid_list->head; current != NULL; current = current->next)
    {
        asteroid_t *asteroid = current->data;
        if (asteroid->is_alive == true)
        {
            /* ALLEGRO_TRANSFORM transform1; */
            /* al_identity_transform(&transform1); */
            /* al_use_transform(&transform1); */
            /* int upper_left_corner_x = asteroid->position.x - asteroid->bound.width; */
            /* int upper_left_corner_y = asteroid->position.y - asteroid->bound.height; */
            /* int lower_right_corner_x = asteroid->position.x + asteroid->bound.width; */
            /* int lower_right_corner_y = asteroid->position.y + asteroid->bound.height; */
            /* al_draw_filled_rectangle(upper_left_corner_x, upper_left_corner_y, lower_right_corner_x, lower_right_corner_y, al_map_rgb(255, 0, 0)); */

            ALLEGRO_TRANSFORM transform;
            al_identity_transform(&transform);
            al_scale_transform(&transform, asteroid->size / 100.0, asteroid->size / 100.0);
            al_rotate_transform(&transform, asteroid->direction.current);
            al_translate_transform(&transform, asteroid->position.x, asteroid->position.y);
            al_use_transform(&transform);

            ALLEGRO_COLOR color = asteroid->color;
            al_draw_line(-20, 20, -25, 5, color, 2.0f);
            al_draw_line(-25, 5, -25, -10, color, 2.0f);
            al_draw_line(-25, -10, -5, -10, color, 2.0f);
            al_draw_line(-5, -10, -10, -20, color, 2.0f);
            al_draw_line(-10, -20, 5, -20, color, 2.0f);
            al_draw_line(5, -20, 20, -10, color, 2.0f);
            al_draw_line(20, -10, 20, -5, color, 2.0f);
            al_draw_line(20, -5, 0, 0, color, 2.0f);
            al_draw_line(0, 0, 20, 10, color, 2.0f);
            al_draw_line(20, 10, 10, 20, color, 2.0f);
            al_draw_line(10, 20, 0, 15, color, 2.0f);
            al_draw_line(0, 15, -20, 20, color, 2.0f);
        }
    }
}

void asteroid_destroy(linked_list_t *asteroid_list)
{
    node_t *current = asteroid_list->head;
    node_t *next = NULL;
    while(current != NULL)
    {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(asteroid_list);
}

static void asteroid_check_limit(asteroid_t *asteroid)
{
    if(asteroid->position.x + asteroid->bound.width < asteroid->limit.lower.x)
    {
        asteroid->position.x = asteroid->limit.upper.x + asteroid->bound.width;
    }
    else if(asteroid->position.x - asteroid->bound.width > asteroid->limit.upper.x)
    {
        asteroid->position.x = asteroid->limit.lower.x - asteroid->bound.width;
    }

    if(asteroid->position.y + asteroid->bound.height < asteroid->limit.lower.y )
    {
        asteroid->position.y = asteroid->limit.upper.y + asteroid->bound.width;
    }
    else if(asteroid->position.y - asteroid->bound.height > asteroid->limit.upper.y)
    {
        asteroid->position.y = asteroid->limit.lower.y - asteroid->bound.height;
    }
}

static void asteroid_initial_position(asteroid_t *asteroid)
{
    int random_edge = random_int_inclusive(0, 3);
    switch (random_edge)
    {
        case 0: // LEFT
            asteroid->position.x = 0;
            asteroid->position.y = random_int_inclusive(0, asteroid->limit.upper.y);
            break;
        case 1: // TOP
            asteroid->position.x = random_int_inclusive(0, asteroid->limit.upper.x);
            asteroid->position.y = 0;
            break;
        case 2: // RIGHT
            asteroid->position.x = asteroid->limit.upper.x;
            asteroid->position.y = random_int_inclusive(0, asteroid->limit.upper.y);
            break;
        case 3: // DOWN
            asteroid->position.x = random_int_inclusive(0, asteroid->limit.upper.x);
            asteroid->position.y = asteroid->limit.upper.y;
            break;
    }
}
