#pragma once
#include <vector>
#include "Tile.h"
#include "Util.h"

using namespace std;
#define CHUNK_SIZE 16

class Chunk {
public:
	static enum dir { NORTH, EAST, SOUTH, WEST };
	Chunk();
	Chunk(int x, int y);
	~Chunk();	//saves data and removes chunk from memory

	void Render(Game* game, float interpolation);

	vec2 chunkPos; // the chunk coordinate

	Chunk* getNorth();	//get the chunk to the north of this one
	Chunk* getEast();	//get the chunk to the east of this one
	Chunk* getSouth();	//get the chunk to the south of this one
	Chunk* getWest();	//get the chunk to the west of this one

	void setNorth(Chunk* N);
	void setEast(Chunk* E);
	void setSouth(Chunk* S);
	void setWest(Chunk* W);

	Tile* getTile(vec2 tilePos);

	//Tile* getNorth(Tile* t);	//get the tile north of the specified tile
	//Tile* getEast(Tile* t);		//get the tile east of the specified tile
	//Tile* getSouth(Tile* t);	//get the tile south of the specified tile
	//Tile* getWest(Tile* t);		//get the tile west of the specified tile

	//operators
	//vector<Tile*>& operator [](int x); // gets the specified row in the chunk

private:
	vector<Chunk*> neighbors;
	//vector< std::vector<Tile*> > grid; // a row-major matrix of tiles
	Tile*** tileGrid;
};