#pragma once
#include "global.h"
#include "states_manager.h"

namespace Scenario
{
	class Menu : public States::Base
	{
	protected:
		Menu() {};

	private:
		static Menu mMenu;

		Gwen::Controls::Base* canvas;
		Gwen::Controls::WindowControl* window;
		Gwen::Controls::Button* windowButton;
		void windowButtonClick();
		Gwen::Controls::ScrollControl* scroll;
		void scrollMove();
		Gwen::Controls::Button* button1;
		void windowCreate();
		void windowResize();
		void button1HoverEnter() {button1->SetImage(al_create_sub_bitmap(scenarioButtons2, 0, 0, 340, 206));};
		void button1HoverLeave() {button1->SetImage(al_create_sub_bitmap(scenarioButtons, 0, 0, 340, 206));};
		Gwen::Controls::Button* button2;
		void button2HoverEnter() {button2->SetImage(al_create_sub_bitmap(scenarioButtons2, 0, 256, 446, 206));};
		void button2HoverLeave() {button2->SetImage(al_create_sub_bitmap(scenarioButtons, 0, 256, 446, 206));};
		Gwen::Controls::Button* button3;
		void button3HoverEnter() {button3->SetImage(al_create_sub_bitmap(scenarioButtons2, 0, 512, 438, 206));};
		void button3HoverLeave() {button3->SetImage(al_create_sub_bitmap(scenarioButtons, 0, 512, 438, 206));};
		Gwen::Controls::Button* button4;
		void button4HoverEnter() {button4->SetImage(al_create_sub_bitmap(scenarioButtons2, 0, 768, 390, 206));};
		void button4HoverLeave() {button4->SetImage(al_create_sub_bitmap(scenarioButtons, 0, 768, 390, 206));};
		Gwen::Controls::Button* button5;
		void button5HoverEnter() {button5->SetImage(al_create_sub_bitmap(scenarioButtons2, 512, 0, 504, 206));};
		void button5HoverLeave() {button5->SetImage(al_create_sub_bitmap(scenarioButtons, 512, 0, 504, 206));};
		Gwen::Controls::Button* buttonBack;
		Gwen::Controls::Base* padding;


		ALLEGRO_BITMAP* scenario1;
		ALLEGRO_BITMAP* scenario2;
		ALLEGRO_BITMAP* scenario3;
		float width;
		float height;
		float offset;
		float position;

		bool out;

		ALLEGRO_BITMAP* scenarioButtons;
		ALLEGRO_BITMAP* scenarioButtons2;

	public:
		static Menu* getInstance() {return &mMenu;};

		void load(States::Manager* manager) {};
		void initialize(States::Manager* manager);
		void resize(States::Manager* manager);

		void pause() {};
		void resume() {};

		void handleEvents(ALLEGRO_EVENT &ev);
		void render();

		void cleanup();
	};
};