#include "Chunk.h"
#include "AssetHandler.h"

Chunk::Chunk() {

}

Chunk::Chunk(int x, int y)
{
	chunkPos = vec2(x, y);
	neighbors = vector<Chunk*>(4);
	//TODO instantialize a 16x16 chunk
	//set chunk pos
	//create array of tiles who are members of the chunk
	//new Tile(vec2(0-15,0-15), chunk's pos)
	tileGrid = new Tile**[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		tileGrid[i] = new Tile*[CHUNK_SIZE];
	}

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			tileGrid[x][y] = new Tile((chunkPos[0]*CHUNK_SIZE) + x, (chunkPos[1]*CHUNK_SIZE) + y);
			tileGrid[x][y]->s = AssetHandler::Instance()->GetSprite("Assets/AnimTest.png", 0);
			//std::cout << tileGrid[x][y]->tilePos << std::endl;
		}
	}
}

Chunk::~Chunk()
{
	//TODO perform saving of chunk before destruction
}

void Chunk::Unload() {
	if (getEast() != nullptr) {
		getEast()->setWest(nullptr);
	}
	if (getWest() != nullptr) {
		getWest()->setEast(nullptr);
	}
	if (getSouth() != nullptr) {
		getSouth()->setNorth(nullptr);
	}
	if (getNorth() != nullptr) {
		getNorth()->setSouth(nullptr);
	}

	// TODO unload all tiles in the chunk
}

Chunk* Chunk::getNorth() {	return neighbors[dir::NORTH];}
Chunk* Chunk::getEast() {	return neighbors[dir::EAST];}
Chunk* Chunk::getSouth() {	return neighbors[dir::SOUTH];}
Chunk* Chunk::getWest() {	return neighbors[dir::WEST];}

void Chunk::setNorth(Chunk* N) {
	neighbors[dir::NORTH] = N;
}

void Chunk::setEast(Chunk* E) {
	neighbors[dir::EAST] = E;
}

void Chunk::setSouth(Chunk* S) {
	neighbors[dir::SOUTH] = S;
}

void Chunk::setWest(Chunk* W) {
	neighbors[dir::WEST] = W;
}
/*
	returns the tile relative to calling chunk's 0,0
	A recursive x-first seek for the target tile
*/
Tile* Chunk::getTile(vec2 tilePos) 
{
	//shouldnt need the ifs anymore

	//if (tilePos[0] >= CHUNK_SIZE)
	//	return getEast()->getTile(tilePos - (CHUNK_SIZE * vec2::E));
	//if (tilePos[0] < 0)
	//	return getWest()->getTile(tilePos - (CHUNK_SIZE * vec2::W));
	//if (tilePos[1] >= CHUNK_SIZE)
	//	return getSouth()->getTile(tilePos - (CHUNK_SIZE * vec2::S));
	//if (tilePos[1] < 0)
	//	return getNorth()->getTile(tilePos - (CHUNK_SIZE * vec2::N));

	return tileGrid[(int)tilePos[0]][(int)tilePos[1]];
}

Chunk* Chunk::getChunk(vec2& tilePos, vec2 direction) {
	vec2 newPos = tilePos + direction;
	if (newPos[0] >= CHUNK_SIZE) {
		if (getEast() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::E;
			tilePos = newPos;
			return getEast();
		}
		else {
			return this;
		}
	}
	if (newPos[0] < 0) {
		if (getWest() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::W;
			tilePos = newPos;
			return getWest();
		}
		else {
			return this;
		}
	}
	if (newPos[1] >= CHUNK_SIZE) {
		if (getSouth() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::S;
			tilePos = newPos;
			return getSouth();
		}
		else {
			return this;
		}
	}
	if (newPos[1] < 0) {
		if (getNorth() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::N;
			tilePos = newPos;
			return getNorth();
		}
		else {
			return this;
		}
	}

	tilePos = newPos;
	return this;
}

void Chunk::Render(Game* game, float interpolation) {
	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			tileGrid[x][y]->Render(game, interpolation);
		}
	}
}