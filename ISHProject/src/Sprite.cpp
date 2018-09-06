#include "Sprite.h"
#include "SDL.h"

Sprite::Sprite(SDL_Texture* spriteSheet, SDL_Rect* srcRect)
{
	this->spriteSheet = spriteSheet;
	this->srcRect = srcRect;
}

Sprite::~Sprite() {

}