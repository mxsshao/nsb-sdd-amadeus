#pragma once

#include "global.h"

class CAircraft : public Gwen::Event::Handler
{
private:
	ALLEGRO_TRANSFORM T;

	int x;
	int y;

	ALLEGRO_BITMAP* left;
	ALLEGRO_BITMAP* arrow;
	bool Gate;
	bool Small;

	ALLEGRO_BITMAP* beacon;
	ALLEGRO_BITMAP* strobe;
	int Count;
	double BAlpha;
	double SAlpha;

	ALLEGRO_BITMAP* loading;
	ALLEGRO_BITMAP* loading2;
	int Circle;

	double CHeading;
	double FHeading;
	double SHeading;
	int Direction; //L1 R2
	enum Direction {NONE, LEFT, RIGHT};
	double State; //0-30
	bool Lock;

	double ArcTan(float x, float y)
	{
		if (x <= 0 && y >= 0)
			return abs(atan(x/y));
		else if (x <= 0 && y <= 0)
			return ALLEGRO_PI - abs(atan(x/y));
		else if (x >= 0 && y <= 0)
			return ALLEGRO_PI + abs(atan(x/y));
		else if (x >= 0 && y >= 0)
			return ALLEGRO_PI*2 - abs(atan(x/y));
		else
			return 0;
	};

public:
	CAircraft();

	void Init();
	void HandleEvents(ALLEGRO_EVENT &ev);
	void Render();
	
};