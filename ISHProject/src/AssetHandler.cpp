#pragma once

#include "AssetHandler.h"
#include "SDL_image.h"

AssetHandler* AssetHandler::instance;

AssetHandler::AssetHandler(){
	Init();
}

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
	animationCounter = 0;
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

	Sprite* s = new Sprite(surface, srcRect);
	loadedSprites.push_back(s);
	return s;
}

void AssetHandler::UpdateSprites() {
	animationCounter++;
	if (animationCounter < UPDATES_PER_FRAME) return;
	else animationCounter = 0;

	std::vector<Sprite*>::iterator it = loadedSprites.begin();
	while(it != loadedSprites.end()) {
		if ((*it) == nullptr) {
			it = loadedSprites.erase(it);
		}
		else {
			Sprite* s = (*it);
			if (s->frames.size() > 1) {
				s->currentFrameIndex = (s->currentFrameIndex + 1) % (s->frames.size());
			}
			++it;
		}
	}
}