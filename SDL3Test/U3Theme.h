#pragma once

#include <SDL3/SDL.h>
#include <vector>

constexpr int UI_NUM_X = 16;
constexpr int UI_NUM_Y = 3;

struct ModeGraphics
{
	ModeGraphics() :
		font(nullptr),
		mini(nullptr),
		//tiles(nullptr),
		ui(nullptr),
		//tile_target(nullptr),
		ui_width(0),
		ui_height(0),
		font_width(0),
		font_height(0),
		tiles_width(0),
		tiles_height(0)
	{
	}

	SDL_Texture* font;
	SDL_Texture* mini;
	SDL_Texture* ui;

	std::vector<SDL_Texture*> tiles;
	std::vector< SDL_Texture*> tile_target;
	std::vector<SDL_Texture*> ui_tiles;

	float ui_width;
	float ui_height;
	float font_width;
	float font_height;
	float tiles_width;
	float tiles_height;
};
