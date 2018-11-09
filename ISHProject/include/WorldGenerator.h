#pragma once
#include "FastNoise.h"
#include "AssetHandler.h"
#include "Sprite.h"

class WorldGenerator {
public:
	WorldGenerator();
	WorldGenerator(int seed);
	~WorldGenerator();

	Sprite* getBackgroundSprite(int x, int y);
	void setSeed(int s);
private:
	int seed;
	double scale = 0.01;
	FastNoise tempMap;
	FastNoise heightMap;
	FastNoise wetMap;

	const int tiles[3][3] = {	{}, 
								{} };
};