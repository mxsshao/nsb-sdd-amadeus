#pragma once
#include "Globals.h"

class HUD_PFD
{
private:
	ALLEGRO_FONT *font;

	ALLEGRO_DISPLAY *display;
	ALLEGRO_BITMAP* base;
	ALLEGRO_BITMAP* beacon;
	ALLEGRO_BITMAP* strobe;

	int width;
	int height;
	int count;

	double BAlpha;
	double SAlpha;

public:
	HUD_PFD();

	void Update();
	void Render();

	~HUD_PFD();
};

