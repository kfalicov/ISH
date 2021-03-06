#pragma once
#include "SDL.h"
#include "Game.h"
#include <vector>

class Sprite {
public:
	Sprite();
	Sprite(SDL_Surface* spriteSheet, SDL_Rect srcRect);
	Sprite(SDL_Surface* spriteSheet, std::vector<SDL_Rect> frames);
	~Sprite();

	void setRectsFromIndices(const int width, const int height, const std::vector<int> indices);

	SDL_Surface* spriteSheet;
	std::vector<SDL_Rect> frames;
	int currentFrameIndex;
	//SDL_Rect srcRect;
};