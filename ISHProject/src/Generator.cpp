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

//generates a new row/column of chunks in the desired direction
void Generator::loadChunks(Environment* e, vec2 dir) {
	vec2 upperLeftBound = e->getTopLeft();
	vec2 lowerRightBound = e->getBottomRight();
	std::deque<Chunk*> freshChunks;
	if (dir == vec2::EAST) {
		for (int y = upperLeftBound[1]; y <= lowerRightBound[1]; y++) {
			Chunk* eastChunk = new Chunk(vec2(lowerRightBound[0] + 1, y));
			eastChunk->setTiles(generateTiles(eastChunk));
			freshChunks.push_back(eastChunk);
		}
		e->moveEast(freshChunks);
	}
	if (dir == vec2::SOUTH) {
		for (int x = upperLeftBound[0]; x <= lowerRightBound[0]; x++) {
			Chunk* southChunk = new Chunk(vec2(x, lowerRightBound[1]+1));
			southChunk->setTiles(generateTiles(southChunk));
			freshChunks.push_back(southChunk);
		}
		e->moveSouth(freshChunks);
	}
	if (dir == vec2::WEST) {
		for (int y = upperLeftBound[1]; y <= lowerRightBound[1]; y++) {
			Chunk* westChunk = new Chunk(vec2(upperLeftBound[0]-1, y));
			westChunk->setTiles(generateTiles(westChunk));
			freshChunks.push_back(westChunk);
		}
		e->moveWest(freshChunks);
	}
	if (dir == vec2::NORTH) {
		for (int x = upperLeftBound[0]; x <= lowerRightBound[0]; x++) {
			Chunk* northChunk = new Chunk(vec2(x, upperLeftBound[1] -1));
			northChunk->setTiles(generateTiles(northChunk));
			freshChunks.push_back(northChunk);
		}
		e->moveNorth(freshChunks);
	}
}

void Generator::createAllChunks(Environment* e, vec2 pos) {
	for (int x = 0 - chunkLoadRadius; x <= chunkLoadRadius; x++) {
		for (int y = 0 - chunkLoadRadius; y <= chunkLoadRadius; y++) {
			vec2 newPos(x, y);

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