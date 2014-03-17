#include "ctrldisplay.h"

CCtrlDisplay CCtrlDisplay::MCtrlDisplay;

void CCtrlDisplay::Init()
{
	X = 200;
	Y = 200;
	Scale = 0.4f;
	CtrlScale = 0.6f;

	Renderer = new Gwen::Renderer::Allegro();
	Skin = new Gwen::Skin::TexturedBase(Renderer);
	Skin->SetRender(Renderer);
	Skin->Init("Resources/skin.png");
	Skin->SetDefaultFont(L"Resources/OpenSans.ttf", 24*CtrlScale);

	Canvas = new Gwen::Controls::Canvas(Skin);
	Canvas->SetDrawBackground(false);

	Base = new Gwen::Controls::Base(Canvas);
	Base->SetBounds(X+(200*Scale),Y-(250*Scale),480*CtrlScale,300*CtrlScale);

	Input = new Gwen::Input::Allegro();
	Input->Initialize(Canvas);

	ButtonOne = new Gwen::Controls::Button(Base);
	ButtonOne->SetText(L"Nav");
	//ButtonOne->SetTextPadding(Gwen::Padding(0,5*CtrlScale,0,0));
	ButtonOne->SetBounds(40*CtrlScale, 90*CtrlScale, 200*CtrlScale, 50*CtrlScale);
	ButtonOne->onPress.Add(this, &CCtrlDisplay::ButtonNav);

	ButtonTwo = new Gwen::Controls::Button(Base);
	ButtonTwo->SetText(L"Approach 34L");
	//ButtonTwo->SetTextPadding(Gwen::Padding(0,5*CtrlScale,0,0));
	ButtonTwo->SetBounds(40*CtrlScale, 145*CtrlScale, 200*CtrlScale, 50*CtrlScale);

	ButtonThree = new Gwen::Controls::Button(Base);
	ButtonThree->SetText(L"Approach 16R");
	//ButtonThree->SetTextPadding(Gwen::Padding(0,5*CtrlScale,0,0));
	ButtonThree->SetBounds(40*CtrlScale, 200*CtrlScale, 200*CtrlScale, 50*CtrlScale);


	BG = al_load_bitmap("Resources/bg.png");
	CtrlStage = al_load_bitmap("Resources/tower.png");
	LiquidCrystal = al_load_font("Resources/LiquidCrystal.otf", 25, 0);

	//AIRCRAFT INIT
	Aircraft = al_load_bitmap("Resources/aircraft.png");
	Beacon = al_load_bitmap("Resources/strobe.png");
	Strobe = al_load_bitmap("Resources/beacon.png");
	Arrow = al_load_bitmap("Resources/arrow.png");

	CHeading = 0.0f;
	FHeading = 0.0f;
	State = 15;
	Direction = NONE;

	Count = 0;
	BAlpha = 0;
	SAlpha = 0;
	Gate = false;
	Small = false;

	Nav = false;

	Load = al_load_bitmap("Resources/load.png");
	Circle = al_load_bitmap("Resources/circle.png");
	LoadCount = 0;

	InfoRender = al_create_bitmap(480, 300);

	al_identity_transform(&T);
	al_translate_transform(&T, -1000+1200/2, -1000+700/2);
	al_use_transform(&T);
}

void CCtrlDisplay::HandleEvents(ALLEGRO_EVENT &ev)
{
	Input->ProcessMessage(ev);

	switch (ev.type)
	{
	case ALLEGRO_EVENT_TIMER:
		Canvas->ProcessDelayedDeletes();
		Update();
		break;
	case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
		if (Nav)
		{
			FHeading = ArcTan(X-ev.mouse.x, Y-ev.mouse.y)*180.0f/ALLEGRO_PI;
			std::cout << "Set Heading: " << FHeading << std::endl;
			Nav = false;
			ButtonOne->SetDisabled(false);
			ButtonTwo->SetDisabled(false);
			ButtonThree->SetDisabled(false);
		}
		break;
	}
}

