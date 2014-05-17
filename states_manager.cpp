#include "states_manager.h"

States::Manager States::Manager::mManager;
bool States::Manager::done;

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
	if (!nStates.empty() && nStates.back() != state) {
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

void States::Manager::handleEvents(ALLEGRO_EVENT& ev)
{
	if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
		done = true;
	}
	else if (ev.type == ALLEGRO_EVENT_DISPLAY_RESIZE)
	{
		al_resize_display(ev.display.source, ev.display.width, ev.display.height);
		canvas->GetCanvas()->SetSize(ev.display.width, ev.display.height);
		canvas->SetSize(ev.display.width, ev.display.height);
		if(!nStates.empty())
		{
			nStates.back()->resize(this);
		}
	}
	else
	{
		if(!nStates.empty())
		{
			nStates.back()->handleEvents(ev);
		}
	}
}