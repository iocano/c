#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

void exit_if(bool condition, const char *format, ...);

int main(void)
{
    int width = 640, height = 480;
    bool done = false, draw = true;
    int pos_x = width / 2, pos_y = height / 2;

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(width, height)), "Can't create display\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives addon\n");
    exit_if(!al_install_mouse(), "Can't install mouse\n");

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_hide_mouse_cursor(display);
    while (!done)
    {
        ALLEGRO_EVENT ev, next_event;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            // button one toggle draw
            if (ev.mouse.button & 1)
            {
                draw = !draw;
            }
            // buttons two exit
            else if (ev.mouse.button & 2)
            {
                done = true;
            }
        }
        else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            pos_x = ev.mouse.x;
            pos_y = ev.mouse.y;
            // *Avoid overflowing event_queue.
            // Copy the contents of the next event in the event queue specified into ret_event and return true
            while (al_peek_next_event(event_queue, &next_event) && next_event.type == ALLEGRO_EVENT_MOUSE_AXES )
            {
                al_drop_next_event(event_queue);
            }
        }
        if (draw)
        {
            al_draw_filled_rectangle(pos_x, pos_y, pos_x + 30, pos_y + 30, al_map_rgb(255, 0, 255));
        }
        al_flip_display();
        al_clear_to_color(al_map_rgb(0, 0, 0));
    }
    al_destroy_display(display);
    return 0;
}

void exit_if(bool condition, const char *format, ...)
{
    if(condition)
    {
        va_list argument_list;
        va_start(argument_list, format);
        vfprintf(stderr, format, argument_list);
        va_end(argument_list);
        exit(EXIT_FAILURE);
    }
}
