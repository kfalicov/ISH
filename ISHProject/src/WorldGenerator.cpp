#include "WorldGenerator.h"
#include "FastNoise.h"
#include "Tile.h"
#include "AssetHandler.h"

WorldGenerator::WorldGenerator() {

}

WorldGenerator::WorldGenerator(int seed) {
	this->seed = seed;
	tempMap = FastNoise(); //TODO why not put the seed here?
	tempMap.SetNoiseType(FastNoise::SimplexFractal);
	tempMap.SetFrequency(0.02);
	tempMap.SetSeed(seed);

	heightMap = FastNoise(); //TODO why not put the seed here?
	heightMap.SetNoiseType(FastNoise::SimplexFractal);
	heightMap.SetFrequency(0.02);
	heightMap.SetSeed(seed);
}

WorldGenerator::~WorldGenerator() {

}

Sprite* WorldGenerator::getBackgroundSprite(int x, int y) {
	double temp = tempMap.GetNoise(x, y);
	double height = heightMap.GetNoise(x+40, y+40); //offsets the height noise from the temp noise
	//std::cout << temp << std::endl;

	//TODO set some integer values i and j based on temp and height. Then use a 2-d array access of [i][j] to get the 
	//texture to display.
	if (temp <= -0.5) {
		return AssetHandler::Instance()->GetSprite("Assets/Temps.png",
			AssetHandler::Temps::LOWEST);
	}
	else if (temp <= 0) {
		return AssetHandler::Instance()->GetSprite("Assets/Temps.png",
			AssetHandler::Temps::LOW);
	}
	else if (temp <= 0.5) {
		return AssetHandler::Instance()->GetSprite("Assets/Temps.png",
			AssetHandler::Temps::MEDIUM);
	}
	else {
		return AssetHandler::Instance()->GetSprite("Assets/Temps.png",
			AssetHandler::Temps::HIGH);
	}

	return AssetHandler::Instance()->GetSprite("Assets/AnimTest.png", 0);
}

void WorldGenerator::setSeed(int s) {
	//tempMap = FastNoise();
	tempMap.SetSeed(s);
}