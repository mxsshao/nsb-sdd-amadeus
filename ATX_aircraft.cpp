#include "ATX_aircraft.h"
#include "ATX.h"

ALLEGRO_BITMAP* ATX::Aircraft::aircraftButton;
ALLEGRO_FONT* ATX::Aircraft::nFonts[2];
ATX::Structs::Waypoint ATX::Aircraft::nWaypoints[20];

ATX::Aircraft::Aircraft(Gwen::Controls::Base* parent, float ex, float ey, float ez, int start, double eSpeed, double eHeading, int eDestination, std::string type)
{
	tinyxml2::XMLDocument document;
	document.LoadFile("derp.xml");
	tinyxml2::XMLElement* element = document.FirstChildElement("Base");
	base = Structs::GuiImage(element->FloatAttribute("sx"), element->FloatAttribute("sy"), element->FloatAttribute("sw"), element->FloatAttribute("sh"), element->FloatAttribute("dx"), element->FloatAttribute("dy"));
	
	element = document.FirstChildElement("BarGreen");
	barGreen = Structs::GuiImage(element->FloatAttribute("sx"), element->FloatAttribute("sy"), element->FloatAttribute("sw"), element->FloatAttribute("sh"), element->FloatAttribute("dx"), element->FloatAttribute("dy"));

	element = document.FirstChildElement("Aircraft");
	aircraft = Structs::GuiText(element->FloatAttribute("dx"), element->FloatAttribute("dy"), element->IntAttribute("align"), element->IntAttribute("size"), element->IntAttribute("r"), element->IntAttribute("g"), element->IntAttribute("b"));

	element = document.FirstChildElement("Flight");
	flight = Structs::GuiText(element->FloatAttribute("dx"), element->FloatAttribute("dy"), element->IntAttribute("align"), element->IntAttribute("size"), element->IntAttribute("r"), element->IntAttribute("g"), element->IntAttribute("b"));

	document.Clear();

	plane = al_load_bitmap(std::string("Resources/").append(type).append(".png").c_str());
	state = 0;

	speed = eSpeed;
	currentHeading = eHeading;
	finalHeading = eHeading;
	direction = NONE;

	previous = start;
	//nPoints.push_back(nWaypoints[start]);
	x = ex;
	y = ey;
	z = ez;

	std::list<int> destination;
	destination.push_front(eDestination);
	navigate(&destination);

	buttonImage = al_create_bitmap(508, 66);

	button = new Gwen::Controls::Button(parent);
	button->SetSize(508, 66);
	button->Dock(Gwen::Pos::Top);
	button->SetImage(buttonImage);
	button->SetPadding(Gwen::Padding());
	button->SetMargin(Gwen::Margin());
	button->onPress.Add(this, &ATX::Aircraft::buttonClick);

	offset = 0.0f;
	done = false;
}

