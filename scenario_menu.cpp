#include "scenario_menu.h"

Scenario::Menu Scenario::Menu::mMenu;

void Scenario::Menu::initialize(States::Manager* manager)
{
	canvas = manager->getCanvas();
	scroll = new Gwen::Controls::ScrollControl(canvas);
	scroll->Dock(Gwen::Pos::Fill);
	scroll->SetAutoHideBars(false);
	scroll->SetScroll(true, false);
	scroll->GetHScroll()->onBarMoved.Add(this, &Scenario::Menu::scrollMove);

	float ratio = al_get_display_height(manager->getDisplay())/1080.0f;

	width = ratio * 5760.0f;
	height = al_get_display_height(manager->getDisplay());
	offset = al_get_display_width(manager->getDisplay());
	position = 0;

	padding = new Gwen::Controls::Base(scroll);
	padding->SetSize(0,0);
	padding->SetPos(width, 0);
	padding->SetDisabled(true);

	scenario1 = al_load_bitmap("Resources/scenario1.png");
	scenario2 = al_load_bitmap("Resources/scenario2.png");
	scenario3 = al_load_bitmap("Resources/scenario3.png");
	scenarioButtons = al_load_bitmap("Resources/scenariobuttons.png");

	button1 = new Gwen::Controls::Button(scroll);
	button1->SetShouldDrawBackground(false);
	button1->SetImage(al_create_sub_bitmap(scenarioButtons, 0, 0, 434, 206));
	button1->SetSize(434*ratio, 206*ratio);
	button1->SetPos(320*ratio, 540*ratio);
	button1->SetCursor(Gwen::CursorType::Finger);

	buttonBack = new Gwen::Controls::Button(canvas);
	buttonBack->SetText(L"Back");
	buttonBack->SetSize(300*ratio, 80*ratio);
	buttonBack->SetPos(20*ratio, 960*ratio);
	buttonBack->SetCursor(Gwen::CursorType::Finger);
}

void Scenario::Menu::scrollMove()
{
	position = (width-offset) * scroll->GetHScroll()->GetScrolledAmount();
}

void Scenario::Menu::handleEvents(ALLEGRO_EVENT &ev)
{
}

void Scenario::Menu::render()
{
	al_draw_scaled_bitmap(scenario1, 0, 0, 1920, 1080, 0 - position, 0, width/3.0f, height, 0);
	al_draw_scaled_bitmap(scenario2, 0, 0, 1920, 1080, width/3.0f - position, 0, width/3.0f, height, 0);
	al_draw_scaled_bitmap(scenario3, 0, 0, 1920, 1080, width/3.0f*2.0f - position, 0, width/3.0f, height, 0);
}