#include "ATX.h"
#include "ATX_menu.h"

ATX::Main ATX::Main::mMain;

void ATX::Main::initialize(States::Manager* manager)
{
	canvas = manager->getCanvas();

	bar = al_load_bitmap("Resources/bar.png");
	bg = al_load_bitmap("Resources/bg.png");

	camera = Structs::Camera(offsetWidth,offsetHeight,0);

	window = new Gwen::Controls::WindowControl(canvas);
	window->MakeScroll();
	window->GetScroll()->SetScroll(false, true);
	window->GetScroll()->onResize.Add(this, &ATX::Main::windowResize);
	window->SetTitle(L"Flight Information");
	//window->MakeModal(false);
	//window->SetClosable(false);
	//window->DisableResizing();
	window->SetMinimumSize(Gwen::Point(200,200));
	window->onResize.Add(this, &ATX::Main::windowResize);

	testControl = new Gwen::Controls::WindowControl(canvas);
	testControl->SetTitle(L"Test Control");
	//window->MakeModal(false);
	testControl->SetClosable(false);
	//window->DisableResizing();
	testControl->DisableResizing();

	buttonMenu = new Gwen::Controls::Button(testControl);
	buttonMenu->SetText(L"MENU");
	buttonMenu->SetSize(300, 30);
	buttonMenu->Dock(Gwen::Pos::Top);
	buttonMenu->onPress.Add(this, &ATX::Main::buttonMenuClick);

	button1 = new Gwen::Controls::Button(testControl);
	button1->SetText(L"Add Flight");
	button1->SetSize(200, 30);
	button1->Dock(Gwen::Pos::Top);
	button1->onPress.Add(this, &ATX::Main::button1Click);

	button2 = new Gwen::Controls::Button(testControl);
	button2->SetText(L"Display Symbols");
	button2->SetIsToggle(true);
	//button2->SetToggleState(true);
	//button2->SetBounds(20, 20, 200, 50);
	button2->SetSize(200, 30);
	button2->Dock(Gwen::Pos::Top);
	//button2->SetShouldDrawBackground(false);

	button3 = new Gwen::Controls::Button(testControl);
	button3->SetText(L"Show Flight Window");
	button3->SetSize(300, 30);
	button3->Dock(Gwen::Pos::Top);
	button3->onPress.Add(this, &ATX::Main::button3Click);

	button4 = new Gwen::Controls::Button(testControl);
	button4->SetText(L"Show Radar Window");
	button4->SetSize(300, 30);
	button4->Dock(Gwen::Pos::Top);
	button4->onPress.Add(this, &ATX::Main::button4Click);

	combo = new Gwen::Controls::ComboBox(testControl);
	combo->SetSize(300, 30);
	combo->Dock(Gwen::Pos::Top);
	combo->SetMargin(0);
	combo->SetAlignment(Gwen::Pos::Center);
	combo->AddItem(L"Default", "one");
	combo->AddItem(L"Simple", "one");

	radarWindow = new Gwen::Controls::WindowControl(canvas);
	radarWindow->SetTitle(L"Radar");
	//radarWindow->SetClosable(false);
	radarWindow->DisableResizing();


	radarImage = al_load_bitmap("Resources/radar.png");
	radarRender = al_create_bitmap(256,256);

	radarPanel = new Gwen::Controls::ImagePanel(radarWindow);
	radarPanel->SetImage(radarRender);

	first = true;
	resize(manager);

	Aircraft::initialize();

	//nAircraft.push_back(new Aircraft(window, 1000, 1000, 0, 3, 0.5f, 90.0f, 2, "MU330"));
	nAircraft.push_back(new Aircraft(window, 0, 0, 0, 0, 0.4f, 120.0f, 4, "TG3802"));

	nAircraft.front()->select();
}

