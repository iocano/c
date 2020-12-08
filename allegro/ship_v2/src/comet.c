#include "include/comet.h"

void comet_init(comet_t comets[], int comet_count, ALLEGRO_BITMAP *sprite_sheet, object_limit_t movement_limit)
{
    for (int i = 0; i < comet_count; i++)
    {
        comet_t *comet = &comets[i];
        comet->live = false;
        comet->speed = 5;
        comet->bound.width = 20;
        comet->bound.height = 24;
        comet->movement_limit = movement_limit;

        comet->sheet.sprite.current = 0;
        comet->sheet.sprite.delay = 2;
        comet->sheet.sprite.width = 64;
        comet->sheet.sprite.height = 64;
        comet->sheet.direction = (rand() % 2) ? 1 : -1;
        comet->sheet.columns = 5;
        comet->sheet.rows = 6;
        comet->sheet.image = sprite_sheet;
    }
}

void comet_draw(comet_t comets[], int comet_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        comet_t *comet = &comets[i];
        if(comet->live)
        {
            int bitmap_region_x = (comet->sheet.sprite.current % comet->sheet.columns) * comet->sheet.sprite.width;
            int bitmap_region_y = (comet->sheet.sprite.current / comet->sheet.columns) * comet->sheet.sprite.height;

            al_draw_bitmap_region(
                    comet->sheet.image,
                    bitmap_region_x,
                    bitmap_region_y,
                    comet->sheet.sprite.width,
                    comet->sheet.sprite.height,
                    comet->position.x - comet->sheet.sprite.width / 2,
                    comet->position.y - comet->sheet.sprite.height / 2,
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
        comet_t *comet = &comets[i];
        if (!comet->live && rand() % 500 == 0)
        {
            comet->live = true;
            // Start comet at 800 on x axis
            comet->position.x = comet->movement_limit.upper.x;
            // Betwen 30 and 340 at y axis
            comet->position.y = 30 + rand() % (comet->movement_limit.upper.y - comet->sheet.sprite.width);
        }
    }
}

void comet_update(comet_t comets[], int comet_count, int timer_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        comet_t *comet = &comets[i];

        if(comet->live == false)
        {
            continue;
        }

        if (timer_count % comet->sheet.sprite.delay == 0)
        {
            comet->sheet.sprite.current += comet->sheet.direction;
            if (comet->sheet.sprite.current > comet->sheet.columns * comet->sheet.rows - 1)
            {
                comet->sheet.sprite.current = 0;
            }
            else if (comet->sheet.sprite.current < 0)
            {
                comet->sheet.sprite.current = comet->sheet.columns * comet->sheet.rows - 1;
            }
        }
        comet->position.x -=  comet->speed;
    }
}

void comet_collide(comet_t comets[], int comet_count, ship_t *ship, explosion_t explosions[], int explosion_count)
{
    for (int i = 0; i < comet_count; i++)
    {
        comet_t *comet = &comets[i];

        if (comet->live == false)
        {
            continue;
        }

        bool comet_inside_left_ship_bound = comet->position.x + comet->bound.width > ship->position.x - ship->bound.width;
        bool comet_inside_right_ship_bound = comet->position.x - comet->bound.width < ship->position.x + ship->bound.width;
        bool comet_inside_upper_ship_bound = comet->position.y - comet->bound.height < ship->position.y + ship->bound.height;
        bool comet_inside_lower_ship_bound = comet->position.y + comet->bound.height > ship->position.y - ship->bound.height;
        if (comet_inside_left_ship_bound && comet_inside_right_ship_bound && comet_inside_upper_ship_bound && comet_inside_lower_ship_bound)
        {
            ship->lives--;
            comet->live = false;
            explosion_start(explosions, explosion_count, ship->position);
        }
        else if (comet->position.x < comet->movement_limit.lower.x)
        {
            ship->lives--;
            comet->live = false;
        }
    }
}
