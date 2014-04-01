#pragma once
#include "global.h"
#include "ATX_structs.h"

namespace ATX
{
	class Aircraft : public Gwen::Event::Handler
	{
	private:
		ALLEGRO_BITMAP* plane;
		ALLEGRO_BITMAP* buttonImage;
		ALLEGRO_TRANSFORM transform;
		int state;

		Gwen::Controls::Button* button;
		void buttonClick();

		Structs::GuiImage base;
		Structs::GuiImage barGreen;
		Structs::GuiText aircraft;
		Structs::GuiText flight;

		double speed;
		double currentHeading;
		double finalHeading;
		int direction;
		enum direction {NONE, LEFT, RIGHT};
		float x;
		float y;
		float z;
		std::list<Structs::Waypoint> nPoints;
		int previous;

		bool isSelected;
		float offset;

	public:
		Aircraft(Gwen::Controls::Base* parent, int start, double eSpeed, double eHeading, int eDestination);
		static ALLEGRO_BITMAP* aircraftButton;
		static ALLEGRO_FONT* nFonts[2];
		static ATX::Structs::Waypoint nWaypoints[20];

		void navigate(std::list<int>* destination);
		void update();
		void render();
		void renderLines();
	};
};