#include "aircraft.h"

CAircraft::CAircraft()
{
}

void CAircraft::Init()
{
	

	left = al_load_bitmap("LEFT.png");
	arrow = al_load_bitmap("arrow.png");
	beacon = al_load_bitmap("strobe.png");
	strobe = al_load_bitmap("beacon.png");

	loading = al_load_bitmap("loading.png");
	loading2 = al_load_bitmap("loading2.png");

	CHeading = 0.0f;
	FHeading = 0.0f;
	State = 15;
	Direction = NONE;
	Lock = false;

	Count = 0;
	BAlpha = 0;
	SAlpha = 0;

	Circle = 0;

	Gate = false;
	Small = true;



	al_identity_transform(&T);
	al_translate_transform(&T, -500, -500);
	al_use_transform(&T);
}
void CAircraft::HandleEvents(ALLEGRO_EVENT &ev)
{
	if (ev.type == ALLEGRO_EVENT_TIMER)
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
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		FHeading = ArcTan(250-x, 250-y)*180.0f/ALLEGRO_PI;
		std::cout << "Set Heading: " << FHeading << std::endl;
	}
}
void CAircraft::Render()
{

	ALLEGRO_MOUSE_STATE state;
	al_get_mouse_state(&state);

	al_draw_rotated_bitmap(arrow, 250, 250, 250, 250, ArcTan(250-state.x, 250-state.y), 0);



	int x = 0;
	int y = 0;

	

	if (Small)
	{
		if (State <= 14)
		{
			y = 600;
			x = floor((14 - State) / 2) * 300;
		}
		else if (State >= 16)
		{
			y = 300;
			x = ceil((State - 16) / 2) * 300;
		}

		if (Gate)
		{
			al_draw_tinted_scaled_rotated_bitmap_region(left, 2100, 0, 300, 300, al_map_rgb_f(1,1,1), 150, 150, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
		}
		al_draw_tinted_scaled_rotated_bitmap_region(left, x, y, 300, 300, al_map_rgb_f(1,1,1), 150, 150, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
		if (Gate)
		{
			al_draw_tinted_scaled_rotated_bitmap_region(left, 1800, 0, 300, 300, al_map_rgb_f(1,1,1), 150, 150, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
		}
		//al_draw_tinted_scaled_rotated_bitmap_region(beacon, x, y, 500, 500, al_map_rgba_f(1,1,1, BAlpha), 250, 250, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
		//al_draw_tinted_scaled_rotated_bitmap_region(strobe, x, y, 500, 500, al_map_rgba_f(1,1,1, SAlpha), 250, 250, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
	}
	else
	{
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
			al_draw_tinted_scaled_rotated_bitmap_region(left, 3500, 0, 500, 500, al_map_rgb_f(1,1,1), 250, 250, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
		}
		al_draw_tinted_scaled_rotated_bitmap_region(left, x, y, 500, 500, al_map_rgb_f(1,1,1), 250, 250, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
		if (Gate)
		{
			al_draw_tinted_scaled_rotated_bitmap_region(left, 3000, 0, 500, 500, al_map_rgb_f(1,1,1), 250, 250, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
		}
		//al_draw_tinted_scaled_rotated_bitmap_region(beacon, x, y, 500, 500, al_map_rgba_f(1,1,1, BAlpha), 250, 250, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
		//al_draw_tinted_scaled_rotated_bitmap_region(strobe, x, y, 500, 500, al_map_rgba_f(1,1,1, SAlpha), 250, 250, 250, 250, 1.0f, 1.0f, CHeading/180.0f*ALLEGRO_PI, 0);
	}
	
	al_draw_rotated_bitmap(loading, 100, 100, 750, 250, Circle/180.0f*ALLEGRO_PI, 0);
	al_draw_rotated_bitmap(loading2, 100, 100, 750, 500, Circle/180.0f*ALLEGRO_PI, 0);
}