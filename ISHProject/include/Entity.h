#pragma once
#include "Util.h"
#include "Sprite.h"

class Entity {
public:
	Entity();
	~Entity();

	Sprite* sprite;
	//TODO Tile* tilePos;	// This is effectively the entity's position
	bool opaque;	// Does the entity take up physical space?
	vec2 tilePos;
	vec2 chunkPos;

	int moveFreq = 5;	// Number of updates to move from a to b
	int moveTicks = 0;	//"e"
	vec2 currentPos;// Current position (movement interpolation target)
	vec2 oldPos;	// Old position
	vec2 renderPos; //where to render the entity

	void updateRenderPosition(float interpolation);
	void Move();
private:

};