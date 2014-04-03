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
		Gwen::Controls::WindowControl* window;
		Gwen::Controls::WindowControl* testControl;
		Gwen::Controls::Button* button1;
		Gwen::Controls::Button* button2;
		void windowResize();
		Gwen::Controls::WindowControl* radarWindow;
		void radarWindowResize();

		int offsetWidth;
		int offsetHeight;

		ALLEGRO_TRANSFORM transform;
		Structs::Camera camera;

		std::list<Aircraft*> nAircraft;
		std::list<Aircraft*>::iterator iter;
		ALLEGRO_BITMAP* bg;

		void update();
		void breakaway();

	public:
		static Main* getInstance() {return &mMain;};

		ALLEGRO_BITMAP* bar;

		void initialize(int displayW, int displayH, Gwen::Controls::Base* pCanvas);

		void resetSelected();

		void handleEvents(ALLEGRO_EVENT &ev);
		void render();

		Structs::Camera* getCamera() {return &camera;};
		void setDisplayOffset(int displayW, int displayH) {offsetWidth = displayW/2; offsetHeight = displayH/2;};

	};
};