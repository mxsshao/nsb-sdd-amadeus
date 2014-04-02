#pragma once
#include "global.h"

namespace ATX
{
class Aircraft;
namespace Structs
{
	struct Waypoint
	{
		Waypoint() {};
		Waypoint(float ex, float ey, float ez) {x = ex; y = ey; z = ez;};
		inline friend bool operator==(const Waypoint& lhs, const Waypoint& rhs){ if(lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z){return true;}else{return false;} }
		inline friend bool operator!=(const Waypoint& lhs, const Waypoint& rhs){return !operator==(lhs,rhs);}
		float x;
		float y;
		float z;
		std::list<int> nConnected;
	};
	struct Camera
	{
		Camera() {};
		Camera(float ex, float ey, float ez) {x = ex; y = ey; z = ez; following = NULL;};
		float x;
		float y;
		float z;
		Aircraft* following;
	};
	struct GuiImage
	{
		float sx;
		float sy;
		float sw;
		float sh;
		float dx;
		float dy;
		GuiImage()
		{
		};
		GuiImage(float sx, float sy, float sw, float sh, float dx, float dy)
		{
			GuiImage::sx = sx;
			GuiImage::sy = sy;
			GuiImage::sw = sw;
			GuiImage::sh = sh;
			GuiImage::dx = dx;
			GuiImage::dy = dy;
		};
	};
	struct GuiText
	{
		float dx;
		float dy;
		int align;
		int size;
		ALLEGRO_COLOR color;
		GuiText()
		{
		};
		GuiText(float dx, float dy, int align, int size, int r, int g, int b)
		{
			GuiText::dx = dx;
			GuiText::dy = dy;
			GuiText::align = align;
			GuiText::size = size;
			GuiText::color = al_map_rgb(r, g, b);
		};
	};
};
	static double arcTan(float x, float y)
	{
		if (x <= 0 && y >= 0)
			return abs(atan(x/y));
		else if (x <= 0 && y <= 0)
			return ALLEGRO_PI - abs(atan(x/y));
		else if (x >= 0 && y <= 0)
			return ALLEGRO_PI + abs(atan(x/y));
		else if (x >= 0 && y >= 0)
			return ALLEGRO_PI*2 - abs(atan(x/y));
		else
			return 0;
	};

	static double calculateHeading(Structs::Waypoint alpha, Structs::Waypoint beta)
	{
		return arcTan(alpha.x - beta.x, alpha.y - beta.y) * 180.0f / ALLEGRO_PI;
	};

	static double calculateHypotenuse(Structs::Waypoint alpha, Structs::Waypoint beta)
	{
		return sqrt(((alpha.x - beta.x) * (alpha.x - beta.x)) + ((alpha.y - beta.y) * (alpha.y - beta.y)));
	};

	static double calculateAOA(Structs::Waypoint alpha, Structs::Waypoint beta)
	{
		double length = sqrt(((alpha.x - beta.x) * (alpha.x - beta.x)) + ((alpha.y - beta.y) * (alpha.y - beta.y)));
		return atan((beta.z - alpha.z)/length) * 180.0f / ALLEGRO_PI;
	};
};