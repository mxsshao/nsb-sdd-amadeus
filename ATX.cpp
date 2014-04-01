#include "ATX.h"

ATX::Main ATX::Main::mMain;

void ATX::Main::initialize(int displayW, int displayH, Gwen::Controls::Base* pCanvas)
{
	canvas = pCanvas;

	screen = al_create_bitmap(displayW, displayH);

	offsetWidth = displayW/2; offsetHeight = displayH/2;

	bar = al_load_bitmap("Resources/bar.png");
	bg = al_load_bitmap("Resources/bg.png");

	camera = Structs::Waypoint(offsetWidth,offsetHeight,0);

	window = new Gwen::Controls::WindowControl(canvas);
	window->SetTitle(L"Flights");
	//window->MakeModal(false);
	window->SetClosable(false);
	//window->DisableResizing();
	window->SetSize(520, 500);
	window->SetMinimumSize(Gwen::Point(100,100));
	window->SetPadding(Gwen::Padding());
	window->Hide();

	Aircraft::nWaypoints[0] = Structs::Waypoint(100,100,0);
	Aircraft::nWaypoints[1] = Structs::Waypoint(200,200,0);
	Aircraft::nWaypoints[2] = Structs::Waypoint(300,300,0);
	Aircraft::nWaypoints[3] = Structs::Waypoint(1000,600,0);
	Aircraft::nWaypoints[4] = Structs::Waypoint(1000,300,0);

	Aircraft::nWaypoints[0].nConnected.push_back(1);
	//Aircraft::nWaypoints[0].nConnected.push_back(4);
	Aircraft::nWaypoints[1].nConnected.push_back(0);
	//Aircraft::nWaypoints[1].nConnected.push_back(4);
	Aircraft::nWaypoints[1].nConnected.push_back(2);
	Aircraft::nWaypoints[2].nConnected.push_back(1);
	Aircraft::nWaypoints[2].nConnected.push_back(3);
	Aircraft::nWaypoints[2].nConnected.push_back(4);
	Aircraft::nWaypoints[3].nConnected.push_back(2);
	Aircraft::nWaypoints[3].nConnected.push_back(4);
	//Aircraft::nWaypoints[4].nConnected.push_back(0);
	//Aircraft::nWaypoints[4].nConnected.push_back(1);
	Aircraft::nWaypoints[4].nConnected.push_back(2);
	Aircraft::nWaypoints[4].nConnected.push_back(3);

	Aircraft::aircraftButton = al_load_bitmap("Resources/FIDS.png");
	Aircraft::nFonts[0] = al_load_font("Resources/OpenSans.ttf", 12, 0);
	Aircraft::nFonts[1] = al_load_font("Resources/OpenSans.ttf", 16, 0);

	nAircraft.push_back(new Aircraft(window, 1000, 1000, 1000, 3, 0.5f, 10.0f, 2, "MU330"));
	nAircraft.push_back(new Aircraft(window, 0, 0, 0, 0, 0.4f, 120.0f, 4, "TG380"));
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
		if (ev.mouse.button == 2)
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
		if (ev.mouse.dz != 0)
		{
			camera.z -= ev.mouse.dz / 3.0f;

			if (camera.z < 0)
			{
				camera.z = 0;
			}
		}
		if (rClick)
		{
			camera.x -= ev.mouse.dx*(camera.z+1);
			camera.y -= ev.mouse.dy*(camera.z+1);
		}
	}
}

void ATX::Main::update()
{
	//AIRCRAFT
	for (iter = nAircraft.begin(); iter != nAircraft.end();)
	{
		(*iter)->update();

		if ((*iter)->done)
		{
			nAircraft.erase(iter++);
		}
		else
		{
			iter++;
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
		}
		else if (keys[LEFT])
		{
			camera.x -= 30.0f*(camera.z+1);
		}

		if ((keys[UP] && keys[DOWN]) || (!keys[UP] && !keys[DOWN]))
		{
		}
		else if (keys[UP])
		{
			camera.y -= 30.0f*(camera.z+1);
		}
		else if (keys[DOWN])
		{
			camera.y += 30.0f*(camera.z+1);
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
		}
		else if (keys[LEFT])
		{
			camera.x -= 10.0f*(camera.z+1);
		}

		if ((keys[UP] && keys[DOWN]) || (!keys[UP] && !keys[DOWN]))
		{
		}
		else if (keys[UP])
		{
			camera.y -= 10.0f*(camera.z+1);
		}
		else if (keys[DOWN])
		{
			camera.y += 10.0f*(camera.z+1);
		}
	}

	al_identity_transform(&transform);
	al_scale_transform(&transform, 1.0f/(camera.z+1), 1.0f/(camera.z+1));
	al_translate_transform(&transform, offsetWidth - camera.x/(camera.z+1), offsetHeight - camera.y/(camera.z+1));
}

void ATX::Main::render()
{
	ALLEGRO_BITMAP* back = al_get_target_bitmap();
	al_set_target_bitmap(screen);
	al_clear_to_color(al_map_rgb(0,0,0));

	al_use_transform(&transform);

	al_draw_bitmap(bg,0,0,0);

	al_hold_bitmap_drawing(true);
	
	al_draw_tinted_scaled_rotated_bitmap_region(bar, 128, 64, 128, 128, al_map_rgb_f(1,1,1), 64, 128, 400, 200, 1.0f, 1.0f, 0, 0);
	al_draw_tinted_scaled_rotated_bitmap_region(bar, 256, 64, 128, 128, al_map_rgb_f(1,1,1), 64, 128, 500, 200, 1.0f, 1.0f, 0, 0);
	al_draw_tinted_scaled_rotated_bitmap_region(bar, 128, 192, 128, 128, al_map_rgb_f(1,1,1), 64, 128, 400, 328, 1.0f, 1.0f, 0, 0);
	al_draw_tinted_scaled_rotated_bitmap_region(bar, 256, 192, 128, 128, al_map_rgb_f(1,1,1), 64, 128, 500, 328, 1.0f, 1.0f, 0, 0);

	al_draw_tinted_scaled_rotated_bitmap_region(bar, 0, 832, 128, 192, al_map_rgb_f(1,1,1), 64, 96, 1000, 328, 1.0f, 1.0f, 0, 0);
	al_draw_tinted_scaled_rotated_bitmap_region(bar, 384, 384, 640, 640, al_map_rgb_f(1,1,1), 320, 320, 1000, 328, 1.0f, 1.0f, 0, 0);

	al_hold_bitmap_drawing(false);


	//AIRCRAFT
	for (iter = nAircraft.begin(); iter != nAircraft.end(); iter++)
	{
		(*iter)->renderLines();
	}

	al_hold_bitmap_drawing(true);

	int i;
	for (i = 0; i != 5; i++)
	{
		al_draw_tinted_scaled_rotated_bitmap_region(bar, 0, 320, 64, 64, al_map_rgb_f(1,1,1), 32, 32, Aircraft::nWaypoints[i].x,  Aircraft::nWaypoints[i].y, 1.0f, 1.0f, 0, 0);
		//al_draw_circle(temp[i].location.x, temp[i].location.y, 6, al_map_rgb_f(1,1,1), 12);
	}

	al_hold_bitmap_drawing(false);

	//AIRCRAFT
	for (iter = nAircraft.begin(); iter != nAircraft.end(); iter++)
	{
		(*iter)->render();
	}

	al_set_target_bitmap(back);
	al_draw_bitmap(screen, 0, 0, 0);

	canvas->GetCanvas()->RenderCanvas();
}