void ATX::Aircraft::navigate(std::list<int>* destination)
{
	std::list<int> groundPath;
	groundPath.push_back(previous);
	std::list<int>::iterator iter;
	std::list<int>::iterator duplicate;

	int value = 0;
	double min = 0.0f;
	double heading = 0;
	double theta = 0;

	while (!destination->empty())
	{
		value = 0;
		min = 360.0f;
		heading = calculateHeading(nWaypoints[groundPath.back()], nWaypoints[destination->back()]);
		theta = 0;
		
		for (iter = nWaypoints[groundPath.back()].nConnected.begin(); iter != nWaypoints[groundPath.back()].nConnected.end(); iter++)
		{
			theta = abs(heading - calculateHeading(nWaypoints[groundPath.back()], nWaypoints[(*iter)]));
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

		if (groundPath.back() == destination->back())
		{
			destination->pop_back();
		}
		
	}

	for (iter = groundPath.begin(); iter != groundPath.end(); iter++)
	{
		nPoints.push_back(nWaypoints[(*iter)]);
	}

}

void ATX::Aircraft::buttonClick()
{
	std::cout << "Click";
}

void ATX::Aircraft::update()
{
	offset += 0.8f;
	if (offset >= 64.0f)
	{
		offset = 0.0f;
	}

	if (!nPoints.empty())
	{
		finalHeading = calculateHeading(Structs::Waypoint(x,y,z), nPoints.front());

		if (abs(x - nPoints.front().x) <= speed)
		{
			if (abs(y - nPoints.front().y) <= speed)
			{
				x = nPoints.front().x;
				y = nPoints.front().y;

				nPoints.pop_front();
			}
		}
	}
	else
	{
		done = true;
	}

	//AIRCRAFT UPDATE
	if (currentHeading != finalHeading)
	{
		double dif = currentHeading - finalHeading;
		if (dif > 0) {direction = (dif < 180) ? LEFT:RIGHT;}
		else {direction = (dif < -180) ? LEFT:RIGHT;}
		if (direction == LEFT)
		{
			currentHeading = fmod(currentHeading-0.2f, 360);
			if (currentHeading < 0)
			{
				currentHeading += 360;
			}
			if ((abs(currentHeading - finalHeading) < 2.6f) && (state < 14))
			{
				state ++;
			}
			else if (state > 0)
			{
				state --;
			}
		}
		else if (direction == RIGHT)
		{
			currentHeading = fmod(currentHeading+0.2f, 360);
			if (currentHeading < 0)
			{
				currentHeading += 360;
			}
			if ((abs(currentHeading - finalHeading) < 2.6f) && (state > 16))
			{
				state --;
			}
			else if (state < 30)
			{
				state ++;
			}
		}
	}

	if (abs(currentHeading - finalHeading) < 0.2f)
	{
		direction = NONE;
		currentHeading = finalHeading;
		state = 15;
	}

	x += speed * sin(currentHeading / 180.0f * ALLEGRO_PI);
	y -= speed * cos(currentHeading / 180.0f * ALLEGRO_PI);
}

void ATX::Aircraft::render()
{
	ALLEGRO_BITMAP* back = al_get_target_bitmap();

	al_set_target_bitmap(buttonImage);

	/*if (c >= 0.0f)
	{
		ALLEGRO_TRANSFORM transform;
		al_identity_transform(&transform);
		al_scale_transform(&transform, 1.0f, c);
		al_translate_transform(&transform, 0.0f, 40 * (1-c));
		al_use_transform(&transform);
*/
		al_clear_to_color(al_map_rgba(0, 0, 0, 0));

		al_draw_bitmap_region(aircraftButton, base.sx, base.sy, base.sw, base.sh, base.dx, base.dy, 0);
		al_draw_bitmap_region(aircraftButton, barGreen.sx, barGreen.sy, barGreen.sw, barGreen.sh, barGreen.dx, barGreen.dy, 0);

		al_draw_text(nFonts[aircraft.size], aircraft.color, aircraft.dx, aircraft.dy, aircraft.align, "Boeing 777-200 LR");
		al_draw_text(nFonts[flight.size], flight.color, flight.dx, flight.dy, flight.align, "DL 716");
	/*}
	else
	{
		ALLEGRO_TRANSFORM transform;
		al_identity_transform(&transform);
		al_scale_transform(&transform, 1.0f, -c);
		al_translate_transform(&transform, 0.0f, 40 * (1+c));
		al_use_transform(&transform);

		al_clear_to_color(al_map_rgba(0, 0, 0, 0));

		al_draw_bitmap_region(bitmap, base.sx, base.sy, base.sw, base.sh, base.dx, base.dy, 0);
		al_draw_bitmap_region(bitmap, barGreen.sx, barGreen.sy, barGreen.sw, barGreen.sh, barGreen.dx, barGreen.dy, 0);

		al_draw_text(fonts[aircraft.size], aircraft.color, aircraft.dx, aircraft.dy, aircraft.align, "Boeing 777-200 LR");
		al_draw_text(fonts[flight.size], flight.color, flight.dx, flight.dy, flight.align, "DL 716");
	}*/



	al_set_target_bitmap(back);

	//AIRCRAFT

	int x = 0;
	int y = 0;

	if (state <= 14)
	{
		y = 1000;
		x = floor((14 - state) / 2) * 500;
	}
	else if (state >= 16)
	{
		y = 500;
		x = ceil((state - 16) / 2) * 500;
	}

	double Scale = 0.4f;
	al_draw_tinted_scaled_rotated_bitmap_region(plane, x, y, 500, 500, al_map_rgb_f(1,1,1), 250, 250, Aircraft::x, Aircraft::y, Scale, Scale, currentHeading/180.0f*ALLEGRO_PI, 0);

}

void ATX::Aircraft::renderLines()
{
	std::list<Structs::Waypoint>::iterator iter;
	for (iter = nPoints.begin(); iter != nPoints.end(); iter++)
	{
		if (iter == nPoints.begin())
		{
			//al_draw_line((*iter).x, (*iter).y, test.location.x, test.location.y, al_map_rgb_f(1,1,1), 2);
			al_draw_tinted_scaled_rotated_bitmap_region(ATX::Main::getInstance()->bar, offset, 0, calculateHypotenuse((*iter), Structs::Waypoint(x,y,z)), 16, al_map_rgb_f(1,1,1), 0, 8, (*iter).x, (*iter).y, 1.0, 1.0, (calculateHeading((*iter), Structs::Waypoint(x,y,z)) - 90.0f) / 180.0f * ALLEGRO_PI, 0);
		}
		else
		{
			std::list<Structs::Waypoint>::iterator p = std::prev(iter);
			//al_draw_line((*iter).x, (*iter).y, (*p).x, (*p).y, al_map_rgb_f(1,1,1), 10);
			al_draw_tinted_scaled_rotated_bitmap_region(ATX::Main::getInstance()->bar, offset, 0, calculateHypotenuse((*iter), (*p)), 16, al_map_rgb_f(1,1,1), 0, 8, (*iter).x, (*iter).y, 1.0, 1.0, (calculateHeading((*iter), (*p)) - 90.0f) / 180.0f * ALLEGRO_PI, 0);
		}
	}
}

ATX::Aircraft::~Aircraft()
{
	delete(button);
	al_destroy_bitmap(plane);
	//al_destroy_bitmap(buttonImage);
}