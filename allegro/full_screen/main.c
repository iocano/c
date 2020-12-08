#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *events = NULL;

int width = 1366;
int height = 768;

int main()
{
    al_init();
    al_install_keyboard();
    al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_REQUIRE);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(width, height);
    al_init_primitives_addon();
	
    events = al_create_event_queue();
    al_register_event_source(events, al_get_keyboard_event_source());
	
    al_clear_to_color(al_map_rgb(0, 128, 128));

    while (al_is_event_queue_empty(events))
    {
        al_draw_filled_circle(rand() % width, rand() % height, rand() % 64, al_map_rgb(rand() % 256, 0, 0));
        al_flip_display();
    }

}
