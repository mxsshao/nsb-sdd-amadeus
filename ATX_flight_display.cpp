#include "ATX_flight_display.h"

ATX_flight_display ATX_flight_display::mATX_flight_display;

void ATX_flight_display::initialize()
{
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

	bitmap = al_load_bitmap("Resources/FIDS.png");
	fonts[0] = al_load_font("Resources/OpenSans.ttf", 12, 0);
	fonts[1] = al_load_font("Resources/OpenSans.ttf", 16, 0);

	document.Clear();

}

void ATX_flight_display::handleEvents(ALLEGRO_EVENT &ev)
{
}

void ATX_flight_display::render()
{
	al_draw_bitmap_region(bitmap, base.sx, base.sy, base.sw, base.sh, base.dx, base.dy, 0);
	al_draw_bitmap_region(bitmap, barGreen.sx, barGreen.sy, barGreen.sw, barGreen.sh, barGreen.dx, barGreen.dy, 0);

	al_draw_text(fonts[aircraft.size], aircraft.color, aircraft.dx, aircraft.dy, aircraft.align, "Boeing 777-200 LR");
	al_draw_text(fonts[flight.size], flight.color, flight.dx, flight.dy, flight.align, "DL 716");

}