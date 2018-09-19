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

	int moveFreq;	// Number of updates per move
	int moveTicks = 0;	//"e"
	vec2 currentPos;// Current position (movement interpolation target)
	vec2 oldPos;	// Old position

private:

};