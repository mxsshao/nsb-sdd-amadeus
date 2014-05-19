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
		Gwen::Controls::Button* button3;
		void button3Click();
		Gwen::Controls::Button* button4;
		void button4Click() {
			States::Manager::done = true;
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