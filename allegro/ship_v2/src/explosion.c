#include "include/explosion.h"

void explosion_init(explosion_t explosions[], int explosion_count, ALLEGRO_BITMAP *sprite_sheet)
{
    for (int i = 0; i < explosion_count; i++)
    {
        explosion_t *explosion = &explosions[i];

        explosion->live = false;
        explosion->sheet.sprite.current = 0;
        explosion->sheet.sprite.delay = 1;
        explosion->sheet.sprite.width = 80;
        explosion->sheet.sprite.height = 80;
        explosion->sheet.columns = 8;
        explosion->sheet.rows = 6;
        explosion->sheet.direction = 1;
        explosion->sheet.image = sprite_sheet;
    }
}

void explosion_draw(explosion_t explosions[], int explosion_count)
{
    for (int i = 0; i < explosion_count; i++)
    {
        explosion_t *explosion = &explosions[i];
        if (explosion->live)
        {
            int bitmap_region_x = (explosion->sheet.sprite.current % explosion->sheet.columns) * explosion->sheet.sprite.width;
            int bitmap_region_y = (explosion->sheet.sprite.current / explosion->sheet.columns) * explosion->sheet.sprite.height;

            al_draw_bitmap_region(
                explosion->sheet.image,
                bitmap_region_x,
                bitmap_region_y,
                explosion->sheet.sprite.width,
                explosion->sheet.sprite.height,
                explosion->position.x - explosion->sheet.sprite.width / 2,
                explosion->position.y - explosion->sheet.sprite.height / 2,
                0);
        }
    }
}

void explosion_start(explosion_t explosions[], int explosion_count, object_point_t position)
{
    for (int i = 0; i < explosion_count; i++)
    {
        explosion_t *explosion = &explosions[i];
        if (!explosion->live)
        {
            explosion->live = true;
            explosion->position = position;
            break;
        }
    }
}

void explosion_update(explosion_t explosions[], int explosion_count, int timer_count)
{
    for (int i = 0; i < explosion_count; i++)
    {
        explosion_t *explosion = &explosions[i];

        if (explosion->live && timer_count % explosion->sheet.sprite.delay == 0)
        {
            explosion->sheet.sprite.current += explosion->sheet.direction;
            if(explosion->sheet.sprite.current >= explosion->sheet.rows * explosion->sheet.columns)
            {
                explosion->sheet.sprite.current = 0;
                explosion->live = false;
            }
        }
    }
}
