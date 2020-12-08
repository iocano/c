#ifndef OBJECT_H
#define OBJECT_H

typedef struct object_size
{
    float width;
    float height;
}object_size_t;

typedef struct object_point
{
    float x;
    float y;
}object_point_t;

typedef struct object_limit
{
    object_point_t lower;
    object_point_t upper;
}object_limit_t;

typedef struct object_speed
{
    float current;
    float maximum;
    float acceleration;
    float deceleration;
}object_speed_t;

typedef struct object_radius
{
    float minimum;
    float maximum;
}object_radius_t;

typedef struct object_direction
{
    float current;
    float increment;
}object_direction_t;

typedef struct object_timer
{
    float start;
    float limit;
} object_timer_t;

typedef struct object_color
{
    int red;
    int green;
    int blue;
}object_color_t;

typedef struct object_collision_result
{
    bool is_collided;
    object_point_t position;
}object_collision_result_t;

#endif /* OBJECT_H */
