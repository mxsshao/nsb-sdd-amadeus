#include "ATX.h"

ATX ATX::mATX;

void ATX::initialize(int displayW, int displayH)
{
	screen = al_create_bitmap(displayW, displayH);

	float time = al_current_time();

	offsetWidth = displayW/2; offsetHeight = displayH/2;

	airline.name = "";
	airline.code = "";
	airline.logo = NULL;

	path.name = "";
	/*path.nPoints.push_back(Coord(100,100,0));
	path.nPoints.push_back(Coord(200,200,0));
	path.nPoints.push_back(Coord(400,400,0));
	path.nPoints.push_back(Coord(600,300,0));
	path.nPoints.push_back(Coord(900,400,0));
	path.nPoints.push_back(Coord(700,650,0));
	path.nPoints.push_back(Coord(200,400,0));*/

	test.type = 0;
	test.company = airline;
	test.flightNo = "";
	test.isDeparture = true;
	test.destination = "";
	test.location.x = 0;
	test.location.y = 0;
	test.location.z = 0;
	test.speed = 0.4f;
	test.currentHeading = 120.0f;
	test.finalHeading = 120.0f;
	test.direction = NONE;
	test.state = 15.0f;

	bitmap = al_load_bitmap("Resources/aircraft.png");
	offset = 0.0f;
	bar = al_load_bitmap("Resources/bar.png");
	bg = al_load_bitmap("Resources/bg.png");

	std::list<int> groundPath;

	temp[0] = GroundCoord(100,100,0);
	temp[1] = GroundCoord(200,200,0);
	temp[2] = GroundCoord(300,300,0);
	temp[3] = GroundCoord(1000,600,0);
	temp[4] = GroundCoord(1000,300,0);

	temp[0].nConnected.push_back(1);
	//temp[0].nConnected.push_back(4);
	temp[1].nConnected.push_back(0);
	//temp[1].nConnected.push_back(4);
	temp[1].nConnected.push_back(2);
	temp[2].nConnected.push_back(1);
	temp[2].nConnected.push_back(3);
	//temp[2].nConnected.push_back(4);
	temp[3].nConnected.push_back(2);
	temp[3].nConnected.push_back(4);
	//temp[4].nConnected.push_back(0);
	//temp[4].nConnected.push_back(1);
	//temp[4].nConnected.push_back(2);
	temp[4].nConnected.push_back(3);

	groundPath.push_back(0);
	std::list<int>::iterator iter;
	std::list<int> final;
	std::list<int>::iterator duplicate;

	final.push_front(4);
	final.push_front(1);
	final.push_front(3);

	int value = 0;
	double min = 0.0f;
	double heading = 0;
	double theta = 0;

	while (!final.empty())
	{
		value = 0;
		min = 360.0f;
		heading = calculateHeading(temp[groundPath.back()].location, temp[final.back()].location);
		theta = 0;
		
		for (iter = temp[groundPath.back()].nConnected.begin(); iter != temp[groundPath.back()].nConnected.end(); iter++)
		{
			theta = abs(heading - calculateHeading(temp[groundPath.back()].location, temp[(*iter)].location));
			if (theta >= 180) {theta = 360 - theta;};
			if (theta <= min)
			{
				min = theta;
				value = (*iter);
			}
		}

		duplicate = std::find(groundPath.begin(), groundPath.end(), value);
		if (duplicate != groundPath.end())
		{
			groundPath.erase(duplicate, groundPath.end());
		}
		else
		{
			groundPath.push_back(value);
		}

		if (groundPath.back() == final.back())
		{
			final.pop_back();
		}
		
	}

	for (iter = groundPath.begin(); iter != groundPath.end(); iter++)
	{
		path.nPoints.push_back(temp[(*iter)].location);
	}

	test.path = path;

	//CAMERA
	camera.x = 0;
	camera.y = 0;




	std::cout << al_current_time() - time << std::endl;

}

void ATX::handleEvents(ALLEGRO_EVENT &ev)
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
	else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
	{
		if (ev.mouse.dz != 0)
		{
			camera.z -= ev.mouse.dz / 5.0f;

			if (camera.z < 0)
			{
				camera.z = 0;
			}
		}
	}
}

