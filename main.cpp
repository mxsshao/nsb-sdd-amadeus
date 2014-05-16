#include "global.h"
#include "ctrldisplay.h"
#include "ATX.h"
#include "scenario_menu.h"
#include "states_manager.h"

int main(int argc, char **argv)
{
	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

	std::cout << "/-----------------------------------------------------------/" << std::endl;
	std::cout << "	Amadeus Testing Platform" << std::endl;
	std::cout << "		v1.1.0" << std::endl;
	std::cout << "/-----------------------------------------------------------/" << std::endl;
	std::cout << std::endl;

	//ALLEGRO INIT
	if (!al_init())
	{
		al_show_native_message_box(NULL, "ERROR", "A01:", "Error loading Allegro.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}
	std::cout << "Allegro Init" << std::endl;

	//ALLEGRO ADDONS
	al_install_keyboard();
	al_install_mouse();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();

	//ALLEGRO VAR
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer;
	ALLEGRO_DISPLAY* display;
	ALLEGRO_FONT* font;

	//Display
	al_set_new_display_flags(ALLEGRO_RESIZABLE);
	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	//al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	//al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);

	display = al_create_display(1280, 720);
	if (!display)
	{
		al_show_native_message_box(NULL, "ERROR", "A02:", "Error creating display.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return -1;
	}

	al_toggle_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, false);

	std::cout << "Display created " << al_get_display_width(display) << "*" << al_get_display_height(display) << std::endl;
	
	//IMAGE SETTINGS
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR | ALLEGRO_MIPMAP);

	//RESOURCES
	font = al_load_font("Resources/DolceVita.ttf", 24, 0);

	//LOADING SCREEN
	al_draw_text(font, al_map_rgb(255, 0, 255), 5, 5, 0, "LOADING");
	al_flip_display();
	al_clear_to_color(al_map_rgb(0, 0, 0));

	//GWEN
	Gwen::Renderer::Allegro* renderer = new Gwen::Renderer::Allegro();
	Gwen::Skin::TexturedBase defaultskin(renderer);
	defaultskin.SetRender(renderer);
	defaultskin.Init("Resources/defaultskin.png");
	defaultskin.SetDefaultFont(L"Resources/OpenSans.ttf", 14);
	Gwen::Controls::Canvas* canvas = new Gwen::Controls::Canvas(&defaultskin);
	canvas->SetSize(al_get_display_width(display), al_get_display_height(display));
	canvas->SetDrawBackground(false);

	Gwen::Controls::Base* base = new Gwen::Controls::Base(canvas);
	base->SetPos(0,0);
	base->SetSize(al_get_display_width(display), al_get_display_height(display));
	base->SetCursor(Gwen::CursorType::Normal);

	Gwen::Input::Allegro GwenInput;
	GwenInput.Initialize(canvas);

	//al_grab_mouse(display);

	std::cout << "GWEN Init successful" << std::endl;

	//EVENT INIT
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	States::Manager::done = false;

	//EVENT SOURCE
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_mouse_event_source());

	al_start_timer(timer);

	//INIT
	States::Manager::getInstance()->initialize(display, event_queue, base);
	//States::Manager::getInstance()->changeState(ATX::Main::getInstance());
	States::Manager::getInstance()->changeState(Scenario::Menu::getInstance());

	std::cout << "Loading Done" << std::endl;

	//EVENT LOOP
	while (!States::Manager::done)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			States::Manager::done = true;
		}
		else
		{
			States::Manager::getInstance()->handleEvents(ev);
		}

		GwenInput.ProcessMessage(ev);
		canvas->ProcessDelayedDeletes();

		if (al_is_event_queue_empty(event_queue))
		{
			States::Manager::getInstance()->render();

			frames ++;
			if(al_current_time() - gameTime >= 1)
			{
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}

			al_draw_textf(font, al_map_rgb(255, 255, 0), 10, 680, 0, "FPS: %i", gameFPS);

			al_flip_display();
			al_clear_to_color(al_map_rgb(100, 100, 255));
		}
	}

	al_stop_timer(timer);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);
	al_destroy_font(font);
	al_destroy_display(display);
	return 0;
}