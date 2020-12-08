#include <math.h>
#include <allegro5/allegro_primitives.h>
#include "include/bullet.h"

static void bullet_check_limit(bullet_t *bullet);

linked_list_t* bullet_init(int bullet_count, object_limit_t limit)
{
    linked_list_t *list = linked_list_create();
    for (int i = 0; i < bullet_count; i++)
    {
        bullet_t *bullet = malloc(sizeof(bullet_t));
        bullet->is_alive = false;
        bullet->speed.current = 0;
        bullet->speed.maximum = 10;
        bullet->displacement.x = 0;
        bullet->displacement.y = 0;
        bullet->limit = limit;
        bullet->timer.start = 0;
        bullet->timer.limit = 1;
        bullet->color = al_map_rgb(200, 200, 200);
        linked_list_add_last(bullet, sizeof(bullet_t), list);
    }
    return list;
}

void bullet_reset(linked_list_t *bullet_list)
{
    for (node_t *current = bullet_list->head; current != NULL; current = current->next)
    {
        bullet_t *bullet = current->data;
        bullet->is_alive = false;
        bullet->speed.current = 0;
        bullet->displacement.x = 0;
        bullet->displacement.y = 0;
        bullet->timer.start = 0;
    }
}

void bullet_fire(linked_list_t *bullet_list, object_point_t position, object_direction_t direction)
{
    for (node_t *current = bullet_list->head; current != NULL; current = current->next )
    {
        bullet_t *bullet = current->data;
        if(bullet->is_alive == false)
        {
            bullet->is_alive = true;
            bullet->direction.current = direction.current;
            bullet->position = position;
            bullet->displacement.x = sin(bullet->direction.current) * bullet->speed.maximum;
            bullet->displacement.y = cos(bullet->direction.current) * bullet->speed.maximum;
            bullet->timer.start = al_current_time();
            break;
        }
    }
}

void bullet_update(linked_list_t *bullet_list)
{
    for(node_t *current = bullet_list->head; current != NULL; current = current->next)
    {
        bullet_t *bullet = current->data;
        if(bullet->is_alive == true)
        {
            bullet->position.x += bullet->displacement.x;
            bullet->position.y -= bullet->displacement.y;
            bullet_check_limit(bullet);
            float current_time = al_current_time();
            if(current_time - bullet->timer.start >= bullet->timer.limit)
            {
                bullet->is_alive = false;
            }
        }
    }
}

void bullet_draw(linked_list_t *bullet_list)
{
    for (node_t *current = bullet_list->head; current != NULL; current = current->next)
    {
        bullet_t *bullet = current->data;
        if (bullet->is_alive == true)
        {
            ALLEGRO_TRANSFORM transform;
            al_identity_transform(&transform);
            al_translate_transform(&transform, bullet->position.x, bullet->position.y);
            al_use_transform(&transform);
            al_draw_filled_circle(0, 0, 2, bullet->color);
        }
    }
}

void bullet_destroy(linked_list_t *bullet_list)
{
    node_t *current = bullet_list->head;
    node_t *next = NULL;
    while(current != NULL)
    {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
    free(bullet_list);
}

static void bullet_check_limit(bullet_t *bullet)
{
    if(bullet->position.x < bullet->limit.lower.x)
    {
        bullet->position.x = bullet->limit.upper.x;
    }
    else if(bullet->position.x > bullet->limit.upper.x)
    {
        bullet->position.x = bullet->limit.lower.x;
    }

    if(bullet->position.y < bullet->limit.lower.y )
    {
        bullet->position.y = bullet->limit.upper.y;
    }
    else if(bullet->position.y > bullet->limit.upper.y)
    {
        bullet->position.y = bullet->limit.lower.y;
    }
}

