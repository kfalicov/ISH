#pragma once
#include "Sprite.h"

Sprite::Sprite() {
	this->spriteSheet = nullptr;
	this->currentFrameIndex = 0;
}

Sprite::Sprite(SDL_Surface* spriteSheet, SDL_Rect srcRect)
{
	this->spriteSheet = spriteSheet;
	frames = std::vector<SDL_Rect>();
	frames.push_back(srcRect);
	currentFrameIndex = 0;
}

Sprite::Sprite(SDL_Surface * spriteSheet, std::vector<SDL_Rect> frames)
{
	this->spriteSheet = spriteSheet;
	this->frames = frames;
	currentFrameIndex = 0;
}

Sprite::~Sprite() {

}

//width and height of the sprite, indices are zero-based.
void Sprite::setRectsFromIndices(const int width, const int height, const std::vector<int> indices)
{
	if (spriteSheet == nullptr) return;

	std::vector<SDL_Rect> frameRects;

	for (int i = 0; i < int(indices.size()); i++) {
		SDL_Rect r = SDL_Rect();
		r.w = width;
		r.h = height;
		r.x = (width * indices[i]) % spriteSheet->w;
		int indexY = (width * indices[i]) / spriteSheet->w;
		r.y = indexY * height;
		frameRects.push_back(r);
	}

	frames = frameRects;
}

void Sprite::nextFrame() {
	if (frames.size() > 1) {
		currentFrameIndex = (currentFrameIndex + 1) % (frames.size());
	}
}

void Sprite::resetAnimation() {
	currentFrameIndex = 0;
}