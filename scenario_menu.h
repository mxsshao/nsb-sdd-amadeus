#pragma once
#include "global.h"
#include "states_manager.h"

namespace Scenario
{
	class Menu : public States::Base
	{
	protected:
		Menu() {};

	private:
		static Menu mMenu;

		Gwen::Controls::Base* canvas;
		Gwen::Controls::ScrollControl* scroll;
		void scrollMove();
		Gwen::Controls::Button* button1;
		Gwen::Controls::Button* buttonBack;
		Gwen::Controls::Base* padding;

		ALLEGRO_BITMAP* scenario1;
		ALLEGRO_BITMAP* scenario2;
		ALLEGRO_BITMAP* scenario3;
		float width;
		float height;
		float offset;
		float position;

		ALLEGRO_BITMAP* scenarioButtons;

	public:
		static Menu* getInstance() {return &mMenu;};

		void load(States::Manager* manager) {};
		void initialize(States::Manager* manager);

		void pause() {};
		void resume() {};

		void handleEvents(ALLEGRO_EVENT &ev);
		void render();

		void cleanup() {};
	};
};