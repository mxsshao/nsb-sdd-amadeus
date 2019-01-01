#include "HUD_PFD.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

HUD_PFD::HUD_PFD()
{
	base = al_load_bitmap("base.png");
	beacon = al_load_bitmap("beacon.png");
	strobe = al_load_bitmap("strobe.png");
	count = 0;
	BAlpha = 0;
	SAlpha = 0;
}

void HUD_PFD::Update()
{
	count++;


	if (count < 5)
	{
		BAlpha += 1/5.0f;
	}
	else if (count < 10)
	{
		BAlpha -= 1/5.0f;
		if (BAlpha < 0) {BAlpha = 0;}
	}
	else if (count < 15)
	{
	}
	else if (count < 20)
	{
		SAlpha += 1/5.0f;
	}
	else if (count < 25)
	{
		SAlpha -= 1/5.0f;
		if (SAlpha < 0) {SAlpha = 0;}
	}
	else if (count < 30)
	{
		SAlpha += 1/5.0f;
	}
	else if (count < 35)
	{
		SAlpha -= 1/5.0f;
		if (SAlpha < 0) {SAlpha = 0;}
	}

	if (count >= 80)
	{
		count = 0;
	}

}

void HUD_PFD::Render()
{
	//al_draw_bitmap(base, 0, 0, 0);
	al_draw_bitmap_region(base, 0, 0, 500, 500, 0, 0, 0);
	al_draw_tinted_bitmap_region(beacon, al_map_rgba_f(1,1,1,BAlpha), 0, 0, 500, 500, 0, 0, 0);
	al_draw_tinted_bitmap_region(strobe, al_map_rgba_f(1,1,1,SAlpha), 0, 0, 500, 500, 0, 0, 0);
	//al_draw_tinted_bitmap(beacon, al_map_rgba_f(1,1,1,BAlpha), 0, 0, 0);
	//al_draw_tinted_bitmap(strobe, al_map_rgba_f(1,1,1,SAlpha), 0, 0, 0);

}

HUD_PFD::~HUD_PFD()
{
	al_destroy_bitmap(base);
	al_destroy_bitmap(beacon);
	al_destroy_bitmap(strobe);
}
