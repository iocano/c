#include <allegro5/allegro_primitives.h>
#include "include/bullet.h"

void bullet_init(bullet_t bullets[], int bullet_count, object_limit_t movement_limit)
{
    for (int i = 0; i < bullet_count; i++)
    {
        bullet_t *bullet = &bullets[i];
        bullet->speed = 10;
        bullet->movement_limit = movement_limit;
        bullet->live = false;
    }
}

void bullet_draw(bullet_t bullets[], int bullet_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        bullet_t *bullet = &bullets[i];
        if (bullet->live)
        {
            al_draw_filled_circle(
                bullet->position.x,
                bullet->position.y,
                2,
                al_map_rgb(255, 255, 255));
        }
    }
}

void bullet_fire(bullet_t bullets[], int bullet_count, ship_t *ship)
{
    for (int i = 0; i < bullet_count; i++)
    {
        bullet_t *bullet = &bullets[i];
        if (!bullet->live)
        {
            bullet->position.x = ship->position.x + 17;
            bullet->position.y = ship->position.y;
            bullet->live = true;
            break;
        }
    }
}

void bullet_update(bullet_t bullets[], int bullet_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        bullet_t *bullet = &bullets[i];
        if (bullet->live)
        {
            bullet->position.x += bullet->speed;
            if (bullet->position.x > bullet->movement_limit.upper.x)
            {
                bullet->live = false;
            }
        }
    }
}

void bullet_collide(bullet_t bullets[], int bullet_count, comet_t comets[], int comet_count, ship_t *ship, explosion_t explosions[], int explosion_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        bullet_t *bullet = &bullets[i];
        if (bullet->live == false)
        {
            continue;
        }

        for (int j = 0; j < comet_count; j++)
        {
            comet_t *comet = &comets[j];
            if (comet->live == false)
            {
                continue;
            }
            bool bullet_inside_left_comet_bound = bullet->position.x > (comet->position.x - comet->bound.width);
            bool bullet_inside_right_comet_bound = bullet->position.x < (comet->position.x + comet->bound.width);
            bool bullet_inside_upper_comet_bound = bullet->position.y > (comet->position.y - comet->bound.height);
            bool bullet_inside_lower_comet_bound = bullet->position.y < (comet->position.y + comet->bound.height);
            if (bullet_inside_left_comet_bound && bullet_inside_right_comet_bound && bullet_inside_upper_comet_bound && bullet_inside_lower_comet_bound)
            {
                bullet->live = false;
                comet->live = false;
                ship->score++;
                explosion_start(explosions, explosion_count, bullet->position);
            }
        }
    }
}
