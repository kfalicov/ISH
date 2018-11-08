#include "WorldGenerator.h"
#include "FastNoise.h"
#include "Tile.h"
#include "AssetHandler.h"

WorldGenerator::WorldGenerator() {

}

WorldGenerator::WorldGenerator(int seed) {
	this->seed = seed;
	tempMap = FastNoise();
	tempMap.SetNoiseType(FastNoise::SimplexFractal);
	tempMap.SetFrequency(0.02);
	tempMap.SetSeed(seed);
}

WorldGenerator::~WorldGenerator() {

}

Sprite* WorldGenerator::getBackgroundSprite(int x, int y) {
	double temp = tempMap.GetNoise(x, y);
	//std::cout << temp << std::endl;

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