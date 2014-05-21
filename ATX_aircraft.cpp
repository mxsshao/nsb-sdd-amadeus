#include "ATX_aircraft.h"
#include "ATX.h"

ATX::Structs::GuiImage ATX::Aircraft::gBase;
ATX::Structs::GuiImage ATX::Aircraft::gSelectedBase;
ATX::Structs::GuiImage ATX::Aircraft::gBarGreen;
ATX::Structs::GuiImage ATX::Aircraft::gAirline;
ATX::Structs::GuiText ATX::Aircraft::gAircraft;
ATX::Structs::GuiText ATX::Aircraft::gFlight;
ATX::Structs::GuiText ATX::Aircraft::gDestination;
ALLEGRO_BITMAP* ATX::Aircraft::aircraftButton;
ALLEGRO_FONT* ATX::Aircraft::nFonts[2];
ATX::Structs::Waypoint ATX::Aircraft::nWaypoints[20];

ATX::Aircraft::Aircraft(Gwen::Controls::WindowControl* parent, float ex, float ey, float ez, int start, double eSpeed, double eHeading, int eDestination, std::string type)
{
	std::cout << "NEW AIRCRAFT	-x" << ex << " -y" << ey << " -z" << ez << " -s" << start << " -v" << eSpeed << " -h" << eHeading << " -d" << eDestination <<std::endl;

	plane = al_load_bitmap(std::string("Resources/").append(type).append(".png").c_str());
	state = 15.0f;

	speed = eSpeed;
	currentHeading = eHeading;
	finalHeading = eHeading;
	direction = NONE;

	isSelected = false;
	flipCount = 1.0f;

	previous = start;
	//nPoints.push_back(nWaypoints[start]);
	x = ex;
	y = ey;
	z = ez;

	std::list<int> destination;
	destination.push_front(eDestination);
	navigate(&destination);

	buttonImage = al_create_bitmap(gBase.sw, gBase.sh);

	button = new Gwen::Controls::Button(parent);
	//button->SetSize(button->GetActualParent()->GetBounds().w, button->GetActualParent()->GetBounds().w / gBase.sw * gBase.sh);
	//button->SetSize(508, 33);
	button->Dock(Gwen::Pos::Top);
	button->SetShouldDrawBackground(false);
	button->SetImage(buttonImage);
	button->SetPadding(Gwen::Padding());
	button->SetMargin(Gwen::Margin());
	button->onPress.Add(this, &ATX::Aircraft::select);

	offset = 0.0f;
	done = false;
}

