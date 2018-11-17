#include <SDL_image.h>

#include "AssetHandler.h"
#include "Environment.h"

AssetHandler::AssetHandler() {
	animationCounter = 0;
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

void AssetHandler::Update() {
	animationCounter++;
	if (animationCounter < UPDATES_PER_FRAME) return;
	else animationCounter = 0;

	UpdateSprites();
	UpdateEntityAnimations();
}

void AssetHandler::UpdateSprites() {
	std::vector<Sprite*>::iterator it = loadedSprites.begin();
	while (it != loadedSprites.end()) {
		if ((*it) == nullptr) {
			it = loadedSprites.erase(it);
		}
		else {
			Sprite* s = (*it);
			s->nextFrame();
			++it;
		}
	}
}

void AssetHandler::UpdateEntityAnimations() {
	std::vector<Entity*>::iterator it = visualEntities.begin();
	while (it != visualEntities.end()) {
		if ((*it) == nullptr) {
			it = visualEntities.erase(it);
		}
		Entity* e = (*it);
		e->queueAnimationChange();
		/*if (e->sprite != e->new_animation && e->new_animation) {
			e->sprite = e->new_animation;
			e->sprite->currentFrameIndex = 0;
		}*/
		++it;
	}
}