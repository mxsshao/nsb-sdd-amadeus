#pragma once

#include "global.h"
#include "states_base.h"

namespace States
{
	class Manager
	{
	protected:
		Manager() {};

	private:
		static Manager mManager;

		std::vector<States::Base*> nStates;
		ALLEGRO_DISPLAY* display;
		ALLEGRO_EVENT_QUEUE* event_queue;
	
		Gwen::Controls::Base* canvas;

	public:
		static Manager* getInstance() {return &mManager;};
		void initialize(ALLEGRO_DISPLAY* pDisplay, ALLEGRO_EVENT_QUEUE* pEvent_queue, Gwen::Controls::Base* pCanvas);

		void changeState(States::Base* state);
		void pauseState(States::Base* state);
		void resumeState();
		void pushState(States::Base* state) {nStates.push_back(state);nStates.back()->load(this);nStates.back()->initialize(this);};
		void popState() {if(!nStates.empty()){nStates.back()->cleanup();nStates.pop_back();}};

		void handleEvents(ALLEGRO_EVENT& ev) {if(!nStates.empty()){nStates.back()->handleEvents(ev);}};
		void render() {if(!nStates.empty()){nStates.back()->render();}};

		void cleanup() {};

		ALLEGRO_DISPLAY* getDisplay() {return display;};
		Gwen::Controls::Base* getCanvas() {return canvas;};
	};
};