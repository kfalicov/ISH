#pragma once
#include "Entity.h"
#include "Chunk.h"
#include "Tile.h"

class Player : Entity {
public:
	Player();
	~Player();

	vec2 tilePos;
	vec2 chunkPos;
	Chunk* currentChunk;
	Tile* currentTile;

private:
	
};