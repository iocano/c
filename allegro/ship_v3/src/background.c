#include "include/background.h"

void background_init(background_t *background, int x, int y, int speed_x, int speed_y, int width, int height, int direction_x, int direction_y, object_limit_t limit, ALLEGRO_BITMAP *image)
{
    background->position.x = x;
    background->position.y = y;
    background->speed.x = speed_x;
    background->speed.y = speed_y;
    background->size.x = width;
    background->size.y = height;
    background->direction.x = direction_x;
    background->direction.y = direction_y;
    background->limit = limit;
    background->image = image;
}

void background_update(background_t *background)
{
    background->position.x += background->speed.x * background->direction.x;
    if(background->position.x + background->size.x < background->limit.lower.x)
    {
        background->position.x = 0;
    }
}

void background_draw(background_t *background)
{
    al_draw_bitmap(background->image, background->position.x, background->position.y, 0);
    if(background->position.x + background->size.x < background->limit.upper.x)
    {
        al_draw_bitmap(background->image, background->position.x + background->size.x, background->position.y, 0);
    }
}