void ATX::Main::resize(States::Manager* manager)
{
	int originalW = displayW;
	int originalH = displayH;


	displayW = al_get_display_width(manager->getDisplay());
	displayH = al_get_display_height(manager->getDisplay());
	offsetWidth = displayW/2; offsetHeight = displayH/2;

	if (first)
	{
		screen = al_create_bitmap(displayW, displayH);
		
		window->SetSize(400, 300);
		window->SetPos(displayW - 400, displayH-300);

		testControl->SetSize(200, 300);
		testControl->SetPos(0, displayH-300);

		radarWindow->SetSize(256, 256 + 28);
		radarWindow->SetPos(0,0);
		radarPanel->SetSize(radarWindow->GetBounds().w - 8, radarWindow->GetBounds().w - 8);
		radarPanel->Dock(Gwen::Pos::Top);
		first = false;
	}
	else
	{
		al_destroy_bitmap(screen);
		screen = al_create_bitmap(displayW, displayH);

		float a = window->GetBounds().x;
		float b = window->GetBounds().y;

		window->SetSize(400, 300);
		window->SetPos(displayW - a + originalW, displayH - b + originalH);

		a = testControl->GetBounds().y;

		testControl->SetSize(200, 300);
		testControl->SetPos(testControl->GetPos().x, displayH - a + originalH);

		radarWindow->SetSize(256, 256 + 28);
		//radarWindow->SetPos(radarWindow->GetPos().x, displayH + radarWindow->GetPos().y - originalH);
		radarPanel->SetSize(radarWindow->GetBounds().w - 8, radarWindow->GetBounds().w - 8);
		radarPanel->Dock(Gwen::Pos::Top);
	}

}

void ATX::Main::buttonMenuClick()
{
	States::Manager::getInstance()->pauseState(ATX::Menu::getInstance());
}

void ATX::Main::windowResize()
{
	if (!nAircraft.empty())
	{
		for (iter = nAircraft.begin(); iter != nAircraft.end(); iter++)
		{
			(*iter)->resize();
		}
	}
}

void ATX::Main::resetSelected()
{
	if (!nAircraft.empty())
	{
		for (iter = nAircraft.begin(); iter != nAircraft.end(); iter++)
		{
			if ((*iter)->getSelected())
			{
				(*iter)->setSelected(false);
			}
		}
	}
	camera.following = NULL;
}

void ATX::Main::pause()
{
}

void ATX::Main::resume()
{
}

void ATX::Main::breakaway()
{
	if (camera.following)
	{
		camera.x = camera.following->getX();
		camera.y = camera.following->getY();
		camera.following = NULL;
	}
}

void ATX::Main::handleEvents(ALLEGRO_EVENT &ev)
{
	if (ev.type == ALLEGRO_EVENT_TIMER)
	{
		update();
	}
	else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		switch (ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_RIGHT:
			keys[RIGHT] = true;
			break;
		case ALLEGRO_KEY_LEFT:
			keys[LEFT] = true;
			break;
		case ALLEGRO_KEY_UP:
			keys[UP] = true;
			break;
		case ALLEGRO_KEY_DOWN:
			keys[DOWN] = true;
			break;
		case ALLEGRO_KEY_LSHIFT:
			keys[LSHIFT] = true;
			break;
		}
	}
	else if (ev.type == ALLEGRO_EVENT_KEY_UP)
	{
		switch (ev.keyboard.keycode)
		{
		case ALLEGRO_KEY_RIGHT:
			keys[RIGHT] = false;
			break;
		case ALLEGRO_KEY_LEFT:
			keys[LEFT] = false;
			break;
		case ALLEGRO_KEY_UP:
			keys[UP] = false;
			break;
		case ALLEGRO_KEY_DOWN:
			keys[DOWN] = false;
			break;
		case ALLEGRO_KEY_LSHIFT:
			keys[LSHIFT] = false;
			break;
		}
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
	{
		if (ev.mouse.button == 2 && canvas->IsHovered())
		{
			rClick = true;
			//al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_MOVE);
			canvas->SetCursor(Gwen::CursorType::SizeAll);
		}
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
	{
		if (ev.mouse.button == 2)
		{
			rClick = false;
			//al_set_system_mouse_cursor(al_get_current_display(), ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT);
			canvas->SetCursor(Gwen::CursorType::Normal);
		}
	}
	else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		if (ev.mouse.dz != 0 && canvas->IsHovered())
		{
			camera.z -= ev.mouse.dz / 3.0f;

			if (camera.z < 0)
			{
				camera.z = 0;
			}
		}
		if (rClick && canvas->IsHovered())
		{
			camera.x -= ev.mouse.dx*(camera.z+1);
			camera.y -= ev.mouse.dy*(camera.z+1);
			breakaway();
		}
	}
}

