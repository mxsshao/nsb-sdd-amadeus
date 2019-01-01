#pragma once
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_native_dialog.h>
#include <cmath>
#include <iostream>

class ATX
{
public:
	ALLEGRO_FONT* font [3];

	typedef struct SPRITE_SET_LEFT_TOP
	{
		SPRITE_SET_LEFT_TOP(): sx(0), sy(0), sw(0), sh(0), dx(0), dy(0) {};
		float sx;
		float sy;
		float sw;
		float sh;
		float dx;
		float dy;
	};

	typedef struct TEXT_SET
	{
		TEXT_SET(): dx(0), dy(0), align(0), size(0), color(al_map_rgb(0,0,0)) {};
		float dx;
		float dy;
		int align;
		int size;
		ALLEGRO_COLOR color;
	};

	/*void draw_sprite_set_center_scale(ALLEGRO_BITMAP* bitmap, SPRITE_SET_CENTER* sprite, float xscale, float yscale)
	{
		al_draw_tinted_scaled_rotated_bitmap_region(bitmap, sprite->sx, sprite->sy, sprite->sw, sprite->sh, al_map_rgb_f(1,1,1), sprite->cx, sprite->cy, sprite->dx, sprite->cy, xscale, yscale, 0, 0);
	};*/

	void draw_sprite_set_left_top(ALLEGRO_BITMAP* bitmap, SPRITE_SET_LEFT_TOP* sprite)
	{
		al_draw_bitmap_region(bitmap, sprite->sx, sprite->sy, sprite->sw, sprite->sh, sprite->dx, sprite->dy, 0);
	};
};