void ATX::Aircraft::initialize()
{
	tinyxml2::XMLDocument document;
	document.LoadFile("derp.xml");

	tinyxml2::XMLElement* element = document.FirstChildElement("Source");

	aircraftButton = al_load_bitmap(std::string("Resources/").append(element->Attribute("src")).c_str());

	element = document.FirstChildElement("Font0");
	nFonts[0] = al_load_font(std::string("Resources/").append(element->Attribute("src")).c_str(), element->IntAttribute("point"), 0);
	element = document.FirstChildElement("Font1");
	nFonts[1] = al_load_font(std::string("Resources/").append(element->Attribute("src")).c_str(), element->IntAttribute("point"), 0);

	element = document.FirstChildElement("Base");
	gBase = Structs::GuiImage(element->FloatAttribute("sx"), element->FloatAttribute("sy"), element->FloatAttribute("sw"), element->FloatAttribute("sh"), element->FloatAttribute("dx"), element->FloatAttribute("dy"));
	
	element = document.FirstChildElement("SelectedBase");
	gSelectedBase = Structs::GuiImage(element->FloatAttribute("sx"), element->FloatAttribute("sy"), element->FloatAttribute("sw"), element->FloatAttribute("sh"), element->FloatAttribute("dx"), element->FloatAttribute("dy"));

	element = document.FirstChildElement("BarGreen");
	gBarGreen = Structs::GuiImage(element->FloatAttribute("sx"), element->FloatAttribute("sy"), element->FloatAttribute("sw"), element->FloatAttribute("sh"), element->FloatAttribute("dx"), element->FloatAttribute("dy"));

	element = document.FirstChildElement("Airline");
	gAirline = Structs::GuiImage(element->FloatAttribute("sx"), element->FloatAttribute("sy"), element->FloatAttribute("sw"), element->FloatAttribute("sh"), element->FloatAttribute("dx"), element->FloatAttribute("dy"));

	element = document.FirstChildElement("Aircraft");
	gAircraft = Structs::GuiText(element->FloatAttribute("dx"), element->FloatAttribute("dy"), element->IntAttribute("align"), element->IntAttribute("size"), element->IntAttribute("r"), element->IntAttribute("g"), element->IntAttribute("b"));

	element = document.FirstChildElement("Flight");
	gFlight = Structs::GuiText(element->FloatAttribute("dx"), element->FloatAttribute("dy"), element->IntAttribute("align"), element->IntAttribute("size"), element->IntAttribute("r"), element->IntAttribute("g"), element->IntAttribute("b"));

	element = document.FirstChildElement("Destination");
	gDestination = Structs::GuiText(element->FloatAttribute("dx"), element->FloatAttribute("dy"), element->IntAttribute("align"), element->IntAttribute("size"), element->IntAttribute("r"), element->IntAttribute("g"), element->IntAttribute("b"));

	document.Clear();

	nWaypoints[0] = Structs::Waypoint(100,100,0);
	nWaypoints[1] = Structs::Waypoint(200,200,0);
	nWaypoints[2] = Structs::Waypoint(300,300,0);
	nWaypoints[3] = Structs::Waypoint(1000,600,0);
	nWaypoints[4] = Structs::Waypoint(1000,300,0);

	nWaypoints[0].nConnected.push_back(1);
	//nWaypoints[0].nConnected.push_back(4);
	nWaypoints[1].nConnected.push_back(0);
	//nWaypoints[1].nConnected.push_back(4);
	nWaypoints[1].nConnected.push_back(2);
	nWaypoints[2].nConnected.push_back(1);
	nWaypoints[2].nConnected.push_back(3);
	nWaypoints[2].nConnected.push_back(4);
	nWaypoints[3].nConnected.push_back(2);
	nWaypoints[3].nConnected.push_back(4);
	//nWaypoints[4].nConnected.push_back(0);
	//nWaypoints[4].nConnected.push_back(1);
	nWaypoints[4].nConnected.push_back(2);
	nWaypoints[4].nConnected.push_back(3);
}

void ATX::Aircraft::cleanup()
{
	al_destroy_bitmap(aircraftButton);
	al_destroy_font(nFonts[0]);
	al_destroy_font(nFonts[1]);
}

