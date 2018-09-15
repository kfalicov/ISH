#include "Chunk.h"

Chunk::Chunk()
{
	//TODO instantialize a 16x16 chunk
	//set chunk pos
	//create array of tiles who are members of the chunk
	//new Tile(vec2(0-15,0-15), chunk's pos)
}

Chunk::~Chunk()
{
	//TODO perform saving of chunk before destruction
}

Chunk* Chunk::getNorth() {	return neighbors[dir::NORTH];}
Chunk* Chunk::getEast() {	return neighbors[dir::EAST];}
Chunk* Chunk::getSouth() {	return neighbors[dir::SOUTH];}
Chunk* Chunk::getWest() {	return neighbors[dir::WEST];}

/*
	returns the tile relative to calling chunk's 0,0
	A recursive x-first seek for the target tile
*/
Tile* Chunk::getTile(vec2 pos) 
{
	if (pos[0] > 15)
		return getEast()->getTile(pos - (16 * vec2::E));
	if (pos[0] < 0)
		return getWest()->getTile(pos - (16 * vec2::W));
	if (pos[1] > 15)
		return getSouth()->getTile(pos - (16 * vec2::S));
	if (pos[1] < 0)
		return getNorth()->getTile(pos - (16 * vec2::N));

	return grid[pos[1]][pos[0]];	// these are flipped along x and y because grid is 
									// row-major which needs to be inverted so that x, y makes sense
}

Tile* Chunk::getNorth(Tile* t) 
{
	return getTile(t->pos + vec2::N);
}
Tile* Chunk::getEast(Tile* t)
{
	return getTile(t->pos + vec2::E);
}
Tile* Chunk::getSouth(Tile* t)
{
	return getTile(t->pos + vec2::S);
}
Tile* Chunk::getWest(Tile* t)
{
	return getTile(t->pos + vec2::W);
}

std::vector<Tile*>& Chunk::operator [] (int x) 
{
	//TODO check for out of bounds
	return grid[x];
}