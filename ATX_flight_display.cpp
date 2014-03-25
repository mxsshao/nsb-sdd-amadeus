#include "ATX_flight_display.h"

ATX_flight_display ATX_flight_display::mATX_flight_display;

void ATX_flight_display::initialize()
{
	/*tinyxml2::XMLDocument document;
	tinyxml2::XMLElement* ATX = document.NewElement("DERP");
	tinyxml2::XMLElement* 
	element->SetAttribute("test", 1.545f);
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

	bitmap = al_load_bitmap("Resources/FIDS.png");
	document.Clear();
}

void ATX_flight_display::handleEvents(ALLEGRO_EVENT &ev)
{
}

void ATX_flight_display::render()
{
	base.Render(bitmap);
	barGreen.Render(bitmap);
}