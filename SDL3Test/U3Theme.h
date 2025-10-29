#pragma once

#include <SDL3/SDL.h>
#include <vector>

constexpr int UI_NUM_X = 16;
constexpr int UI_NUM_Y = 3;

constexpr int MINI_X = 64;
constexpr int MINI_Y = 1;

struct ModeGraphics
{
	ModeGraphics() :
		font(nullptr),
		ui(nullptr),
		ui_width(0),
		ui_height(0),
		font_width(0),
		font_height(0),
		tiles_width(0),
		tiles_height(0),
		mini_height(0),
		mini_width(0)
	{
	}

	SDL_Texture* font;
	SDL_Texture* ui;

	std::vector<SDL_Texture*> tiles;
	std::vector<SDL_Texture*> extended_tiles;
	std::vector< SDL_Texture*> tile_target;
	std::vector<SDL_Texture*> mini_tiles;

	std::vector< SDL_Texture*> tile_display;


	float ui_width;
	float ui_height;
	float font_width;
	float font_height;
	float tiles_width;
	float tiles_height;
	float mini_height;
	float mini_width;
};
