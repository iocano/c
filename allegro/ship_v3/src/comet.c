#include "include/comet.h"

void comet_init(comet_t comets[], int comet_count, ALLEGRO_BITMAP *image, object_limit_t limit)
{
    for (int i = 0; i < comet_count; i++)
    {
        comets[i].live = false;
        comets[i].speed = 5;
        comets[i].bound.x = 20;
        comets[i].bound.y = 24;
        comets[i].limit = limit;

        comets[i].sheet.rows = 6;
        comets[i].sheet.columns = 5;
        comets[i].sheet.sprite.current = 0;
        comets[i].sheet.sprite.delay = 2;
        comets[i].sheet.sprite.width = 64;
        comets[i].sheet.sprite.height = 64;
        comets[i].sheet.direction = (rand() % 2) ? 1 : -1;
        comets[i].sheet.image = image;
    }
}

void comet_draw(comet_t comets[], int comet_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        if (comets[i].live)
        {
            int fx = (comets[i].sheet.sprite.current % comets[i].sheet.columns) * comets[i].sheet.sprite.width;
            int fy = (comets[i].sheet.sprite.current / comets[i].sheet.columns) * comets[i].sheet.sprite.height;

            al_draw_bitmap_region(
                    comets[i].sheet.image,
                    fx,
                    fy,
                    comets[i].sheet.sprite.width,
                    comets[i].sheet.sprite.height,
                    comets[i].position.x - comets[i].sheet.sprite.width / 2,
                    comets[i].position.y - comets[i].sheet.sprite.height / 2,
                    0);

            // al_draw_filled_rectangle(
            //     comets[i].x - comets[i].boundx,
            //     comets[i].y - comets[i].boundy,
            //     comets[i].x + comets[i].boundx,
            //     comets[i].y + comets[i].boundy,
            //     al_map_rgba(255, 0, 255, 100));
        }
    }
}

void comet_start(comet_t comets[], int comet_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        if (!comets[i].live)
        {
            if (rand() % 500 == 0)
            {
                comets[i].live = true;
                // Start commet at 800 on x axis
                comets[i].position.x = comets[i].limit.upper.x;

                // Betwen 30 and 340 at y
                comets[i].position.y = 30 + rand() % (comets[i].limit.upper.y - 60);
            }
        }
    }
}

void comet_update(comet_t comets[], int comet_count, int timerCount)
{
    for (int i = 0; i < comet_count; i++)
    {
        if (comets[i].live)
        {
            if (timerCount % comets[i].sheet.sprite.delay == 0)
            {
                comets[i].sheet.sprite.current += comets[i].sheet.direction;

                if (comets[i].sheet.sprite.current > comets[i].sheet.rows * comets[i].sheet.columns - 1)
                {
                    comets[i].sheet.sprite.current = 0;
                }
                else if (comets[i].sheet.sprite.current < 0)
                {
                    comets[i].sheet.sprite.current = comets[i].sheet.rows * comets[i].sheet.columns - 1;
                }
            }

            comets[i].position.x -= comets[i].speed;
        }
    }
}

void comet_collide(comet_t comets[], int comet_count, ship_t *ship, explosion_t explosions[], int explosion_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        if (comets[i].live)
        {
            bool comet_inside_right_ship_bound = comets[i].position.x - comets[i].bound.x < ship->position.x + ship->bound.x;
            bool comet_inside_left_ship_bound = comets[i].position.x + comets[i].bound.x > ship->position.x - ship->bound.x;
            bool comet_inside_upper_ship_bound = comets[i].position.y - comets[i].bound.y < ship->position.y + ship->bound.y;
            bool comet_inside_lower_ship_bound = comets[i].position.y + comets[i].bound.y > ship->position.y - ship->bound.y;
            if (comet_inside_left_ship_bound && comet_inside_right_ship_bound && comet_inside_upper_ship_bound && comet_inside_lower_ship_bound)
            {
                ship->lives--;
                comets[i].live = false;
                explosion_start(explosions, explosion_count, ship->position);
            }
            else if (comets[i].position.x < 0)
            {
                ship->lives--;
                comets[i].live = false;
            }
        }
    }
}

void comet_reset(comet_t comets[], int comet_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        comets[i].live = false;

    }
}
