#include "global.h"

namespace ATX
{
namespace Structs
{
	struct Waypoint
	{
		Waypoint() {};
		Waypoint(float ex, float ey, float ez) {x = ex; y = ey; z = ez;};
		float x;
		float y;
		float z;
		std::list<int> nConnected;
	};
	struct Flight
	{
		Flight() {};
		Flight(double eSpeed, double eCurrentHeading, float ex, float ey, float ez, int eFinalPoint, std::list<Waypoint>* ePoints)
		{
			speed = eSpeed;
			currentHeading = eCurrentHeading;
			x = ex;
			y = ey;
			z = ez;
			finalPoint = eFinalPoint;
			nPoints = *ePoints;
		};
		double speed;
		double currentHeading;
		double finalHeading;
		float x;
		float y;
		float z;
		int finalPoint;
		std::list<Waypoint> nPoints;
	};
};
};