void ATX::Main::update()
{
	//AIRCRAFT
	if (!nAircraft.empty())
	{
		for (iter = nAircraft.begin(); iter != nAircraft.end();)
		{
			(*iter)->update();

			if ((*iter)->done)
			{
				bool shouldSelect = (*iter)->getSelected();
				breakaway();
				delete(*iter);
				nAircraft.erase(iter++);
				if (iter != nAircraft.end() && shouldSelect)
				{
					(*iter)->select();
				}
			}
			else
			{
				iter++;
			}
		}
	}

	//CAMERA
	if (keys[LSHIFT])
	{
		if ((keys[RIGHT] && keys[LEFT]) || (!keys[RIGHT] && !keys[LEFT]))
		{
		}
		else if (keys[RIGHT])
		{
			camera.x += 30.0f*(camera.z+1);
			breakaway();
		}
		else if (keys[LEFT])
		{
			camera.x -= 30.0f*(camera.z+1);
			breakaway();
		}

		if ((keys[UP] && keys[DOWN]) || (!keys[UP] && !keys[DOWN]))
		{
		}
		else if (keys[UP])
		{
			camera.y -= 30.0f*(camera.z+1);
			breakaway();
		}
		else if (keys[DOWN])
		{
			camera.y += 30.0f*(camera.z+1);
			breakaway();
		}
	}
	else
	{
		if ((keys[RIGHT] && keys[LEFT]) || (!keys[RIGHT] && !keys[LEFT]))
		{
		}
		else if (keys[RIGHT])
		{
			camera.x += 10.0f*(camera.z+1);
			breakaway();
		}
		else if (keys[LEFT])
		{
			camera.x -= 10.0f*(camera.z+1);
			breakaway();
		}

		if ((keys[UP] && keys[DOWN]) || (!keys[UP] && !keys[DOWN]))
		{
		}
		else if (keys[UP])
		{
			camera.y -= 10.0f*(camera.z+1);
			breakaway();
		}
		else if (keys[DOWN])
		{
			camera.y += 10.0f*(camera.z+1);
			breakaway();
		}
	}

	if (camera.following)
	{
		al_identity_transform(&transform);
		al_scale_transform(&transform, 1.0f/(camera.z+1), 1.0f/(camera.z+1));
		al_translate_transform(&transform, offsetWidth - camera.following->getX()/(camera.z+1), offsetHeight - camera.following->getY()/(camera.z+1));
	}
	else
	{
		al_identity_transform(&transform);
		al_scale_transform(&transform, 1.0f/(camera.z+1), 1.0f/(camera.z+1));
		al_translate_transform(&transform, offsetWidth - camera.x/(camera.z+1), offsetHeight - camera.y/(camera.z+1));
	}
}

