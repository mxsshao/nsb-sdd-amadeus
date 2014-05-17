#pragma once

#include "global.h"

namespace States
{
	class Manager;
	class Base : public Gwen::Event::Handler
	{
	protected:
		Base() {};

	public:
		virtual void load(States::Manager* manager) {};
		virtual void initialize(States::Manager* manager) {};
		virtual void resize(States::Manager* manager) {};

		virtual void pause() {};
		virtual void resume() {};

		virtual void handleEvents(ALLEGRO_EVENT &ev) {};
		virtual void render() {};

		virtual void cleanup() {};
	};
};