#pragma once
#include "SDL.h"
#include "Game.h"

class Sprite {
public:
	Sprite(SDL_Texture* spriteSheet, SDL_Rect* srcRect);
	~Sprite();

	SDL_Texture* spriteSheet;
	SDL_Rect* srcRect;
};