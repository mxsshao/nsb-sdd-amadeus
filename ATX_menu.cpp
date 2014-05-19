#include "ATX_menu.h"
#include "scenario_menu.h"

ATX::Menu ATX::Menu::mMenu;

void ATX::Menu::initialize(States::Manager* manager)
{
	int displayW = al_get_display_width(manager->getDisplay());
	int displayH = al_get_display_height(manager->getDisplay());

	back = al_create_bitmap(displayW, displayH);

	ALLEGRO_BITMAP* display = al_get_target_bitmap();
	al_set_target_bitmap(back);
	ATX::Main::getInstance()->render();
	al_set_target_bitmap(display);

	window = new Gwen::Controls::WindowControl(manager->getCanvas());
	window->MakeModal();
	window->SetClosable(false);
	window->DisableMovement();
	window->DisableResizing();
	window->SetTitle(L"Pause");
	window->SetSize(400, 300);
	window->SetPos((displayW - 400)/2, (displayH-300)/2);

	button1 = new Gwen::Controls::Button(window);
	button1->SetText(L"Resume");
	button1->SetSize(200, 60);
	button1->Dock(Gwen::Pos::Top);
	button1->onPress.Add(this, &ATX::Menu::button1Click);

	button2 = new Gwen::Controls::Button(window);
	button2->SetIsToggle(true);
	button2->SetToggleState(al_get_display_flags(al_get_current_display())&ALLEGRO_FULLSCREEN_WINDOW);
	button2->SetText(L"Fullscreen");
	button2->SetSize(200, 60);
	button2->Dock(Gwen::Pos::Top);
	button2->onToggle.Add(this, &ATX::Menu::button2Click);

	button3 = new Gwen::Controls::Button(window);
	button3->SetText(L"Scenario Select");
	button3->SetSize(200, 60);
	button3->Dock(Gwen::Pos::Top);
	button3->onPress.Add(this, &ATX::Menu::button3Click);

	button4 = new Gwen::Controls::Button(window);
	button4->SetText(L"Quit");
	button4->SetSize(200, 60);
	button4->Dock(Gwen::Pos::Top);
	button4->onPress.Add(this, &ATX::Menu::button4Click);
}

void ATX::Menu::button3Click()
{
	//ATX::Main::getInstance()->cleanup();
	States::Manager::getInstance()->changeState(Scenario::Menu::getInstance());
	//States::Manager::getInstance()->changeState(ATX::Main::getInstance());
}