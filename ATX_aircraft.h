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

		static Structs::GuiImage gBase;
		static Structs::GuiImage gSelectedBase;
		static Structs::GuiImage gBarGreen;
		static Structs::GuiImage gAirline;
		static Structs::GuiText gAircraft;
		static Structs::GuiText gFlight;
		static Structs::GuiText gDestination;
		static ALLEGRO_BITMAP* aircraftButton;
		static ALLEGRO_FONT* nFonts[2];
		static ATX::Structs::Waypoint nWaypoints[20];

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
		float flipCount;

		float offset;

	public:
		Aircraft(Gwen::Controls::Base* parent, float ex, float ey, float ez, int start, double eSpeed, double eHeading, int eDestination, std::string type);
		~Aircraft();

		static void initialize();

		bool done;

		void resize();

		void navigate(std::list<int>* destination);
		void update();
		void render();
		void renderLines();
		void select();

		float getX() {return x;};
		float getY() {return y;};
		float getZ() {return z;};
		bool getSelected() {return isSelected;};
		void setSelected(bool s) {isSelected = s;};

		static ATX::Structs::Waypoint* getWaypoint(int index) {return &nWaypoints[index];};
	};
};