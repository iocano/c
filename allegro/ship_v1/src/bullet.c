#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include "include/bullet.h"

bullet_t** bullet_array_init(int bullet_count, object_limit_t limit)
{
    bullet_t **bullets = malloc(sizeof(bullet_t*) * bullet_count);
    for (int i = 0; i < bullet_count; i++)
    {
        bullets[i] = malloc(sizeof(bullet_t));
        bullets[i]->speed = 10;
        bullets[i]->live = false;
        bullets[i]->limit = limit;
    }
    return bullets;
}

void bullet_array_draw(bullet_t **bullets, int bullet_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        if (bullets[i]->live)
        {
            al_draw_filled_circle(
                bullets[i]->position.x,
                bullets[i]->position.y,
                2,
                al_map_rgb(255, 255, 255));
        }
    }
}

void bullet_array_fire(bullet_t **bullets, int bullet_count, ship_t *ship)
{
    for (int i = 0; i < bullet_count; i++)
    {
        if (!bullets[i]->live)
        {
            bullets[i]->position.x = ship->position.x + 17;
            bullets[i]->position.y = ship->position.y;
            bullets[i]->live = true;
            break;
        }
    }
}

void bullet_array_update(bullet_t **bullets, int bullet_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        if (bullets[i]->live)
        {
            bullets[i]->position.x += bullets[i]->speed;
            if (bullets[i]->position.x > bullets[i]->limit.upper.x)
            {
                bullets[i]->live = false;
            }
        }
    }
}

void bullet_array_collide(bullet_t **bullets, int bullet_count, comet_t **comets, int comet_count, ship_t *ship)
{
    for (int i = 0; i < bullet_count; i++)
    {
        if (bullets[i]->live)
        {
            for (int j = 0; j < comet_count; j++)
            {
                if (comets[j]->live)
                {
                    if (bullets[i]->position.x > (comets[j]->position.x - comets[j]->bound.x)
                            && bullets[i]->position.x < (comets[j]->position.x + comets[j]->bound.x)
                            && bullets[i]->position.y > (comets[j]->position.y - comets[j]->bound.y)
                            && bullets[i]->position.y < (comets[j]->position.y + comets[j]->bound.y))
                    {
                        bullets[i]->live = false;
                        comets[j]->live = false;
                        ship->score++;
                    }
                }
            }
        }
    }
}

void bullet_array_destroy(bullet_t **bullets, int bullet_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        free(bullets[i]);
    }
    free(bullets);
}
