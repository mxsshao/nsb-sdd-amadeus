#include "Globals.h"
#include "HUD_PFD.h"
#include "SQ380.h"

const int WIDTH = 1000;
const int HEIGHT = 650;

HUD_PFD *pfd;
SQ380 *sq380;
ALLEGRO_DISPLAY *display = NULL;


int main(int argc, char **argv)
{
	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

	bool done = false;

	//Allegro
	
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer;
	ALLEGRO_FONT *font24;
	//Allegro Init
	if (!al_init())
		return -1;

	//al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR|ALLEGRO_MAG_LINEAR|ALLEGRO_MIPMAP);
	display = al_create_display(WIDTH, HEIGHT);

	if (!display)
		return -1;

	//Addons
	al_install_mouse();
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();

	al_reserve_samples(10);

	//PROJECT INIT
	font24 = al_load_font("LiquidCrystal-Normal.otf", 24, 0);
	if (!font24)
	{
		al_show_native_message_box(display, "Error", "", "Failed to load font", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	//pfd = new HUD_PFD();
	sq380 = new SQ380();

	//Timer
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);
	gameTime = al_current_time();

	while (!done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//Game Update
		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			//pfd->Update();
			sq380->Update();
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
		{
			if (ev.mouse.button == 1)
			{
				sq380->ButtonDown(ev.mouse.x, ev.mouse.y);
			}
		}

		//Render
		if (al_is_event_queue_empty(event_queue))
		{
			//pfd->Render();
			sq380->Render();

			frames ++;
				if(al_current_time() - gameTime >= 1)
				{
					gameTime = al_current_time();
					gameFPS = frames;
					frames = 0;
				}


			al_draw_textf(font24, al_map_rgb(255, 255, 0), 750, 5, 0, "FPS: %i DERP", gameFPS);
			al_draw_textf(font24, al_map_rgb(255, 255, 0), 750, 5, ALLEGRO_ALIGN_CENTER, "FPS: %i", gameFPS);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0,0,0));
		}
	}

		//Exit
		al_destroy_event_queue(event_queue);
		al_destroy_display(display);
		al_destroy_font(font24);



		return 0;
}