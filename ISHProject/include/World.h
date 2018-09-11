#pragma once
#include "Chunk.h"

class World
{
public:
	World();
	~World();
	Tile* getTile(vec2 pos);
private:
	Chunk* currentChunk;
	Chunk* spawn; //the 0,0 coordinate
};