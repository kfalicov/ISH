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

	int animPeriod = 8;	// Number of updates to visually transition from a to b
	int moveFreq = 12; // the speed at which movement can happen. read as "once every x ticks"
						// ^ must be higher than animPeriod
	int moveTicks = 0;	//"e"
	
	vec2 currentPos;// Current position (movement interpolation target)
	vec2 oldPos;	// Old position
	vec2 renderPos; //where to render the entity

	bool canMove;

	void updateRenderPosition(float interpolation);
	void Move();
private:

};