#pragma once
#include "Chunk.h"

class World
{
public:
	World();
	~World();
	Chunk* getChunk(int x, int y);
	Tile* getTile(vec2 pos);
private:
	Chunk* currentChunk;
	Chunk* spawn; //the 0,0 coordinate
};