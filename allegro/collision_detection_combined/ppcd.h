#include <allegro5/allegro5.h>
#include <allegro5/bitmap.h>

typedef struct mask
{
    int width;
    int height;
    bool *bits;
} mask_t;

// Create a new mask
mask_t *mask_create(ALLEGRO_BITMAP *image, int w, int h);

// Free mask memory
void mask_destroy(mask_t *m);

// Test collision between ma and mb
// Return true if collision has occurred
// xoffset: ma.x - mb.x
// yoffset: ma.y - mb.y
bool mask_collide(const mask_t *ma, const mask_t *mb, int x_offset, int y_offset);

// Draw the mask on the screen
void mask_draw(mask_t *m, int x, int y, ALLEGRO_DISPLAY *display);
