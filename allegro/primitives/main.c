#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>

void exit_if(bool condition, const char *format, ...);

int main(void)
{
    int width = 640;
    int height = 480;

    ALLEGRO_DISPLAY *display = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(width, height)), "Can't create display\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives addon");

    al_draw_pixel(55, 55, al_map_rgb(255, 0, 255));

    al_draw_line(0, 300, width, 300, al_map_rgb(255, 0, 255), 10);

    al_draw_triangle(10, 200, 100, 10, 190, 200, al_map_rgb(255, 0, 255), 5);
    al_draw_filled_triangle(300, 400, 400, 200, 500, 400, al_map_rgb(0, 0, 255));

    al_draw_rectangle(10, 10, 250, 250, al_map_rgb(255, 0, 255), 5);
    al_draw_rounded_rectangle(width - 200, 10, width - 10, 50, 5, 5, al_map_rgb(0, 0, 255), 15);
    al_draw_filled_rectangle(10, 280, 250, height - 10, al_map_rgb(255, 255, 255));
    al_draw_filled_rounded_rectangle(width - 200, 180, width - 10, height - 10, 10, 10, al_map_rgb(0, 255, 0));

    al_draw_circle(100, 100, 50, al_map_rgb(255, 255, 0), 7);
    al_draw_filled_circle(400, 400, 70, al_map_rgb(255, 255, 255));

    al_draw_ellipse(150, 100, 100, 50, al_map_rgb(127, 3, 34), 7);
    al_draw_filled_ellipse(400, 250, 100, 200, al_map_rgb(0, 255, 255));

    float points[] = {0, 0, 400, 100, 50, 200, width, height};
    al_draw_spline(points, al_map_rgb(255, 0, 255), 0);

    float points2[] = {0, height, 200, 100, 400, 200, width, height};
    al_draw_spline(points2, al_map_rgb(0, 0, 255), 3);

    al_flip_display();

    al_rest(5.0);

    al_destroy_display(display); //destroy our display object

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
