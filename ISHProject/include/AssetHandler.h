#pragma once
#include "Game.h"
#include "SDL.h"
#include "Sprite.h"
#include <vector>
#include <map>

using std::vector;
using std::map;

class AssetHandler {
public:
	static AssetHandler* Instance();
	
	void Init(Game* game);
	void Clean();

	Sprite* GetSprite(const char* spriteSheet, int spriteIndex);
	void RenderSprite(Sprite* sprite, int position[2]);

	static const enum Tiles{ GRASS, SAND, STONE };
	static const enum Weapons{ DAGGER, SWORD, AXE };
	
	~AssetHandler();

private:
	static AssetHandler* instance;

	Game* game;

	std::map<const char*, SDL_Texture*> loadedSpriteSheets;
	
	AssetHandler();

};