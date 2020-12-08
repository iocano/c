#ifndef OBJECT_H
#define OBJECT_H

#include <allegro5/allegro.h>
/*
   Model a point
*/
typedef struct object_point
{
    int x;
    int y;
}object_point_t;

/*
   Model a rectangular area
*/
typedef struct object_limit
{
    object_point_t lower;
    object_point_t upper;
}object_limit_t;

/*
   Model size width * height
*/
typedef struct object_size
{
    float width;
    float height;
}object_size_t;

typedef struct object_sprite
{
    int delay;
    int current;
    int width;
    int height;
}object_sprite_t;

typedef struct object_sprite_sheet
{
    int direction;
    int rows;
    int columns;
    object_sprite_t sprite;
    ALLEGRO_BITMAP *image;
}object_sprite_sheet_t;

#endif /* OBJECT_H */