void CCtrlDisplay::Update()
{
	LoadCount --;
	//X += sin(CHeading/180*ALLEGRO_PI)/10;
	//Y += -cos(CHeading/180*ALLEGRO_PI)/10;

	//Base->SetPos(X+(200*Scale),Y-(250*Scale));

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

void CCtrlDisplay::ButtonNav()
{
	Nav = true;
	ButtonOne->SetDisabled(true);
	ButtonTwo->SetDisabled(true);
	ButtonThree->SetDisabled(true);
}

void CCtrlDisplay::Render()
{
	//ARROW RENDER
	if (Nav)
	{
		ALLEGRO_MOUSE_STATE state;
		al_get_mouse_state(&state);

		al_draw_scaled_rotated_bitmap(Arrow, 250, 250, X, Y, CtrlScale, CtrlScale, ArcTan(X-state.x, Y-state.y), 0);
	}

	//AIRCRAFT RENDER
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
			al_draw_tinted_scaled_rotated_bitmap_region(Aircraft, 2100, 0, 300, 300, al_map_rgb_f(1,1,1), 150, 150, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
		}
		al_draw_tinted_scaled_rotated_bitmap_region(Aircraft, x, y, 300, 300, al_map_rgb_f(1,1,1), 150, 150, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
		if (Gate)
		{
			al_draw_tinted_scaled_rotated_bitmap_region(Aircraft, 1800, 0, 300, 300, al_map_rgb_f(1,1,1), 150, 150, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
		}
		al_draw_tinted_scaled_rotated_bitmap_region(Beacon, x, y, 500, 500, al_map_rgba_f(BAlpha,BAlpha,BAlpha,BAlpha), 150, 150, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
		al_draw_tinted_scaled_rotated_bitmap_region(Strobe, x, y, 500, 500, al_map_rgba_f(SAlpha,SAlpha,SAlpha,SAlpha), 150, 150, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
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
			al_draw_tinted_scaled_rotated_bitmap_region(Aircraft, 3500, 0, 500, 500, al_map_rgb_f(1,1,1), 250, 250, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
		}
		al_draw_tinted_scaled_rotated_bitmap_region(Aircraft, x, y, 500, 500, al_map_rgb_f(1,1,1), 250, 250, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
		if (Gate)
		{
			al_draw_tinted_scaled_rotated_bitmap_region(Aircraft, 3000, 0, 500, 500, al_map_rgb_f(1,1,1), 250, 250, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
		}
		al_draw_tinted_scaled_rotated_bitmap_region(Beacon, x, y, 500, 500, al_map_rgba_f(BAlpha,BAlpha,BAlpha,BAlpha), 250, 250, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
		al_draw_tinted_scaled_rotated_bitmap_region(Strobe, x, y, 500, 500, al_map_rgba_f(SAlpha,SAlpha,SAlpha,SAlpha), 250, 250, X, Y, Scale, Scale, CHeading/180.0f*ALLEGRO_PI, 0);
	}


	//INFO DISPLAY RENDER
	ALLEGRO_BITMAP* Display = al_get_target_bitmap();
	al_set_target_bitmap(InfoRender);
	al_clear_to_color(al_map_rgba_f(0, 0, 0, 0));

	al_draw_bitmap(BG, 0, 0, 0);

	al_draw_bitmap(CtrlStage, 326, 98, 0);

	al_draw_textf(LiquidCrystal, al_map_rgb(0,255,210), 36, 29, 0, "TG 458");
	al_draw_textf(LiquidCrystal, al_map_rgb(255,255,255), 174, 29, 0, "240");

	al_draw_textf(LiquidCrystal, al_map_rgb(255,255,255), 274, 29, 0, "%03.0f", CHeading);

	if (true == false)
	{
		al_draw_textf(LiquidCrystal, al_map_rgb(255,255,255), 374, 29, 0, "5000");
	}
	else
	{
		al_draw_textf(LiquidCrystal, al_map_rgb(0,246,255), 374, 29, 0, "FL");
		al_draw_textf(LiquidCrystal, al_map_rgb(255,255,255), 398, 29, 0, "370");
	}



	al_set_target_bitmap(Display);
	al_draw_scaled_bitmap(InfoRender, 0, 0, 480, 300, X+(200*Scale), Y-(250*Scale), 480.0f*CtrlScale, 300.0f*CtrlScale, 0);
	
	//LOADING SCREEN
	al_draw_scaled_bitmap(Load, 0, 0, 1920, 1080, 300, 300, 500, 500/16.0f*9.0f, 0);
	al_draw_scaled_rotated_bitmap(Circle, 120, 120, 300+(1700/1920.0f*500.0f), 300+(910/1920.0f*500.0f), 500.0f/1920.0f, 500.0f/1920.0f, LoadCount/90.0f*ALLEGRO_PI, 0);


	Canvas->RenderCanvas();

	al_draw_circle(1000, 1000, 10, al_map_rgb(255,0,255), 10);
}