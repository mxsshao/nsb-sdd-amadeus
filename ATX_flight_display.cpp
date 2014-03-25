#include "ATX_flight_display.h"

ATX_flight_display ATX_flight_display::mATX_flight_display;

void ATX_flight_display::initialize(Gwen::Controls::Canvas* pCanvas)
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
	window->MakeModal(false);
	window->SetDeleteOnClose(true);
	window->SetClosable(false);
	window->SetSize(500, 500);

	bg = al_create_bitmap(510, 80);

	button = new Gwen::Controls::Button(window);
	button->SetBounds(0, 0, 510, 80);
	//button->Hide();
	button->SetImage(bg);
	button->SetPadding(Gwen::Padding());
	button->SetMargin(Gwen::Margin());
}

void ATX_flight_display::handleEvents(ALLEGRO_EVENT &ev)
{
}

void ATX_flight_display::render()
{

	ALLEGRO_BITMAP* display = al_get_backbuffer(al_get_current_display());

	al_set_target_bitmap(bg);

	/*ALLEGRO_TRANSFORM transform;
	al_identity_transform(&transform);
	al_scale_transform(&transform, 1.0f, 0.5f);
	al_translate_transform(&transform, 0.0f, 40 * 0.5f);
	al_use_transform(&transform);*/

	al_clear_to_color(al_map_rgba(0, 0, 0, 0));

	al_draw_bitmap_region(bitmap, base.sx, base.sy, base.sw, base.sh, base.dx, base.dy, 0);
	al_draw_bitmap_region(bitmap, barGreen.sx, barGreen.sy, barGreen.sw, barGreen.sh, barGreen.dx, barGreen.dy, 0);

	al_draw_text(fonts[aircraft.size], aircraft.color, aircraft.dx, aircraft.dy, aircraft.align, "Boeing 777-200 LR");
	al_draw_text(fonts[flight.size], flight.color, flight.dx, flight.dy, flight.align, "DL 716");



	al_set_target_bitmap(display);
	canvas->RenderCanvas();
	//al_draw_scaled_bitmap(bg, 0, 0, 510, 80, window->GetBounds().x + window->GetInnerBounds().x + button->GetBounds().x, window->GetBounds().y + window->GetInnerBounds().y + button->GetBounds().y, button->GetBounds().w, button->GetBounds().h, 0);
}