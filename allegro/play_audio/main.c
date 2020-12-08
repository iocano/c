#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>

void exit_if(bool condition, const char *format, ...);

int main()
{
    ALLEGRO_SAMPLE *sample = NULL;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!al_install_audio(), "Can't install audio\n");
    exit_if(!al_init_acodec_addon(), "Can't initialize acoded addon\n");
    exit_if(!al_reserve_samples(1), "Can't reserve sample instances\n");
    exit_if(!(sample = al_load_sample("air.ogg")), "Can't load air.ogg\n");
    al_play_sample(sample, 1, 0, 1, ALLEGRO_PLAYMODE_LOOP, NULL);
    al_rest(10);
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
