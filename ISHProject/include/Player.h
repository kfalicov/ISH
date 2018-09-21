#pragma once
#include "Entity.h"
#include "Chunk.h"
#include "Tile.h"

class Player : public Entity {
public:
	Player();
	~Player();

	Chunk* currentChunk;
	Tile* currentTile;

	void Move(vec2 dir);
private:
	
};