#pragma once

#include "global.h"

class CCtrlDisplay : public Gwen::Event::Handler
{
protected:
	CCtrlDisplay() {};

private:
	ALLEGRO_TRANSFORM T;

	float X;
	float Y;
	double Scale;

	Gwen::Renderer::Allegro* Renderer;
	Gwen::Skin::TexturedBase* Skin;
	Gwen::Controls::Canvas* Canvas;
	Gwen::Controls::Base* Base;
	Gwen::Input::Allegro* Input;
	Gwen::Controls::Button* ButtonOne;
	Gwen::Controls::Button* ButtonTwo;
	Gwen::Controls::Button* ButtonThree;

	//INFO DISPLAY
	ALLEGRO_BITMAP* CtrlStage;
	ALLEGRO_BITMAP* BG;
	ALLEGRO_FONT* LiquidCrystal;
	double CtrlScale;

	ALLEGRO_BITMAP* InfoRender;

	//AIRCRAFT IMAGE
	ALLEGRO_BITMAP* Aircraft;
	ALLEGRO_BITMAP* Beacon;
	ALLEGRO_BITMAP* Strobe;
	ALLEGRO_BITMAP* Arrow;

	//AIRCRAFT SETTINGS
	bool Gate;
	bool Small;

	//AIRCRAFT LIGHTS
	int Count;
	double BAlpha;
	double SAlpha;

	//AIRCRAFT VARIABLES
	double CHeading;
	double FHeading;
	int Direction; //L1 R2
	enum Direction {NONE, LEFT, RIGHT};
	double State; //0-30

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

	void Update();

	//CTRL
	bool Nav;
	void ButtonNav();

	//LOADING SCREEN
	ALLEGRO_BITMAP* Load;
	ALLEGRO_BITMAP* Circle;
	int LoadCount;

public:
	static CCtrlDisplay MCtrlDisplay;

	void Init();
	void HandleEvents(ALLEGRO_EVENT &ev);
	void Render();

};