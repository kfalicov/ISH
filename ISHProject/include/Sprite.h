#pragma once
#include "SDL.h"

class Sprite {
public:
	Sprite();
	Sprite(SDL_Surface* spriteSheet, SDL_Rect srcRect);
	~Sprite();

	SDL_Surface* spriteSheet;
	SDL_Rect srcRect;
};