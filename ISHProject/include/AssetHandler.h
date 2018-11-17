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
	static AssetHandler* Instance();

	void Init();
	void Clean();
	void Update();

	Sprite* GetSprite(const char* spriteSheet, int spriteIndex);

	static const enum Temps { LOWEST, LOW, MEDIUM, HIGH };
	static const enum Tiles { GRASS, SAND, STONE };
	static const enum Weapons { DAGGER, SWORD, AXE };

	std::vector<Sprite*> loadedSprites;
	std::vector<Entity*> visualEntities;
	void subscribeEntity(Entity* e) { visualEntities.push_back(e); };
	void subscribeSprite(Sprite* s) { loadedSprites.push_back(s); };

	~AssetHandler();

private:
	static AssetHandler* instance;
	std::unordered_map<const char*, SDL_Surface*> loadedSpriteSheets;
	int animationCounter;

	void UpdateSprites();
	void UpdateEntityAnimations();

	AssetHandler();

};