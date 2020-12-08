#include "include/explosion.h"

void explosion_init(explosion_t explosions[], int explosion_count, ALLEGRO_BITMAP *image, ALLEGRO_SAMPLE *sample)
{
    for (int i = 0; i < explosion_count; i++)
    {
        explosions[i].live = false;
        explosions[i].sheet.sprite.current = 0;
        explosions[i].sheet.sprite.delay = 1;
        explosions[i].sheet.sprite.width = 80;
        explosions[i].sheet.sprite.height = 80;
        explosions[i].sheet.columns = 8;
        explosions[i].sheet.rows = 6;
        explosions[i].sheet.direction = 1;
        explosions[i].sheet.image = image;
        explosions[i].sample = sample;
    }
}

void explosion_draw(explosion_t explosions[], int explosion_count)
{
    for (int i = 0; i < explosion_count; i++)
    {
        if (explosions[i].live)
        {
            int fx = (explosions[i].sheet.sprite.current % explosions[i].sheet.columns) * explosions[i].sheet.sprite.width;
            int fy = (explosions[i].sheet.sprite.current / explosions[i].sheet.columns) * explosions[i].sheet.sprite.height;
            al_draw_bitmap_region(
                    explosions[i].sheet.image,
                    fx,
                    fy,
                    explosions[i].sheet.sprite.width,
                    explosions[i].sheet.sprite.height,
                    explosions[i].position.x - explosions[i].sheet.sprite.width / 2,
                    explosions[i].position.y - explosions[i].sheet.sprite.height / 2,
                    0);
        }
    }
}

void explosion_start(explosion_t explosions[], int explosion_count, object_point_t position)
{
    for (int i = 0; i < explosion_count; i++)
    {
        if (!explosions[i].live)
        {
            explosions[i].live = true;
            explosions[i].position = position;
            al_play_sample(explosions[i].sample, 1, 0, 1, ALLEGRO_PLAYMODE_ONCE, 0);
            break;
        }
    }
}

void explosion_update(explosion_t explosions[], int explosion_count, int timerCount)
{
    for (int i = 0; i < explosion_count; i++)
    {
        if (explosions[i].live)
        {
            if (timerCount % explosions[i].sheet.sprite.delay == 0)
            {
                explosions[i].sheet.sprite.current += explosions[i].sheet.direction;
                if (explosions[i].sheet.sprite.current >= explosions[i].sheet.rows * explosions[i].sheet.columns)
                {
                    explosions[i].sheet.sprite.current = 0;
                    explosions[i].live = false;
                }
            }
        }
    }
}

void explosion_reset(explosion_t explosions[], int explosion_count)
{
    for (int i = 0; i < explosion_count; i++)
    {
        explosions[i].live = false;
    }
}
