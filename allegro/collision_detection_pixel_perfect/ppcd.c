#include "ppcd.h"

mask_t *mask_create(ALLEGRO_BITMAP *image, int width, int height)
{
    mask_t *temp = malloc(sizeof(mask_t));
    temp->width = width;
    temp->height = height;
    temp->bits = malloc(sizeof(bool) * width * height);
    ALLEGRO_COLOR pixel;
    al_lock_bitmap(image, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READONLY);
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            pixel = al_get_pixel(image, col, row);
            temp->bits[row * width + col] = (pixel.a > 0) ? true : false;
        }
    }
    al_unlock_bitmap(image);
    return temp;
}

void mask_destroy(mask_t *mask)
{
    if (mask->bits != NULL)
    {
        free(mask->bits);
    }
    if (mask != NULL)
    {
        free(mask);
    }
}

bool mask_collide(const mask_t *mask_a, const mask_t *mask_b, int x_offset , int y_offset)
{
    int x_over = (mask_a->width + mask_b->width) / 2 - abs(x_offset);
    int y_over = (mask_a->height + mask_b->height) / 2 - abs(y_offset);
    if (x_over <= 0 || y_over <= 0)
    {
        return false;
    }

    // Object 1 is to the left of object 2
    int x1 = (mask_a->width - x_over) * ((x_offset < 0) ? 1 : 0);
    // Object 1 is on top of object 2
    int y1 = (mask_a->height - y_over) * ((y_offset < 0) ? 1 : 0);
    // Object 2 is to the right of object 1
    int x2 = (mask_b->width - x_over) * ((x_offset < 0) ? 0 : 1);
    // Object 2 is below object 1
    int y2 = (mask_b->height - y_over) * ((y_offset < 0) ? 0 : 1);

    for (int row = 0; row < y_over; row++)
    {
        for (int col = 0; col < x_over; col++)
        {
			int pixel_object_a = mask_a->bits[(row + y1) * mask_a->width + (col + x1)];
			int pixel_object_b = mask_b->bits[(row + y2) * mask_b->width + (col + x2)];
            if (pixel_object_a == true && pixel_object_b == true)
            {
                return true;
            }
        }
    }
    return false;
}

void mask_draw(mask_t *mask, int x, int y, ALLEGRO_DISPLAY *display)
{
    x = x - mask->width / 2;
    y = y - mask->height / 2;
    ALLEGRO_BITMAP *temp = al_get_backbuffer(display);
    al_lock_bitmap(temp, ALLEGRO_PIXEL_FORMAT_ANY, ALLEGRO_LOCK_READWRITE);
    for (int row = 0; row < mask->height; row++)
    {
        for (int col = 0; col < mask->width; col++)
        {
            if (mask->bits[row * mask->width + col] == true)
            {
                al_put_pixel(x + col, y + row, al_map_rgba_f(0, .3, .3, .1));
            }
        }
    }
    al_unlock_bitmap(temp);
}
