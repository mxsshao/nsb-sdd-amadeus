#include "ATX_menu.h"

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
	window->SetMinimumSize(Gwen::Point(200,200));
}