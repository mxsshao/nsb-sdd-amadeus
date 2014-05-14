#pragma once
#include "global.h"
#include "ATX.h"

namespace ATX
{
	class Menu : public States::Base
	{
	protected:
		Menu() {};

	private:
		static Menu mMenu;

		ALLEGRO_BITMAP* back;

		Gwen::Controls::WindowControl* window;
		Gwen::Controls::Button* button1;
		void button1Click() {States::Manager::getInstance()->resumeState();};
		Gwen::Controls::Button* button2;
		void button2Click() {
			al_toggle_display_flag(al_get_current_display(), ALLEGRO_FULLSCREEN_WINDOW, button2->GetToggleState());
		};

	public:
		static Menu* getInstance() {return &mMenu;};

		void load(States::Manager* manager) {};
		void initialize(States::Manager* manager);

		void pause() {};
		void resume() {};

		void handleEvents(ALLEGRO_EVENT &ev) {};
		void render() {al_draw_bitmap(back, 0, 0, 0);};

		void cleanup() {al_destroy_bitmap(back); window->DelayedDelete();};

	};
};