void ATX::update()
{
	offset += 0.8f;
	if (offset >= 64.0f)
	{
		offset = 0.0f;
	}

	if (!test.path.nPoints.empty())
	{
		test.finalHeading = calculateHeading(test.location, test.path.nPoints.front());

		if (abs(test.location.x - test.path.nPoints.front().x) <= test.speed)
		{
			if (abs(test.location.y - test.path.nPoints.front().y) <= test.speed)
			{
				test.location.x = test.path.nPoints.front().x;
				test.location.y = test.path.nPoints.front().y;

				test.path.nPoints.pop_front();
			}
		}
	}

	//AIRCRAFT UPDATE
	if (test.currentHeading != test.finalHeading)
	{
		double dif = test.currentHeading - test.finalHeading;
		if (dif > 0) {test.direction = (dif < 180) ? LEFT:RIGHT;}
		else {test.direction = (dif < -180) ? LEFT:RIGHT;}
		if (test.direction == LEFT)
		{
			test.currentHeading = fmod(test.currentHeading-0.2f, 360);
			if (test.currentHeading < 0)
			{
				test.currentHeading += 360;
			}
			if ((abs(test.currentHeading - test.finalHeading) < 2.6f) && (test.state < 14))
			{
				test.state ++;
			}
			else if (test.state > 0)
			{
				test.state --;
			}
		}
		else if (test.direction == RIGHT)
		{
			test.currentHeading = fmod(test.currentHeading+0.2f, 360);
			if (test.currentHeading < 0)
			{
				test.currentHeading += 360;
			}
			if ((abs(test.currentHeading - test.finalHeading) < 2.6f) && (test.state > 16))
			{
				test.state --;
			}
			else if (test.state < 30)
			{
				test.state ++;
			}
		}
	}

	if (abs(test.currentHeading - test.finalHeading) < 0.2f)
	{
		test.direction = NONE;
		test.currentHeading = test.finalHeading;
		test.state = 15;
	}

	test.location.x += test.speed * sin(test.currentHeading / 180.0f * ALLEGRO_PI);
	test.location.y -= test.speed * cos(test.currentHeading / 180.0f * ALLEGRO_PI);



	//CAMERA
	if (keys[LSHIFT])
	{
		if ((keys[RIGHT] && keys[LEFT]) || (!keys[RIGHT] && !keys[LEFT]))
		{
		}
		else if (keys[RIGHT])
		{
			camera.x += 30;
		}
		else if (keys[LEFT])
		{
			camera.x -= 30;
		}

		if ((keys[UP] && keys[DOWN]) || (!keys[UP] && !keys[DOWN]))
		{
		}
		else if (keys[UP])
		{
			camera.y -= 30;
		}
		else if (keys[DOWN])
		{
			camera.y += 30;
		}
	}
	else
	{
		if ((keys[RIGHT] && keys[LEFT]) || (!keys[RIGHT] && !keys[LEFT]))
		{
		}
		else if (keys[RIGHT])
		{
			camera.x += 10;
		}
		else if (keys[LEFT])
		{
			camera.x -= 10;
		}

		if ((keys[UP] && keys[DOWN]) || (!keys[UP] && !keys[DOWN]))
		{
		}
		else if (keys[UP])
		{
			camera.y -= 10;
		}
		else if (keys[DOWN])
		{
			camera.y += 10;
		}
	}



	al_identity_transform(&transform);
	al_scale_transform(&transform, 1.0f/(camera.z+1), 1.0f/(camera.z+1));
	al_translate_transform(&transform, offsetWidth - test.location.x/(camera.z+1), offsetHeight - test.location.y/(camera.z+1));
}

void ATX::render()
{
	ALLEGRO_BITMAP* back = al_get_target_bitmap();
	al_set_target_bitmap(screen);
	al_clear_to_color(al_map_rgb(0,0,0));

	al_use_transform(&transform);

	al_draw_bitmap(bg,0,0,0);



	std::list<Coord>::iterator iter;
	
	al_hold_bitmap_drawing(true);
	for (iter = test.path.nPoints.begin(); iter != test.path.nPoints.end(); iter++)
	{
		if (iter == test.path.nPoints.begin())
		{
			//al_draw_line((*iter).x, (*iter).y, test.location.x, test.location.y, al_map_rgb_f(1,1,1), 2);
			al_draw_tinted_scaled_rotated_bitmap_region(bar, offset, 0, calculateHypotenuse((*iter), test.location), 16, al_map_rgb_f(1,1,1), 0, 8, (*iter).x, (*iter).y, 1.0, 1.0, (calculateHeading((*iter), test.location) - 90.0f) / 180.0f * ALLEGRO_PI, 0);
		}
		else
		{
			std::list<Coord>::iterator p = std::prev(iter);
			//al_draw_line((*iter).x, (*iter).y, (*p).x, (*p).y, al_map_rgb_f(1,1,1), 10);
			al_draw_tinted_scaled_rotated_bitmap_region(bar, offset, 0, calculateHypotenuse((*iter), (*p)), 16, al_map_rgb_f(1,1,1), 0, 8, (*iter).x, (*iter).y, 1.0, 1.0, (calculateHeading((*iter), (*p)) - 90.0f) / 180.0f * ALLEGRO_PI, 0);
		}
	}
	int i;
	for (i = 0; i != 5; i++)
	{
		al_draw_tinted_scaled_rotated_bitmap_region(bar, 64, 64, 64, 64, al_map_rgb_f(1,1,1), 32, 32, temp[i].location.x, temp[i].location.y, 1.0f, 1.0f, 0, 0);
		//al_draw_circle(temp[i].location.x, temp[i].location.y, 6, al_map_rgb_f(1,1,1), 12);
	}

	al_hold_bitmap_drawing(false);

	//AIRCRAFT RENDER
	int x = 0;
	int y = 0;

	if (test.state <= 14)
	{
		y = 1000;
		x = floor((14 - test.state) / 2) * 500;
	}
	else if (test.state >= 16)
	{
		y = 500;
		x = ceil((test.state - 16) / 2) * 500;
	}

	double Scale = 0.4f;
	al_draw_tinted_scaled_rotated_bitmap_region(bitmap, x, y, 500, 500, al_map_rgb_f(1,1,1), 250, 250, test.location.x, test.location.y, Scale, Scale, test.currentHeading/180.0f*ALLEGRO_PI, 0);

	al_set_target_bitmap(back);
	al_draw_bitmap(screen,0,0,0);

}