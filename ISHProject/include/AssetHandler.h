#pragma once
#include "Game.h"
#include "SDL.h"
#include "Entity.h"
#include "Sprite.h"
#include <vector>
#include <unordered_map>

using std::vector;
using std::unordered_map;

constexpr auto UPDATES_PER_FRAME = 5;

class Entity;

class AssetHandler {
public:
	AssetHandler();
	~AssetHandler();

	void Clean();
	void Update();

	Sprite* GetSprite(const char* spriteSheet, int spriteIndex);

	std::vector<Sprite*> loadedSprites;
	std::vector<Entity*> visualEntities;
	void subscribeEntity(Entity* e) { visualEntities.push_back(e); };
	void subscribeSprite(Sprite* s) { loadedSprites.push_back(s); };

private:
	std::unordered_map<const char*, SDL_Surface*> loadedSpriteSheets;
	int animationCounter;

	void UpdateSprites();
	void UpdateEntityAnimations();
};