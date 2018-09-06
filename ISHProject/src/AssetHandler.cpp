#pragma once

#include "AssetHandler.h"
#include "SDL_image.h"

#define TILE_SIZE 16;

AssetHandler* AssetHandler::instance;

AssetHandler::AssetHandler(){}

AssetHandler::~AssetHandler() {}

AssetHandler* AssetHandler::Instance()
{
	if (instance == 0) {
		instance = new AssetHandler();
	}
	return instance;
}

void AssetHandler::Init(Game* game)
{
	this->game = game;
}

void AssetHandler::Clean()
{
}

Sprite* AssetHandler::GetSprite(const char* spriteSheet, int spriteIndex)
{
	SDL_Texture* texture;

	std::map<const char*, SDL_Texture*>::iterator it;
	it = loadedSpriteSheets.find(spriteSheet);
	if (it != loadedSpriteSheets.end()) {
		//std::cout << spriteSheet << " found!" << std::endl;
		texture = it->second;
	}
	else {
		//std::cout << "Loading " << spriteSheet << " for the first time..." << std::endl;
		SDL_Surface* surface = IMG_Load(spriteSheet);
		texture = SDL_CreateTextureFromSurface(game->renderer, surface);
		loadedSpriteSheets.insert(std::pair<const char*, SDL_Texture*>(spriteSheet, texture));
		SDL_FreeSurface(surface);
	}

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect* srcRect = &SDL_Rect();
	
	w /= TILE_SIZE;
	h /= TILE_SIZE;

	srcRect->x = (spriteIndex % w) * TILE_SIZE;
	srcRect->y = (spriteIndex / w) * TILE_SIZE;
	srcRect->w = srcRect->h = TILE_SIZE;

	return new Sprite(texture, srcRect);
}

void AssetHandler::RenderSprite(Sprite *sprite, int position[2])
{
	SDL_Rect destRect = SDL_Rect();
	destRect.w = sprite->srcRect->w;
	destRect.h = sprite->srcRect->h;
	destRect.x = position[0];
	destRect.y = position[1];

	SDL_RenderCopy(game->renderer, sprite->spriteSheet, sprite->srcRect, &destRect);
}
