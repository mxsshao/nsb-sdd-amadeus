#pragma once
#include "global.h"
#include "ATX_structs.h"
#include "ATX_aircraft.h"

namespace ATX
{
	class Main : public Gwen::Event::Handler
	{
	protected:
		Main() {};

	private:
		static Main mMain;

		ALLEGRO_BITMAP* screen;

		bool keys [5];
		enum keys {UP, DOWN, LEFT, RIGHT, LSHIFT};
		bool rClick;

		Gwen::Controls::Base* canvas;
		Gwen::Controls::Button* button1;
		Gwen::Controls::WindowControl* window;

		int offsetWidth;
		int offsetHeight;

		ALLEGRO_TRANSFORM transform;
		Structs::Waypoint camera;

		std::list<Aircraft*> nAircraft;
		std::list<Aircraft*>::iterator iter;
		ALLEGRO_BITMAP* bg;

		void update();

	public:
		static Main* getInstance() {return &mMain;};

		ALLEGRO_BITMAP* bar;

		void initialize(int displayW, int displayH, Gwen::Controls::Base* pCanvas);
		void handleEvents(ALLEGRO_EVENT &ev);
		void render();

		void setDisplayOffset(int displayW, int displayH) {offsetWidth = displayW/2; offsetHeight = displayH/2;};

	};
};