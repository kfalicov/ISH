#include "Environment.h"
#include "AssetHandler.h"
#include "Generator.h"


Generator::Generator()
{
	this->assetHandler = nullptr;
}

Generator::~Generator()
{
}

Generator::Generator(AssetHandler* assetHandler)
{
	this->assetHandler = assetHandler;
}

void Generator::loadChunks(Environment* e, vec2 pos) {
	Chunk* newCenter = e->getloadedChunk(pos);
	//if the chunk to load does not exist in the loadedchunks structure
	if (newCenter == nullptr) {
		Chunk* chunkToAdd = new Chunk(pos);
		chunkToAdd->setTiles(generateTiles(chunkToAdd));
		e->insertChunk(chunkToAdd);
		newCenter = chunkToAdd;
	}
	if (e->getCenterChunk() != newCenter) {
		e->setCenter(newCenter);
		verifyNeighbors(e, pos);
	}
}

void Generator::verifyNeighbors(Environment* e, vec2 pos) {
	//Get bounds of chunks to render
	int minX = int(pos[0] - chunkLoadRadius);
	int maxX = int(pos[0] + chunkLoadRadius);
	int minY = int(pos[1] - chunkLoadRadius);
	int maxY = int(pos[1] + chunkLoadRadius);
	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {

			Chunk* newNeighbor = e->getloadedChunk(vec2(x, y));
			//if the chunk to load does not exist in the loadedchunks structure
			if (newNeighbor == nullptr) {
				Chunk* chunkToAdd = new Chunk(vec2(x, y));
				chunkToAdd->setTiles(generateTiles(chunkToAdd));
				e->insertChunk(chunkToAdd);
			}
		}
	}
	for (int x = minX; x <= maxX; x++) {
		for (int y = minY; y <= maxY; y++) {
			Chunk* newNeighbor = e->getloadedChunk(vec2(x, y));
			//if the chunk to load does not exist in the loadedchunks structure
			if (newNeighbor != nullptr) {
				newNeighbor->setEast(e->getloadedChunk(vec2(x, y) + vec2::EAST));
				newNeighbor->setWest(e->getloadedChunk(vec2(x, y) + vec2::WEST));
				newNeighbor->setNorth(e->getloadedChunk(vec2(x, y) + vec2::NORTH));
				newNeighbor->setSouth(e->getloadedChunk(vec2(x, y) + vec2::SOUTH));
			}
		}
	}
}

std::vector<Tile*> Generator::generateTiles(Chunk* chunk) {
	//TODO instantialize a 16x16 chunk
	//set chunk pos
	//create array of tiles who are members of the chunk
	//new Tile(vec2(0-15,0-15), chunk's pos)
	std::vector<Tile*> tiles;
	for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++) {
		int x = i % CHUNK_SIZE;
		int y = int(i / CHUNK_SIZE);
		Tile* tempTile = new Tile(vec2(x, y), chunk);
		if ((x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1)) {
			Sprite* s = assetHandler->GetSprite("Assets/Temps.png", 2, TILE_SIZE);
			tempTile->setSprite(s);
		}
		else {
			Sprite* s = assetHandler->GetSprite("Assets/Temps.png", 1, TILE_SIZE);
			tempTile->setSprite(s);
		}
		tiles.push_back(tempTile);
	}
	return tiles;
}