#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <stdio.h>

typedef struct point
{
  int x,y;
}point_t;

void exit_if(bool condition, const char *format, ...);

int main()
{
    int width = 640, height = 480;
    bool done = false, redraw = false;
    point_t position = {width / 2 , height / 2};
    bool keys[ALLEGRO_KEY_MAX] = {false};

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_FONT *font18 = NULL;
    ALLEGRO_VOICE *voice = NULL;
    ALLEGRO_MIXER *mixer = NULL;
    ALLEGRO_SAMPLE *sample1, *sample2, *sample3;
    ALLEGRO_SAMPLE_INSTANCE *instance1, *instance2, *instance3;

    exit_if(!al_init(), "Can't initialize allegro\n");
    exit_if(!(display = al_create_display(width, height)), "Can't create display\n");
    exit_if(!al_install_keyboard(), "Can't install keyboard\n");
    exit_if(!al_init_primitives_addon(), "Can't initialize primitives addon\n");
    exit_if(!al_init_font_addon(), "Can't initialize font addon\n");
    exit_if(!al_init_ttf_addon(), "Can't initialize ttf addon\n");

    exit_if(!al_install_audio(), "Can't install audio\n");
    exit_if(!al_init_acodec_addon(), "Can't initialize acoded addon\n");

    voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
    al_attach_mixer_to_voice(mixer, voice);
    al_set_default_mixer(mixer);

    exit_if(!(font18 = al_load_font("resources/arial.ttf", 18, 0)), "Can't load arial.ttf\n");
    exit_if(!(sample1 = al_load_sample("resources/step.ogg")), "Can't load step.ogg\n");
    exit_if(!(sample2 = al_load_sample("resources/warning.ogg")), "Can't load warning.ogg\n");
    exit_if(!(sample3 = al_load_sample("resources/background.ogg")), "Can't load background.ogg\n");

    instance1 = al_create_sample_instance(sample1);
    instance2 = al_create_sample_instance(sample2);
    instance3 = al_create_sample_instance(sample3);

    al_attach_sample_instance_to_mixer(instance1, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance2, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance3, al_get_default_mixer());

    al_set_sample_instance_length(instance2, al_get_sample_instance_length(instance2) / 4);
    al_set_sample_instance_playmode(instance3, ALLEGRO_PLAYMODE_LOOP);

    timer = al_create_timer(1.0 / 60);
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_play_sample_instance(instance3);

    al_start_timer(timer);
    while (!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            keys[ev.keyboard.keycode] = true;
        }
        if (ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            keys[ev.keyboard.keycode] = false;
        }
        else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER)
        {
            redraw = true;
            point_t previous_position = position;

            position.y -= 5 * keys[ALLEGRO_KEY_UP];
            position.y += 5 * keys[ALLEGRO_KEY_DOWN];
            position.x -= 5 * keys[ALLEGRO_KEY_LEFT];
            position.x += 5 * keys[ALLEGRO_KEY_RIGHT];

            if (previous_position.x != position.x || previous_position.y != position.y)
            {
                al_play_sample_instance(instance1);
            }
            if (position.x - 10 < 0 || position.x + 10 > width || position.y - 10 < 0 || position.y + 10 > height)
            {
                al_play_sample_instance(instance2);
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue))
        {
            redraw = false;
            al_draw_filled_rectangle(position.x - 10, position.y - 10, position.x + 10, position.y + 10, al_map_rgb(255, 0, 255));
            if (al_get_sample_instance_playing(instance1))
            {
                al_draw_text(font18, al_map_rgb(255, 255, 255), 5, 5, 0, "Instance 1 is playing");
            }
            if (al_get_sample_instance_playing(instance2))
            {
                al_draw_text(font18, al_map_rgb(255, 255, 255), width - 5, 5, ALLEGRO_ALIGN_RIGHT, "Instance 2 is playing");
            }
            if (al_get_sample_instance_playing(instance3))
            {
                float currentPosition = al_get_sample_instance_position(instance3);
                float sampleLength = (float)al_get_sample_instance_length(instance3);
                float playingPercentage = currentPosition / sampleLength * 100;
                al_draw_textf( font18, al_map_rgb(255, 255, 255), 5, height - 20, 0, "Instance 3 is playing: %.1f ", playingPercentage);
            }
            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));
        }
    }
    al_destroy_sample_instance(instance1);
    al_destroy_sample_instance(instance2);
    al_destroy_sample_instance(instance3);
    al_destroy_sample(sample1);
    al_destroy_sample(sample2);
    al_destroy_sample(sample3);
    al_destroy_font(font18);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    al_destroy_display(display);
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
