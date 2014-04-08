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

	public:
		static Menu* getInstance() {return &mMenu;};

		void load(States::Manager* manager) {};
		void initialize(States::Manager* manager);

		void pause() {};
		void resume() {};

		void handleEvents(ALLEGRO_EVENT &ev) {};
		void render() {al_draw_bitmap(back, 0, 0, 0);};

		void cleanup() {};

	};
};