#include <SDL_image.h>

#include "AssetHandler.h"
#include "Environment.h"

AssetHandler::AssetHandler() {
	animationCounter = 0;
	updates_per_frame = 5;
}

AssetHandler::~AssetHandler() {}

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
	w /= TILE_SIZE;
	h /= TILE_SIZE;

	srcRect.x = (spriteIndex % w) * TILE_SIZE;
	srcRect.y = (spriteIndex / w) * TILE_SIZE;
	srcRect.w = srcRect.h = TILE_SIZE;

	return new Sprite(surface, srcRect);
}

bool AssetHandler::Update() {
	animationCounter++;
	if (animationCounter >= updates_per_frame) {
		animationCounter = 0;
		return true;
	}
	return false;
}

void AssetHandler::UpdateSpriteFrames(std::vector<Sprite*> sprites) {
	std::vector<Sprite*>::iterator it = sprites.begin();
	while (it != sprites.end()) {
		if ((*it) == nullptr) {
			it = sprites.erase(it);
		}
		else {
			Sprite* s = (*it);
			s->nextFrame();
			++it;
		}
	}
}

void AssetHandler::UpdateEntityAnimations(std::vector<Entity*> entities) {
	std::vector<Entity*>::iterator it = entities.begin();
	while (it != entities.end()) {
		if ((*it) == nullptr) {
			it = entities.erase(it);
		}
		Entity* e = (*it);
		e->queueAnimationChange();
		++it;
	}
}