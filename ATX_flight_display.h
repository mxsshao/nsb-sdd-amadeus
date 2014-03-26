#pragma once

#include "global.h"


struct GuiImage
{
	float sx;
	float sy;
	float sw;
	float sh;
	float dx;
	float dy;
	GuiImage()
	{
	};
	GuiImage(float sx, float sy, float sw, float sh, float dx, float dy)
	{
		GuiImage::sx = sx;
		GuiImage::sy = sy;
		GuiImage::sw = sw;
		GuiImage::sh = sh;
		GuiImage::dx = dx;
		GuiImage::dy = dy;
	};
};

struct GuiText
{
	float dx;
	float dy;
	int align;
	int size;
	ALLEGRO_COLOR color;
	GuiText()
	{
	};
	GuiText(float dx, float dy, int align, int size, int r, int g, int b)
	{
		GuiText::dx = dx;
		GuiText::dy = dy;
		GuiText::align = align;
		GuiText::size = size;
		GuiText::color = al_map_rgb(r, g, b);
	};
};

class ATX_flight_display : public Gwen::Event::Handler
{
protected:
	ATX_flight_display() {};

private:
	static ATX_flight_display mATX_flight_display;

	Gwen::Controls::Base* canvas;
	Gwen::Controls::Button* button;

	ALLEGRO_BITMAP* bitmap;
	ALLEGRO_FONT* fonts[2];
	GuiImage base;
	GuiImage barGreen;
	GuiText aircraft;
	GuiText flight;

	ALLEGRO_BITMAP* bg;

	Gwen::Controls::WindowControl* window;

public:
	static ATX_flight_display* getInstance() {return &mATX_flight_display;};

	void initialize(Gwen::Controls::Base* pCanvas);
	void handleEvents(ALLEGRO_EVENT &ev);
	void render();

};