#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <math.h>

typedef struct point
{
    float x;
    float y;
}point_t;

typedef struct animation
{
    point_t position;
    point_t speed;
    point_t sprite_size;
    point_t movement_limit;
    int sprite_count;
    int sprite_current;
    int sprite_direction;
    point_t sprite_sheet_length;
    ALLEGRO_BITMAP *sprite_sheet;
} animation_t;

void animation_init(animation_t *animation, ALLEGRO_BITMAP *sprite_sheet, point_t sprite_sheet_length, point_t sprite_size, point_t movement_limit);

void animation_update(animation_t *animation, int timerCount);

void animation_draw(animation_t *animation);

double angle_to_target(double x1, double y1, double x2, double y2);

void attract_particles(animation_t animations[], int  animation_count, float gravity);

void exit_if(bool condition, const char *format, ...);

int main()
{
    int orb_count = 100;
    point_t window_size = { 800, 600 };
	bool done = false, render = false;

	animation_t orbs[orb_count];

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_BITMAP *image;

    exit_if(!al_init(), "Can't initialize allegro\n");
	exit_if(!(display = al_create_display(window_size.x, window_size.y)), "Can't create display\n");
    exit_if(!al_init_image_addon(), "Can't initialize image addon\n");
	exit_if(!al_install_keyboard(), "Can't install keyboard\n");
	exit_if(!(image = al_load_bitmap("orb.png")), "Can't load orb.png\n");

	point_t sprite_sheet_length = { 5, 4 };
	point_t sprite_size =
	{
		al_get_bitmap_width(image) / sprite_sheet_length.x,
		al_get_bitmap_height(image) / sprite_sheet_length.y,
	};

	srand(time(NULL));

	for (int i = 0; i < orb_count; i++)
	{
		animation_init(&orbs[i], image, sprite_sheet_length, sprite_size, window_size);
	}

	timer = al_create_timer(1.0 / 60);
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			attract_particles(orbs, orb_count, 2);
			for (int i = 0; i < orb_count; i++)
			{
				animation_update(&orbs[i], al_get_timer_count(timer));
			}
			render = true;
		}

		if (render && al_is_event_queue_empty(event_queue))
		{
			render = false;

			for (int i = 0; i < orb_count; i++)
			{
				animation_draw(&orbs[i]);
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}

	al_destroy_bitmap(image);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_display(display); //destroy our display object

	return 0;
}

void animation_init(animation_t *animation, ALLEGRO_BITMAP *sprite_sheet, point_t sprite_sheet_length, point_t sprite_size, point_t movement_limit)
{
    animation->position = (point_t){rand() % 200 + 100, rand() % 200 + 100};
    animation->speed = (point_t){rand() % 5 + 2, rand() % 5 + 2};
    animation->sprite_size = sprite_size;
    animation->movement_limit = movement_limit;
    animation->sprite_count = sprite_sheet_length.x * sprite_sheet_length.y;
    animation->sprite_current = 0;
    animation->sprite_direction = 1;
    animation->sprite_sheet_length = sprite_sheet_length;
    animation->sprite_sheet = sprite_sheet;
}

void animation_update(animation_t *animation, int timerCount)
{
	if (timerCount % 5 == 0)
	{
        // next/previous sprite based on sprite_direction
		animation->sprite_current += animation->sprite_direction;
        if (animation->sprite_current >= animation->sprite_count)
		{
			animation->sprite_current = 0;
		}
        else if (animation->sprite_current <= 0)
		{
            animation->sprite_current = animation->sprite_count - 1;
		}
	}

    animation->position.x += animation->speed.x;
	if ((animation->position.x <= 0) || (animation->position.x >= animation->movement_limit.x - animation->sprite_size.x))
	{
        animation->speed.x *= -1;
        animation->sprite_direction *= -1;
	}

    animation->position.y += animation->speed.y;
	if ((animation->position.y <= 0) || (animation->position.y >= animation->movement_limit.y - animation->sprite_size.y))
	{
      animation->speed.y *= -1;
      animation->sprite_direction *= -1;
	}
}

void animation_draw(animation_t *animation)
{
	int fx = (animation->sprite_current % (int)animation->sprite_sheet_length.x) * animation->sprite_size.x;
    int fy = (animation->sprite_current / (int)animation->sprite_sheet_length.y) * animation->sprite_size.y;
	fx = 3 * animation->sprite_size.x;
	fy = 2 * animation->sprite_size.y;

	al_draw_bitmap_region(
		animation->sprite_sheet,		// image
		fx,					            // image offset at x
		fy,					            // image offset at y
		animation->sprite_size.x,	    // frame width
		animation->sprite_size.y,       // frame height
		animation->position.x,			// display offset at x
		animation->position.y,			// display offset at y
		0);
}

double angle_to_target(double x1, double y1, double x2, double y2)
{
	double deltaX = (x2 - x1);
	double deltaY = (y2 - y1);
	return atan2(deltaY, deltaX);
}

void attract_particles(animation_t animations[], int  animation_count, float gravity)
{
	for (int i = 0; i < animation_count; i++)
	{
		float displacement_x = 0;
		float displacement_y = 0;
		for (int j = 0; j < animation_count; j++)
		{
			if (i != j)
			{
				float angle = angle_to_target(animations[i].position.x, animations[i].position.y, animations[j].position.x, animations[j].position.y);
				displacement_x += (gravity * cos(angle)) / 1000;
				displacement_y += (gravity * sin(angle)) / 1000;
			}
		}
		animations[i].speed.x += displacement_x;
		animations[i].speed.y += displacement_y;
	}
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
