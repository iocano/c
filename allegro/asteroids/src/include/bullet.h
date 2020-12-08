#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include "object.h"
#include "../linked_list/linked_list.h"

typedef struct bullet
{
    int score;
    bool is_alive;
    object_timer_t timer;
    object_direction_t direction;
    object_speed_t speed;
    object_limit_t limit;
    object_point_t position;
    object_point_t displacement;
    ALLEGRO_SAMPLE *audio;
    ALLEGRO_COLOR color;
}bullet_t;

linked_list_t* bullet_init(int bullet_count, object_limit_t limit);

void bullet_reset(linked_list_t *bullet_list);

void bullet_fire(linked_list_t *bullet_list, object_point_t position, object_direction_t direction);

void bullet_update(linked_list_t *bullet_list);

void bullet_draw(linked_list_t *bullet_list);

void bullet_destroy(linked_list_t *bullet_list);
