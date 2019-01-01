#include "SQ380.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

SQ380::SQ380()
{
	std::cout << "/-----------------------------------------------------------/" << std::endl;
	std::cout << "	Amadeus Testing Platform" << std::endl;
	std::cout << "		v1.0.1" << std::endl;
	std::cout << "/-----------------------------------------------------------/" << std::endl;
	std::cout << std::endl;

	left = al_load_bitmap("LEFT.png");
	arrow = al_load_bitmap("arrow.png");
	beacon = al_load_bitmap("strobe.png");
	strobe = al_load_bitmap("beacon.png");


	loading = al_load_bitmap("loading.png");
	loading2 = al_load_bitmap("loading2.png");

	CHeading = 0.0f;
	FHeading = 0.0f;
	SHeading = 0.0f;
	State = 15;
	Direction = NONE;
	Lock = false;

	Count = 0;
	BAlpha = 0;
	SAlpha = 0;

	Circle = 0;

	Gate = false;

	test = al_load_bitmap("test.png");

	TEMP = 0.0f;
	CLICK = false;
}

void SQ380::Update()
{
	Circle --;


	/*if ((CHeading != FHeading) && (Direction == NONE))
	{
		
	}*/

	if (!Gate)
	{
	if (CHeading != FHeading)
	{
		double Dif = CHeading - FHeading;

		if (Dif > 0)
		{
			Direction = (Dif < 180) ? LEFT:RIGHT;
		}
		else
		{
			Direction = (Dif < -180) ? LEFT:RIGHT;
		}

		if (Direction == LEFT)
		{
			CHeading = fmod(CHeading-0.2f, 360);
			if (CHeading < 0)
			{
				CHeading = CHeading + 360;
			}

			if ((abs(CHeading - FHeading) < 2.6f) && (State < 14))
			{
				State ++;
			}
			else if (State > 0)
			{
				State --;
			}

		}
		else if (Direction == RIGHT)
		{
			CHeading = fmod(CHeading+0.2f, 360);
			if (CHeading < 0)
			{
				CHeading = CHeading + 360;
			}

			if ((abs(CHeading - FHeading) < 2.6f) && (State > 16))
			{
				State --;
			}
			else if (State < 30)
			{
				State ++;
			}
		}
	}

	if (abs(CHeading - FHeading) < 0.2f)
	{
		Direction = NONE;
		CHeading = FHeading;
		State = 15;
	}
	}


	//---------------
	//LIGHTING
	//---------------

	Count++;


	if (Count < 5)
	{
		BAlpha += 1/5.0f;
	}
	else if (Count < 10)
	{
		BAlpha -= 1/5.0f;
		if (BAlpha < 0) {BAlpha = 0;}
	}
	else if (Count < 15)
	{
	}
	else if (Count < 20)
	{
		SAlpha += 1/5.0f;
	}
	else if (Count < 25)
	{
		SAlpha -= 1/5.0f;
		if (SAlpha < 0) {SAlpha = 0;}
	}
	else if (Count < 30)
	{
		SAlpha += 1/5.0f;
	}
	else if (Count < 35)
	{
		SAlpha -= 1/5.0f;
		if (SAlpha < 0) {SAlpha = 0;}
	}

	if (Count >= 80)
	{
		Count = 0;
	}



	if (CLICK && (TEMP != 2.0f))
	{
		TEMP += 0.1f;
		if (TEMP > 2.0f) {TEMP = 2.0f;}
	}
	else if (!CLICK && (TEMP != 0.0f))
	{
		TEMP -= 0.1f;
		if (TEMP < 0.0f) {TEMP = 0.0f;}
	}

}

void SQ380::ButtonDown(int x, int y)
{
	FHeading = ArcTan(250-x, 250-y)*180.0f/ALLEGRO_PI;
	std::cout << "Set Heading: " << FHeading << std::endl;

	CLICK = !CLICK;
}

void SQ380::Render()
{
	/*ALLEGRO_MOUSE_STATE state;
	al_get_mouse_state(&state);

	int AIRX = 300;
	int AIRY = 300;

	al_draw_rotated_bitmap(arrow, 250, 250, AIRX, AIRY, ArcTan(250-state.x, 250-state.y), 0);

	int x = 0;
	int y = 0;

	if (State <= 14)
	{
		y = 1000;
		x = floor((14 - State) / 2) * 500;
	}
	else if (State >= 16)
	{
		y = 500;
		x = ceil((State - 16) / 2) * 500;
	}

	if (Gate)
	{
		al_draw_tinted_scaled_rotated_bitmap_region(left, 3500, 0, 500, 500, al_map_rgb_f(1,1,1), 250, 250, AIRX, AIRY, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
	}
	al_draw_tinted_scaled_rotated_bitmap_region(left, x, y, 500, 500, al_map_rgb_f(1,1,1), 250, 250, AIRX, AIRY, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
	if (Gate)
	{
		al_draw_tinted_scaled_rotated_bitmap_region(left, 3000, 0, 500, 500, al_map_rgb_f(1,1,1), 250, 250, AIRX, AIRY, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
	}
	al_draw_tinted_scaled_rotated_bitmap_region(beacon, x, y, 500, 500, al_map_rgba_f(1,1,1, BAlpha), 250, 250, AIRX, AIRY, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
	al_draw_tinted_scaled_rotated_bitmap_region(strobe, x, y, 500, 500, al_map_rgba_f(1,1,1, SAlpha), 250, 250, AIRX, AIRY, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);*/
	
	
	//al_draw_rotated_bitmap(loading, 100, 100, 750, 250, Circle/180.0f*ALLEGRO_PI, 0);
	//al_draw_rotated_bitmap(loading2, 100, 100, 750, 500, Circle/180.0f*ALLEGRO_PI, 0);

	al_draw_tinted_scaled_rotated_bitmap_region(test, 511, 511, 1, 1, al_map_rgba_f(0.5, 0.5, 0.5, 0.5), 0, 0, 0, 0, 800, 500, 0, 0);
	//al_draw_bitmap_region(test, 0, 0, 510, 70, 100, 100, 0);

	if (TEMP <= 1.0f)
	{
	al_draw_tinted_scaled_rotated_bitmap_region(test, 0, 0, 510, 80, al_map_rgb_f(1,1,1), 255, 40, 500, 200, 1, 1-TEMP, 0, 0);
	}
	if (TEMP >= 1.0f)
	{
	al_draw_tinted_scaled_rotated_bitmap_region(test, 0, 80, 510, 80, al_map_rgb_f(1,1,1), 255, 40, 500, 200, 1, TEMP-1, 0, 0);
	}
	
}

SQ380::~SQ380()
{
}