#include "Chunk.h"

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
			tileGrid[x][y] = new Tile((chunkPos[0]*16) + x, (chunkPos[1]*16) + y);
			//std::cout << tileGrid[x][y]->tilePos << std::endl;
		}
	}
}

Chunk::~Chunk()
{
	//TODO perform saving of chunk before destruction
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
	//if (pos[0] > 15)
	//	return getEast()->getTile(pos - (16 * vec2::E));
	//if (pos[0] < 0)
	//	return getWest()->getTile(pos - (16 * vec2::W));
	//if (pos[1] > 15)
	//	return getSouth()->getTile(pos - (16 * vec2::S));
	//if (pos[1] < 0)
	//	return getNorth()->getTile(pos - (16 * vec2::N));

	return tileGrid[(int)tilePos[0]][(int)tilePos[1]];
}

void Chunk::Render(Game* game, float interpolation) {
	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			tileGrid[x][y]->Render(game, interpolation);
		}
	}
}

//Tile* Chunk::getNorth(Tile* t) 
//{
//	return getTile(t->pos + vec2::N);
//}
//Tile* Chunk::getEast(Tile* t)
//{
//	return getTile(t->pos + vec2::E);
//}
//Tile* Chunk::getSouth(Tile* t)
//{
//	return getTile(t->pos + vec2::S);
//}
//Tile* Chunk::getWest(Tile* t)
//{
//	return getTile(t->pos + vec2::W);
//}

//std::vector<Tile*>& Chunk::operator [] (int x) 
//{
//	//TODO check for out of bounds
//	return grid[x];
//}