void ATX::Aircraft::resize()
{
	button->SetSize(button->GetActualParent()->GetBounds().w, button->GetActualParent()->GetBounds().w / gBase.sw * gBase.sh);
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

void ATX::Aircraft::select()
{
	ATX::Main::getInstance()->resetSelected();
	ATX::Main::getInstance()->getCamera()->following = this;
	ATX::Main::getInstance()->getCamera()->z = z;
	isSelected = true;
}

void ATX::Aircraft::update()
{
	//FLIP DISPLAY
	if (isSelected)
	{
		if (flipCount > -1.0f)
		{
			flipCount -= 0.1f;
		}
	}
	else
	{
		if (flipCount < 1.0f)
		{
			flipCount += 0.1f;
		}
	}

	//BAR
	offset += 0.8f;
	if (offset >= 64.0f)
	{
		offset = 0.0f;
	}

	//CHECK POINTS
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

	al_hold_bitmap_drawing(true);

	if (flipCount >= 0.0f)
	{
		al_identity_transform(&transform);
		al_scale_transform(&transform, 1.0f, flipCount);
		al_translate_transform(&transform, 0.0f, 40 * (1-flipCount));
		al_use_transform(&transform);

		al_clear_to_color(al_map_rgba(0, 0, 0, 0));

		al_draw_bitmap_region(aircraftButton, gBase.sx, gBase.sy, gBase.sw, gBase.sh, gBase.dx, gBase.dy, 0);
		al_draw_bitmap_region(aircraftButton, gBarGreen.sx, gBarGreen.sy, gBarGreen.sw, gBarGreen.sh, gBarGreen.dx, gBarGreen.dy, 0);

		al_draw_text(nFonts[gAircraft.size], gAircraft.color, gAircraft.dx, gAircraft.dy, gAircraft.align, "Boeing 777-200 LR");
		al_draw_text(nFonts[gFlight.size], gFlight.color, gFlight.dx, gFlight.dy, gFlight.align, "DL 716");
	}
	else
	{
		ALLEGRO_TRANSFORM transform;
		al_identity_transform(&transform);
		al_scale_transform(&transform, 1.0f, -flipCount);
		al_translate_transform(&transform, 0.0f, 40 * (1+flipCount));
		al_use_transform(&transform);

		al_clear_to_color(al_map_rgba(0, 0, 0, 0));

		al_draw_bitmap_region(aircraftButton, gSelectedBase.sx, gSelectedBase.sy, gSelectedBase.sw, gSelectedBase.sh, gSelectedBase.dx, gSelectedBase.dy, 0);
	}

	al_hold_bitmap_drawing(false);

	al_set_target_bitmap(back);

	//AIRCRAFT

	int x = 0;
	int y = 0;
	float Scale = 0.8f;

	if (state <= 14)
	{
		y = 500;
		x = floor((14 - state) / 2) * 250;
	}
	else if (state >= 16)
	{
		y = 250;
		x = ceil((state - 16) / 2) * 250;
	}

	al_draw_tinted_scaled_rotated_bitmap_region(plane, x, y, 250, 250, al_map_rgb_f(1,1,1), 125, 125, Aircraft::x, Aircraft::y, Scale, Scale, currentHeading/180.0f*ALLEGRO_PI, 0);

}

void ATX::Aircraft::renderLines()
{
	if (isSelected)
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
	else
	{
		std::list<Structs::Waypoint>::iterator iter;
		for (iter = nPoints.begin(); iter != nPoints.end(); iter++)
		{
			if (iter == nPoints.begin())
			{
				//al_draw_line((*iter).x, (*iter).y, test.location.x, test.location.y, al_map_rgb_f(1,1,1), 2);
				al_draw_tinted_scaled_rotated_bitmap_region(ATX::Main::getInstance()->bar, offset, 48, calculateHypotenuse((*iter), Structs::Waypoint(x,y,z)), 16, al_map_rgb_f(1,1,1), 0, 8, (*iter).x, (*iter).y, 1.0, 1.0, (calculateHeading((*iter), Structs::Waypoint(x,y,z)) - 90.0f) / 180.0f * ALLEGRO_PI, 0);
			}
			else
			{
				std::list<Structs::Waypoint>::iterator p = std::prev(iter);
				//al_draw_line((*iter).x, (*iter).y, (*p).x, (*p).y, al_map_rgb_f(1,1,1), 10);
				al_draw_tinted_scaled_rotated_bitmap_region(ATX::Main::getInstance()->bar, offset, 48, calculateHypotenuse((*iter), (*p)), 16, al_map_rgb_f(1,1,1), 0, 8, (*iter).x, (*iter).y, 1.0, 1.0, (calculateHeading((*iter), (*p)) - 90.0f) / 180.0f * ALLEGRO_PI, 0);
			}
		}
	}
}

ATX::Aircraft::~Aircraft()
{
	delete(button);
	al_destroy_bitmap(plane);
	//al_destroy_bitmap(buttonImage);
}