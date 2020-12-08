#include <math.h>
#include <allegro5/allegro_primitives.h>
#include "include/explosion.h"
#include "include/utils.h"


linked_list_t* explosion_init(int explosion_count)
{
    int particle_count = 100;
    linked_list_t *list = linked_list_create();
    for (int i = 0; i < explosion_count; i++)
    {
        explosion_t *explosion = malloc(sizeof(explosion_t));
        explosion->is_alive = false;
        explosion->radius.minimum = 0;
        explosion->radius.maximum = 2;
        explosion->angle.current = 0.0;
        explosion->angle.increment = M_PI * 2 / particle_count;
        explosion->live.start = 0;
        explosion->live.limit = 1;
        explosion->particle.count = particle_count;
        explosion->particle.points = malloc(sizeof(object_point_t) * particle_count);
        explosion->color = (object_color_t){200, 200, 200};
        linked_list_add_last(explosion, sizeof(explosion_t), list);
    }
    return list;
}

void explosion_reset(linked_list_t *explosion_list)
{
    for (node_t *current = explosion_list->head; current != NULL; current = current->next)
    {
        explosion_t *explosion = current->data;
        explosion->is_alive = false;
        explosion->position.x = 0;
        explosion->position.y = 0;
        explosion->angle.current = 0.0;
        explosion->live.start = 0;
    }
}

void explosion_start(linked_list_t *explosion_list, object_point_t initial_point)
{
    for (node_t *current = explosion_list->head; current != NULL; current = current->next)
    {
        explosion_t *explosion = current->data;
        if (explosion->is_alive == false)
        {
            explosion->is_alive = true;
            explosion->live.start = al_current_time();
            explosion->position = initial_point;
            object_point_t origin = {0, 0};
            for (int i = 0; i < explosion->particle.count; i++)
            {
                explosion->particle.points[i] = origin;
            }
            break;
        }
    }
}

void explosion_update(linked_list_t *explosion_list)
{
    for (node_t *current = explosion_list->head; current != NULL; current = current->next)
    {
        explosion_t *explosion = current->data;
        if (explosion->is_alive == true)
        {
            float current_time = al_current_time();
            if(current_time - explosion->live.start < explosion->live.limit)
            {
                float radius = 0;
                for (int i = 0; i < explosion->particle.count; i++)
                {
                    radius = random_float_inclusive(explosion->radius.minimum, explosion->radius.maximum);
                    explosion->particle.points[i].x += sin(explosion->angle.current) * radius;
                    explosion->particle.points[i].y -= cos(explosion->angle.current) * radius;
                    explosion->angle.current += explosion->angle.increment;
                    if(explosion->angle.current >= 2 * M_PI)
                    {
                        explosion->angle.current = 0;
                    }
                }
                // explosion->color.red--;
                // explosion->color.green--;
                // explosion->color.blue--;
            }
            else
            {
                explosion->is_alive = false;
            }
        }
    }
}

void explosion_draw(linked_list_t *explosion_list)
{
    for (node_t *current = explosion_list->head; current != NULL; current = current->next)
    {
        explosion_t *explosion = current->data;
        if (explosion->is_alive == true)
        {
            ALLEGRO_TRANSFORM transform;
            al_identity_transform(&transform);
            al_translate_transform(&transform, explosion->position.x, explosion->position.y);
            al_use_transform(&transform);
            ALLEGRO_COLOR color = al_map_rgb(explosion->color.red, explosion->color.green, explosion->color.blue);
            for (int i = 0; i < explosion->particle.count; i++)
            {
                al_draw_filled_circle(explosion->particle.points[i].x, explosion->particle.points[i].y, 1, color);
            }
        }
    }
}

void explosion_destroy(linked_list_t *explosion_list)
{
    node_t *current = explosion_list->head;
    node_t *next = NULL;
    while(current != NULL)
    {
        next = current->next;
        explosion_t *explosion = current->data;
        free(explosion->particle.points);
        free(explosion);
        free(current);
        current = next;
    }
    free(explosion_list);
}
