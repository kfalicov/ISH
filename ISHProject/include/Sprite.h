#pragma once
#include "SDL.h"
#include "Game.h"

class Sprite {
public:
	Sprite(SDL_Surface* spriteSheet, SDL_Rect srcRect);
	~Sprite();

	SDL_Surface* spriteSheet;
	SDL_Rect srcRect;
};