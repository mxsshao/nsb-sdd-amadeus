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
	void Render(ALLEGRO_BITMAP* bitmap)
	{
		al_draw_bitmap_region(bitmap, sx, sy, sw, sh, dx, dy, 0);
	};
};

class ATX_flight_display : public Gwen::Event::Handler
{
protected:
	ATX_flight_display() {};

private:
	static ATX_flight_display mATX_flight_display;
	ALLEGRO_BITMAP* bitmap;
	GuiImage base;
	GuiImage barGreen;

public:
	static ATX_flight_display* getInstance() {return &mATX_flight_display;};

	void initialize();
	void handleEvents(ALLEGRO_EVENT &ev);
	void render();

};