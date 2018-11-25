#pragma once
#include <vector>
#include <unordered_map>

#include "Game.h"
#include "SDL.h"
#include "Entity.h"
#include "Sprite.h"

using std::vector;
using std::unordered_map;

class Entity;

class AssetHandler {
public:
	AssetHandler();
	~AssetHandler();

	void Clean();
	bool Update();

	Sprite* GetSprite(const char* spriteSheet, int spriteIndex);

	//Progresses each sprite to the next frame of their animation.
	void UpdateSpriteFrames(std::vector<Sprite*> sprites);
	//Changes the sprite that an entity is displaying.
	void UpdateEntityAnimations(std::vector<Entity*> entities);

private:
	std::unordered_map<const char*, SDL_Surface*> loadedSpriteSheets;
	int updates_per_frame;
	int animationCounter;
};