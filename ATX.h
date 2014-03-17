#pragma once

#include "global.h"

struct Coord
{
	Coord()
	{
	};
	Coord(float x, float y, float z)
	{
		Coord::x = x;
		Coord::y = y;
		Coord::z = z;
	};
	inline friend bool operator==(const Coord& lhs, const Coord& rhs){ if(lhs.x==rhs.x && lhs.y==rhs.y && lhs.z==rhs.z){return true;}else{return false;} }
	inline friend bool operator!=(const Coord& lhs, const Coord& rhs){return !operator==(lhs,rhs);}
	float x;
	float y;
	float z;
};

struct GroundCoord
{
	GroundCoord()
	{
	};
	GroundCoord(float x, float y, float z)
	{
		location = Coord(x,y,z);
	};
	Coord location;
	std::list<int> nConnected;
};

struct Path
{
	std::string name;
	std::list<Coord> nPoints;
};

struct Airline
{
	std::string name;
	std::string code;
	ALLEGRO_BITMAP* logo;
};

struct Aircraft
{
	int			type; //enum
	Airline		company;
	std::string	flightNo;
	bool		isDeparture;
	std::string destination; //or Origin
	Coord		location;
	double		speed;
	double		currentHeading;
	double		finalHeading;
	int			direction;
	double		state; //0-30
	Path		path;
};

enum direction {NONE, LEFT, RIGHT};

class ATX : public Gwen::Event::Handler
{
protected:
	ATX() {};

private:
	static ATX mATX;

	int offsetWidth;
	int offsetHeight;

	ALLEGRO_TRANSFORM transform;
	Coord camera;

	//TESTING ONLY
	ALLEGRO_BITMAP* bg;
	GroundCoord temp[5];
	float offset;
	ALLEGRO_BITMAP* bar;
	ALLEGRO_BITMAP* bitmap;
	Path path;
	Airline airline;
	Aircraft test;

	void update();

	double arcTan(float x, float y)
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

	double calculateHeading(Coord alpha, Coord beta)
	{
		return arcTan(alpha.x - beta.x, alpha.y - beta.y) * 180.0f / ALLEGRO_PI;
	};

	double calculateHypotenuse(Coord alpha, Coord beta)
	{
		return sqrt(((alpha.x - beta.x) * (alpha.x - beta.x)) + ((alpha.y - beta.y) * (alpha.y - beta.y)));
	};

	double calculateAOA(Coord alpha, Coord beta)
	{
		double length = sqrt(((alpha.x - beta.x) * (alpha.x - beta.x)) + ((alpha.y - beta.y) * (alpha.y - beta.y)));
		return atan((beta.z - alpha.z)/length) * 180.0f / ALLEGRO_PI;
	};

public:
	static ATX* getInstance() {return &mATX;};

	void initialize(int displayW, int displayH);
	void handleEvents(ALLEGRO_EVENT &ev);
	void render();

	void setDisplayOffset(int displayW, int displayH) {offsetWidth = displayW/2; offsetHeight = displayH/2;};

};