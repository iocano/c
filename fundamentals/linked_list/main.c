/*
    Program:
        Test generic linked list basic operations with custom struct (bullet_t)
    Compile:
        gcc main.c linked_list/linked_list.c -o bullets
    -------------------------------------
    Run:
        ./bullets
*/

#include <stdio.h>
#include "linked_list/linked_list.h"

typedef struct bullet
{
    int position;
    float speed;
    float acceleration;
}bullet_t;

// initialize 'bullet_count' bullets
void bullet_manager_init(linked_list_t *list, int bullet_count);

// initialize and return a bullet
bullet_t* bullet_init(int position);

// search bullet by position and return node where it is contained
node_t *search_bullet_with_position(int position, linked_list_t *list);

// linked_list_foreach callback: display single bullet data
void bullet_display(void *data);

int main()
{
    // Initialize list
    linked_list_t *list = linked_list_create();

    // fill list with 3 elements (bullet_t)
    linked_list_add_first(bullet_init(0), sizeof(bullet_t), list);
    linked_list_add_at(1, bullet_init(1), sizeof(bullet_t), list);
    linked_list_add_last(bullet_init(2), sizeof(bullet_t), list);

    // getting data
    bullet_t *bullet_0 = linked_list_get_first(list);
    bullet_t *bullet_1 = linked_list_get_at(1, list);
    bullet_t *bullet_2 = linked_list_get_last(list);

    // show individual bullet info
    bullet_display(bullet_0);
    bullet_display(bullet_1);
    bullet_display(bullet_2);

    // Remove/pop from linked list
    bullet_t pop_bullet;
    /* Pop fist list element */
    if (linked_list_remove_first(list, &pop_bullet) == true) { /*use pop_bullet*/ }
    else { /*print linked_list_status_string(list->last_status) */ }

    /* Remove fist list element */
    if (linked_list_remove_first(list, NULL) == true) { /*first list element deleted*/ }
    else { /*print linked_list_status_string(list->last_status) */ }

    /* Pop last list element */
    if (linked_list_remove_last(list, &pop_bullet) == true) { /*use pop_bullet*/ }
    else { /*print linked_list_status_string(list->last_status) */ }

    /* Remove last list element */
    if (linked_list_remove_last(list, NULL) == true) { /*last list element deleted*/ }
    else { /*print linked_list_status_string(list->last_status) */ }

    /* Pop nth list element */
    if (linked_list_remove_at(2, list, &pop_bullet) == true) { /*use pop_bullet*/ }
    else { /*print linked_list_status_string(list->last_status) */ }

    /* Remove nth list element */
    if (linked_list_remove_at(2, list, NULL) == true) { /*last list element deleted*/ }
    else { /*print linked_list_status_string(list->last_status) */ }

    // remove node
    linked_list_remove_node(list->head, list, NULL);

    // show list data
    linked_list_foreach(bullet_display, list);

    // show list data on reverse order
    linked_list_foreach_reverse(bullet_display, list);


    // free resources
    linked_list_destroy(list);
}

bullet_t* bullet_init(int position)
{
    bullet_t *bullet = malloc(sizeof(bullet_t));
    bullet->position = position;
    bullet->speed = 5;
    bullet->acceleration = .5;
    return bullet;
}

node_t *search_bullet_with_position(int position, linked_list_t *list)
{
    for(node_t *current = list->head; current != NULL; current = current->next)
    {
        bullet_t *bullet = current->data;
        if(bullet->position == position)
        {
            return current;
        }
    }
    return NULL;
}

void bullet_display(void *data)
{
    bullet_t *bullet = (bullet_t*) data;
    printf("Postion: %i ,Speed: %f, Acceleration: %f\n",bullet->position, bullet->speed, bullet->acceleration);
}
