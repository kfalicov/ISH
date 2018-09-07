#pragma once

#include "AssetHandler.h"
#include "SDL_image.h"

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
	SDL_Surface* surface;

	std::map<const char*, SDL_Surface*>::iterator it;
	it = loadedSpriteSheets.find(spriteSheet);
	if (it != loadedSpriteSheets.end()) {
		//std::cout << spriteSheet << " found!" << std::endl;
		surface = it->second;
	}
	else {
		//std::cout << "Loading " << spriteSheet << " for the first time..." << std::endl;
		surface = IMG_Load(spriteSheet);
		loadedSpriteSheets.insert(std::pair<const char*, SDL_Surface*>(spriteSheet, surface));
	}

	int w, h;
	w = surface->clip_rect.w;
	h = surface->clip_rect.h;
	SDL_Rect srcRect = SDL_Rect();
	
	w /= game->TILE_SIZE;
	h /= game->TILE_SIZE;

	srcRect.x = (spriteIndex % w) * game->TILE_SIZE;
	srcRect.y = (spriteIndex / w) * game->TILE_SIZE;
	srcRect.w = srcRect.h = game->TILE_SIZE;

	return new Sprite(surface, srcRect);
}