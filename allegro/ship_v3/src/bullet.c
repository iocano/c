
#include "include/bullet.h"

void bullet_init(bullet_t bullets[], int bullet_count, object_limit_t limit, ALLEGRO_SAMPLE *sample)
{
    for (int i = 0; i < bullet_count; i++)
    {
        bullets[i].speed = 10;
        bullets[i].live = false;
        bullets[i].limit = limit;
        bullets[i].sample = sample;
    }
}

void bullet_draw(bullet_t bullets[], int bullet_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        if (bullets[i].live)
        {
            al_draw_filled_circle( bullets[i].position.x, bullets[i].position.y, 2, al_map_rgb(255, 255, 255));
        }
    }
}

void bullet_fire(bullet_t bullets[], int bullet_count, ship_t *ship)
{
    for (int i = 0; i < bullet_count; i++)
    {
        if (!bullets[i].live)
        {
            bullets[i].position.x = ship->position.x + 17;
            bullets[i].position.y = ship->position.y;
            bullets[i].live = true;
            al_play_sample(bullets[i].sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
            break;
        }
    }
}

void bullet_update(bullet_t bullets[], int bullet_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        if (bullets[i].live)
        {
            bullets[i].position.x += bullets[i].speed;
            if (bullets[i].position.x > bullets[i].limit.upper.x)
            {
                bullets[i].live = false;
            }
        }
    }
}

void bullet_collide(bullet_t bullets[], int bullet_count, comet_t comets[], int comet_count, ship_t *ship, explosion_t explosions[], int explosion_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        if (bullets[i].live)
        {
            for (int j = 0; j < comet_count; j++)
            {
                if (comets[j].live)
                {
                    bool bullet_inside_comet_left_bound = bullets[i].position.x > (comets[j].position.x - comets[j].bound.x);
                    bool bullet_inside_comet_right_bound = bullets[i].position.x < (comets[j].position.x + comets[j].bound.x);
                    bool bullet_inside_comet_lower_bound = bullets[i].position.y > (comets[j].position.y - comets[j].bound.y);
                    bool bullet_inside_comet_upper_bound = bullets[i].position.y < (comets[j].position.y + comets[j].bound.y);
                    if (bullet_inside_comet_left_bound && bullet_inside_comet_right_bound && bullet_inside_comet_lower_bound && bullet_inside_comet_upper_bound)
                    {
                        bullets[i].live = false;
                        comets[j].live = false;
                        ship->score++;
                        explosion_start(explosions, explosion_count, bullets[i].position);
                    }
                }
            }
        }
    }
}

void bullet_reset(bullet_t bullets[], int bullet_count)
{
    for (int i = 0; i < bullet_count; i++)
    {
        bullets[i].live = false;
    }
}
