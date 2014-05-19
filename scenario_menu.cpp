#include "scenario_menu.h"
#include "ATX.h"

Scenario::Menu Scenario::Menu::mMenu;

void Scenario::Menu::initialize(States::Manager* manager)
{
	canvas = manager->getCanvas();
	scroll = new Gwen::Controls::ScrollControl(canvas);
	scroll->SetAutoHideBars(false);
	scroll->SetScroll(true, false);
	scroll->GetHScroll()->onBarMoved.Add(this, &Scenario::Menu::scrollMove);

	position = 0;

	padding = new Gwen::Controls::Base(scroll);
	padding->SetSize(0,0);
	padding->SetDisabled(true);

	scenario1 = al_load_bitmap("Resources/scenario1.png");
	scenario2 = al_load_bitmap("Resources/scenario2.png");
	scenario3 = al_load_bitmap("Resources/scenario3.png");
	scenarioButtons = al_load_bitmap("Resources/scenariobuttons.png");
	scenarioButtons2 = al_load_bitmap("Resources/scenariobuttons2.png");

	button1 = new Gwen::Controls::Button(scroll);
	button1->SetShouldDrawBackground(false);
	button1->SetImage(al_create_sub_bitmap(scenarioButtons, 0, 0, 340, 206));
	button1->SetCursor(Gwen::CursorType::Finger);
	button1->onPress.Add(this, &Scenario::Menu::windowCreate);
	button1->onHoverEnter.Add(this, &Scenario::Menu::button1HoverEnter);
	button1->onHoverLeave.Add(this, &Scenario::Menu::button1HoverLeave);

	button2 = new Gwen::Controls::Button(scroll);
	button2->SetShouldDrawBackground(false);
	button2->SetImage(al_create_sub_bitmap(scenarioButtons, 0, 256, 446, 206));
	button2->SetCursor(Gwen::CursorType::Finger);
	button2->onPress.Add(this, &Scenario::Menu::windowCreate);
	button2->onHoverEnter.Add(this, &Scenario::Menu::button2HoverEnter);
	button2->onHoverLeave.Add(this, &Scenario::Menu::button2HoverLeave);

	button3 = new Gwen::Controls::Button(scroll);
	button3->SetShouldDrawBackground(false);
	button3->SetImage(al_create_sub_bitmap(scenarioButtons, 0, 512, 438, 206));
	button3->SetCursor(Gwen::CursorType::Finger);
	button3->onPress.Add(this, &Scenario::Menu::windowCreate);
	button3->onHoverEnter.Add(this, &Scenario::Menu::button3HoverEnter);
	button3->onHoverLeave.Add(this, &Scenario::Menu::button3HoverLeave);

	button4 = new Gwen::Controls::Button(scroll);
	button4->SetShouldDrawBackground(false);
	button4->SetImage(al_create_sub_bitmap(scenarioButtons, 0, 768, 390, 206));
	button4->SetCursor(Gwen::CursorType::Finger);
	button4->onPress.Add(this, &Scenario::Menu::windowCreate);
	button4->onHoverEnter.Add(this, &Scenario::Menu::button4HoverEnter);
	button4->onHoverLeave.Add(this, &Scenario::Menu::button4HoverLeave);

	button5 = new Gwen::Controls::Button(scroll);
	button5->SetShouldDrawBackground(false);
	button5->SetImage(al_create_sub_bitmap(scenarioButtons, 512, 0, 504, 206));
	button5->SetCursor(Gwen::CursorType::Finger);
	button5->onPress.Add(this, &Scenario::Menu::windowCreate);
	button5->onHoverEnter.Add(this, &Scenario::Menu::button5HoverEnter);
	button5->onHoverLeave.Add(this, &Scenario::Menu::button5HoverLeave);

	buttonBack = new Gwen::Controls::Button(canvas);
	buttonBack->SetText(L"Back");
	buttonBack->SetCursor(Gwen::CursorType::Finger);

	resize(manager);
}

void Scenario::Menu::resize(States::Manager* manager)
{
	scroll->Dock(Gwen::Pos::Fill);

	float ratio = al_get_display_height(manager->getDisplay())/1080.0f;
	
	width = ratio * 5760.0f;
	height = al_get_display_height(manager->getDisplay());
	offset = al_get_display_width(manager->getDisplay());

	position = (width-offset) * scroll->GetHScroll()->GetScrolledAmount();

	padding->SetPos(width, 0);

	button1->SetSize(340*ratio, 206*ratio);
	button1->SetPos(554*ratio, 542*ratio);
	button2->SetSize(446*ratio, 206*ratio);
	button2->SetPos(1172*ratio, 340*ratio);
	button3->SetSize(438*ratio, 206*ratio);
	button3->SetPos(1730*ratio, 678*ratio);
	button4->SetSize(390*ratio, 206*ratio);
	button4->SetPos(2462*ratio, 517*ratio);
	button5->SetSize(504*ratio, 206*ratio);
	button5->SetPos(3174*ratio, 456*ratio);

	buttonBack->SetSize(300*ratio, 80*ratio);
	buttonBack->SetPos(20*ratio, 960*ratio);

	out = offset / height > 5760.0f / 1080.0f;
	
	//windowResize();
}

void Scenario::Menu::windowButtonClick()
{
	States::Manager::getInstance()->changeState(ATX::Main::getInstance());
	//States::Manager::getInstance()->changeState(Scenario::Menu::getInstance());
}

void Scenario::Menu::windowCreate()
{
	//FIX DOUBLE CLICKING BUTTON CREATING MORE WINDOWS

	//FIX UNABLE TO RESIZE WINDOWS -> POINTERS NOT BEING CLEARED
		window = new Gwen::Controls::WindowControl(canvas);
		window->MakeModal();
		window->SetClosable(true);
		window->DisableResizing();
		window->SetTitle(L"Scenario");
		window->SetDeleteOnClose(true);

		windowButton = new Gwen::Controls::Button(window);
		windowButton->SetText(L"Start");
		windowButton->onPress.Add(this, &Scenario::Menu::windowButtonClick);

		windowResize();
}

void Scenario::Menu::windowResize()
{
	if (window)
	{
		window->SetSize(400, 300);
		window->SetPos((offset - 400)/2, (height-300)/2);
		windowButton->SetSize(400,100);
		windowButton->Dock(Gwen::Pos::Top);
	}
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

	if (out)
	{
		al_draw_scaled_bitmap(scenario3, 0, 0, 1920, 1080, width/3.0f*4.0f - position, 0, -width/3.0f, height, 0);
	}
}

void Scenario::Menu::cleanup()
{
	scroll->DelayedDelete();
	buttonBack->DelayedDelete();
	if (window)
		window->CloseButtonPressed();

	al_destroy_bitmap(scenario1);
	al_destroy_bitmap(scenario2);
	al_destroy_bitmap(scenario3);

	al_destroy_bitmap(scenarioButtons);
	al_destroy_bitmap(scenarioButtons2);


}