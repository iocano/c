#ifndef OBJECT_H
#define OBJECT_H

/*
   Model a point
*/
typedef struct object_coordinate
{
    int x;
    int y;
}object_coordinate_t;

/*
   Model a rectangular area
*/
typedef struct object_limit
{
    object_coordinate_t lower;
    object_coordinate_t upper;
}object_limit_t;

#endif /* OBJECT_H */
