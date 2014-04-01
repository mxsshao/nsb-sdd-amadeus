#include "ATC_flight_display.h"

ATC_flight_display ATC_flight_display::mATC_flight_display;

void ATC_flight_display::initialize(Gwen::Controls::Base* pCanvas)
{
	canvas = pCanvas;

	/*tinyxml2::XMLDocument document;
	tinyxml2::XMLElement* element = document.NewElement("DERP");
	element->SetAttribute("test", true);
	document.InsertEndChild(element);
	document.SaveFile("lolz.xml");*/
	//document.LoadFile("DERP.XML");
	//float test = document.FirstChildElement("DERP")->FloatAttribute("test");

	tinyxml2::XMLDocument document;
	document.LoadFile("derp.xml");
	tinyxml2::XMLElement* element = document.FirstChildElement("Base");
	base = GuiImage(element->FloatAttribute("sx"), element->FloatAttribute("sy"), element->FloatAttribute("sw"), element->FloatAttribute("sh"), element->FloatAttribute("dx"), element->FloatAttribute("dy"));
	
	element = document.FirstChildElement("BarGreen");
	barGreen = GuiImage(element->FloatAttribute("sx"), element->FloatAttribute("sy"), element->FloatAttribute("sw"), element->FloatAttribute("sh"), element->FloatAttribute("dx"), element->FloatAttribute("dy"));

	element = document.FirstChildElement("Aircraft");
	aircraft = GuiText(element->FloatAttribute("dx"), element->FloatAttribute("dy"), element->IntAttribute("align"), element->IntAttribute("size"), element->IntAttribute("r"), element->IntAttribute("g"), element->IntAttribute("b"));

	element = document.FirstChildElement("Flight");
	flight = GuiText(element->FloatAttribute("dx"), element->FloatAttribute("dy"), element->IntAttribute("align"), element->IntAttribute("size"), element->IntAttribute("r"), element->IntAttribute("g"), element->IntAttribute("b"));

	document.Clear();

	bitmap = al_load_bitmap("Resources/FIDS.png");
	fonts[0] = al_load_font("Resources/OpenSans.ttf", 12, 0);
	fonts[1] = al_load_font("Resources/OpenSans.ttf", 16, 0);

	
	window = new Gwen::Controls::WindowControl(canvas);
	window->SetTitle(L"Options");
	//window->MakeModal(false);
	window->SetClosable(false);
	//window->DisableResizing();
	window->SetSize(600, 500);
	window->SetMinimumSize(Gwen::Point(100,100));
	window->SetPadding(Gwen::Padding());

	bg = al_create_bitmap(508, 66);

	button = new Gwen::Controls::Button(window);
	button->SetSize(508, 66);
	button->Dock(Gwen::Pos::Top);
	button->SetImage(bg);
	button->SetPadding(Gwen::Padding());
	button->SetMargin(Gwen::Margin());
	button->onPress.Add(this, &ATC_flight_display::Click);

	button2 = new Gwen::Controls::Button(window);
	button2->SetSize(508, 66);
	button2->Dock(Gwen::Pos::Top);
	button2->SetImage(bg);
	button2->SetPadding(Gwen::Padding());
	button2->SetMargin(Gwen::Margin());

	click = false;
	c = 1.0f;
	//window->IsHovered();
}

void ATC_flight_display::Click()
{
	click = !click;
}

void ATC_flight_display::handleEvents(ALLEGRO_EVENT &ev)
{
	if (ev.type==ALLEGRO_EVENT_TIMER)
	{
		if (click)
		{
			if (c > -1.0f)
			{
				c -= 0.1f;
			}
		}
		else
		{
			if (c < 1.0f)
			{
				c += 0.1f;
			}
		}
	}
}

void ATC_flight_display::render()
{

	ALLEGRO_BITMAP* display = al_get_backbuffer(al_get_current_display());

	al_set_target_bitmap(bg);

	if (c >= 0.0f)
	{
		ALLEGRO_TRANSFORM transform;
		al_identity_transform(&transform);
		al_scale_transform(&transform, 1.0f, c);
		al_translate_transform(&transform, 0.0f, 40 * (1-c));
		al_use_transform(&transform);

		al_clear_to_color(al_map_rgba(0, 0, 0, 0));

		al_draw_bitmap_region(bitmap, base.sx, base.sy, base.sw, base.sh, base.dx, base.dy, 0);
		al_draw_bitmap_region(bitmap, barGreen.sx, barGreen.sy, barGreen.sw, barGreen.sh, barGreen.dx, barGreen.dy, 0);

		al_draw_text(fonts[aircraft.size], aircraft.color, aircraft.dx, aircraft.dy, aircraft.align, "Boeing 777-200 LR");
		al_draw_text(fonts[flight.size], flight.color, flight.dx, flight.dy, flight.align, "DL 716");
	}
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
	}



	al_set_target_bitmap(display);
	canvas->GetCanvas()->RenderCanvas();
	//al_draw_scaled_bitmap(bg, 0, 0, 510, 80, window->GetBounds().x + window->GetInnerBounds().x + button->GetBounds().x, window->GetBounds().y + window->GetInnerBounds().y + button->GetBounds().y, button->GetBounds().w, button->GetBounds().h, 0);
}