void ATX::Main::render()
{
	ALLEGRO_BITMAP* back = al_get_target_bitmap();
	al_set_target_bitmap(screen);
	al_clear_to_color(al_map_rgb(0,0,0));

	al_use_transform(&transform);

	al_draw_bitmap(bg,0,0,0);

	//MAP SYMBOLS
	if (button2->GetToggleState())
	{
		al_hold_bitmap_drawing(true);
	
		al_draw_tinted_scaled_rotated_bitmap_region(bar, 128, 64, 128, 128, al_map_rgb_f(1,1,1), 64, 128, 400, 200, 1.0f, 1.0f, 0, 0);
		al_draw_tinted_scaled_rotated_bitmap_region(bar, 256, 64, 128, 128, al_map_rgb_f(1,1,1), 64, 128, 500, 200, 1.0f, 1.0f, 0, 0);
		al_draw_tinted_scaled_rotated_bitmap_region(bar, 128, 192, 128, 128, al_map_rgb_f(1,1,1), 64, 128, 400, 328, 1.0f, 1.0f, 0, 0);
		al_draw_tinted_scaled_rotated_bitmap_region(bar, 256, 192, 128, 128, al_map_rgb_f(1,1,1), 64, 128, 500, 328, 1.0f, 1.0f, 0, 0);

		al_draw_tinted_scaled_rotated_bitmap_region(bar, 0, 832, 128, 192, al_map_rgb_f(1,1,1), 64, 96, 1000, 328, 1.0f, 1.0f, 0, 0);
		al_draw_tinted_scaled_rotated_bitmap_region(bar, 384, 384, 640, 640, al_map_rgb_f(1,1,1), 320, 320, 1000, 328, 1.0f, 1.0f, 0, 0);

		al_hold_bitmap_drawing(false);
	}

	al_hold_bitmap_drawing(true);
	//AIRCRAFT
	if (!nAircraft.empty())
	{
		for (iter = nAircraft.begin(); iter != nAircraft.end(); iter++)
		{
			(*iter)->renderLines();
		}
		al_hold_bitmap_drawing(false);
	}

	al_hold_bitmap_drawing(true);
	int i;
	for (i = 0; i != 5; i++)
	{
		//al_draw_tinted_scaled_rotated_bitmap_region(bar, 0, 320, 64, 64, al_map_rgb_f(1,1,1), 32, 32, Aircraft::getWaypoint(i)->x,  Aircraft::getWaypoint(i)->y, camera.z+1, camera.z+1, 0, 0);
		al_draw_tinted_scaled_rotated_bitmap_region(bar, 0, 320, 64, 64, al_map_rgb_f(1,1,1), 32, 32, Aircraft::getWaypoint(i)->x,  Aircraft::getWaypoint(i)->y, 1.0f, 1.0f, 0, 0);
		//al_draw_circle(temp[i].location.x, temp[i].location.y, 6, al_map_rgb_f(1,1,1), 12);
	}
	al_hold_bitmap_drawing(false);

	//AIRCRAFT
	if (!nAircraft.empty())
	{
		for (iter = nAircraft.begin(); iter != nAircraft.end(); iter++)
		{
			(*iter)->render();
		}
	}

	//RADAR
	al_set_target_bitmap(radarRender);
	al_hold_bitmap_drawing(true);
	al_clear_to_color(al_map_rgba(0,0,0,0));
	al_draw_bitmap_region(radarImage, 0, 0, 256, 256, 0, 0, 0);

	if (!nAircraft.empty())
	{
		for (iter = nAircraft.begin(); iter != nAircraft.end(); iter++)
		{
			if ((*iter)->getSelected())
			{
				al_draw_tinted_scaled_rotated_bitmap_region(radarImage, 4, 256, 4, 4, al_map_rgb_f(1,1,1), 2, 2, (*iter)->getX() / 10.0f,  (*iter)->getY() / 10.0f, 1.0f, 1.0f, 0, 0);
			}
			else
			{
				al_draw_tinted_scaled_rotated_bitmap_region(radarImage, 0, 256, 4, 4, al_map_rgb_f(1,1,1), 2, 2, (*iter)->getX() / 10.0f,  (*iter)->getY() / 10.0f, 1.0f, 1.0f, 0, 0);
			}
		}
	}

	al_draw_bitmap_region(radarImage, 256, 0, 256, 256, 0, 0, 0);
	al_hold_bitmap_drawing(false);

	al_set_target_bitmap(back);
	al_draw_bitmap(screen, 0, 0, 0);
}

void ATX::Main::cleanup()
{
	window->DelayedDelete();
	testControl->DelayedDelete();
	radarWindow->DelayedDelete();

	al_destroy_bitmap(screen);
	al_destroy_bitmap(radarImage);
	al_destroy_bitmap(bg);

	if (!nAircraft.empty())
	{
		for (iter = nAircraft.begin(); iter != nAircraft.end();)
		{
			delete(*iter);
			nAircraft.erase(iter++);
		}
	}
	ATX::Aircraft::cleanup();

	al_destroy_bitmap(bar);
}