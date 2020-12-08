#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

void exit_if(bool condition, const char *format, ...);

int main()
{
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_FONT *font20;
    ALLEGRO_FONT *font24;
    ALLEGRO_FONT *font28;
    ALLEGRO_FONT *font32;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(640, 480)), "Can't create display\n");
    exit_if(!al_init_font_addon(), "Can't initialize font addon\n");
    exit_if(!al_init_ttf_addon(), "Can't initialize ttf addon");

    char *font_name = "DejaVuSans.ttf";
    exit_if(!(font20 = al_load_font(font_name, 20, 0)), "Can't load %s\n", font_name);
    exit_if(!(font24 = al_load_font(font_name, 24, 0)), "Can't load %s\n", font_name);
    exit_if(!(font28 = al_load_font(font_name, 28, 0)), "Can't load %s\n", font_name);
    exit_if(!(font32 = al_load_font(font_name, 32, 0)), "Can't load %s\n", font_name);

    al_clear_to_color(al_map_rgb(0, 0, 0));
    int screen_witdth = al_get_display_width(display);
    int screen_height = al_get_display_height(display);
    al_draw_text(font20, al_map_rgb(255, 0, 255), 50, 50, 0, "Hello world");
    al_draw_text(font24, al_map_rgb(255, 0, 255), screen_witdth / 2, screen_height / 2, ALLEGRO_ALIGN_CENTRE, "Hello world");
    al_draw_text(font28, al_map_rgb(255, 0, 255), 600, 350, ALLEGRO_ALIGN_RIGHT, "Hello world");
    al_draw_textf(font32, al_map_rgb(34, 78, 43), screen_witdth / 2, 400, ALLEGRO_ALIGN_CENTRE, " Display width: %i, height: %i", screen_witdth, screen_height);
    al_flip_display();

    // wait five seconds
    al_rest(5.0);

    al_destroy_font(font20);
    al_destroy_font(font24);
    al_destroy_font(font28);
    al_destroy_font(font32);
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
