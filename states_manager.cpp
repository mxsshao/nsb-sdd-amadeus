#include "states_manager.h"

States::Manager States::Manager::mManager;

void States::Manager::initialize(ALLEGRO_DISPLAY* pDisplay, ALLEGRO_EVENT_QUEUE* pEvent_queue, Gwen::Controls::Base* pCanvas)
{
	display = pDisplay;
	event_queue = pEvent_queue;
	canvas = pCanvas;
}

void States::Manager::changeState(States::Base* state)
{
	// cleanup the current state
	if (!nStates.empty()) {
		nStates.back()->cleanup();
		nStates.pop_back();
	}

	// store and init the new state
	nStates.push_back(state);
	nStates.back()->load(this);
	nStates.back()->initialize(this);

	al_flush_event_queue(event_queue);
}

void States::Manager::pauseState(States::Base* state)
{
	if (!nStates.empty()) {
		nStates.back()->pause();
		nStates.push_back(state);
		state->load(this);
		state->initialize(this);
	}
}

void States::Manager::resumeState()
{
	// cleanup the current state
	if (!nStates.empty()) {
		nStates.back()->cleanup();
		nStates.pop_back();
		nStates.back()->resume();
	}
}