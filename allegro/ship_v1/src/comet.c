#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include "include/comet.h"

comet_t** comet_array_init(int comet_count, object_limit_t limit)
{
    comet_t **comets = malloc(sizeof(comet_t*) * comet_count);
    for (int i = 0; i < comet_count; i++)
    {
        comets[i] = malloc(sizeof(comet_t));
        comets[i]->live = false;
        comets[i]->speed = 5;
        comets[i]->bound = (object_coordinate_t){18, 18};
        comets[i]->limit = limit;
    }
    return comets;
}

void comet_array_draw(comet_t **comets, int comet_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        if (comets[i]->live)
        {
            al_draw_filled_circle(comets[i]->position.x, comets[i]->position.y, 20, al_map_rgb(0, 0, 0));
        }
    }
}

void comet_array_start(comet_t **comets, int comet_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        if (!comets[i]->live)
        {
            if (rand() % 500 == 0)
            {
                comets[i]->live = true;

                // comet start at limit.x
                comets[i]->position.x = comets[i]->limit.upper.x;

                // Betwen 30 and 340 at y
                comets[i]->position.y = 30 + rand() % (comets[i]->limit.upper.y - 60);
            }
        }
    }
}

void comet_array_update(comet_t **comets, int comet_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        if (comets[i]->live)
        {
            comets[i]->position.x -= comets[i]->speed;
            if(comets[i]->position.x + comets[i]->bound.x < 0)
            {
                comets[i]->live = false;
            }
        }
    }
}

void comet_array_collide(comet_t **comets, int comet_count, ship_t *ship)
{
    for (int i = 0; i < comet_count; i++)
    {
        if (comets[i]->live)
        {
            if (comets[i]->position.x - comets[i]->bound.x < ship->position.x + ship->bound.x
                    && comets[i]->position.x + comets[i]->bound.x > ship->position.x - ship->bound.x
                    && comets[i]->position.y - comets[i]->bound.y < ship->position.y + ship->bound.y
                    && comets[i]->position.y + comets[i]->bound.y > ship->position.y + ship->bound.y)
            {
                ship->lives--;
                comets[i]->live = false;
            }
            else if (comets[i]->position.x < 0)
            {
                ship->lives--;
                comets[i]->live = false;
            }
        }
    }
}

void commet_array_free(comet_t **comets, int comet_count)
{
    for (int i = 0; i < comet_count; ++i)
    {
        free(comets[i]);
    }
    free(comets);
}
