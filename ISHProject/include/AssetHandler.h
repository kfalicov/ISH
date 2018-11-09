#pragma once
#include "Game.h"
#include "SDL.h"
#include "Sprite.h"
#include <vector>
#include <unordered_map>

using std::vector;
using std::unordered_map;

class AssetHandler {
public:
	static AssetHandler* Instance();
	
	void Init();
	void Clean();

	static Sprite* GetSprite(const char* spriteSheet, int spriteIndex);

	static const enum Temps{LOWEST, LOW, MEDIUM, HIGH};
	static const enum Tiles{ GRASS, SAND, STONE };
	static const enum Weapons{ DAGGER, SWORD, AXE };
	
	~AssetHandler();

private:
	static AssetHandler* instance;
	static std::unordered_map<const char*, SDL_Surface*> loadedSpriteSheets;
	
	AssetHandler();

};