#pragma once

#include "AssetHandler.h"
#include "SDL_image.h"

AssetHandler* AssetHandler::instance;
std::unordered_map<const char*, SDL_Surface*> AssetHandler::loadedSpriteSheets;

AssetHandler::AssetHandler(){}

AssetHandler::~AssetHandler() {}

AssetHandler* AssetHandler::Instance()
{
	if (instance == 0) {
		instance = new AssetHandler();
	}
	return instance;
}

void AssetHandler::Init()
{
}

void AssetHandler::Clean()
{
}

Sprite* AssetHandler::GetSprite(const char* spriteSheet, int spriteIndex)
{
	SDL_Surface* surface;

	std::unordered_map<const char*, SDL_Surface*>::iterator it;
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
	
	//using Game::instance here will break the game's startup with an infinite loop because AssetHandler is created
	//before Game is fully initialized, so it will try and re-instance the Game which will then create AssetHandler again
	//and you get the idea.
	w /= Game::TILE_SIZE;
	h /= Game::TILE_SIZE;

	srcRect.x = (spriteIndex % w) * Game::TILE_SIZE;
	srcRect.y = (spriteIndex / w) * Game::TILE_SIZE;
	srcRect.w = srcRect.h = Game::TILE_SIZE;

	return new Sprite(surface, srcRect);
}