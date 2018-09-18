#include "Sprite.h"
#include "SDL.h"

Sprite::Sprite() {

}

Sprite::Sprite(SDL_Surface* spriteSheet, SDL_Rect srcRect)
{
	this->spriteSheet = spriteSheet;
	this->srcRect = srcRect;
}

Sprite::~Sprite() {

}