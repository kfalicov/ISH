#pragma once
#include <vector>
#include "Util.h"
#include "Sprite.h"
#include "Entity.h"

class Tile
{
public:
	Tile();
	Tile(vec2 coord, vec2 chunk); //creates a Tile with the given coordinate (bounded by 0-15) and knowing the position of its chunk
	~Tile();
	vec2 chunk; //the chunk this tile belongs to
	vec2 pos; //the position 0-15 in the chunk

private:
	Sprite s;
	std::vector<Entity*> transparent;
	Entity* opaque;
	vec2 worldPos(); // returns the absolute position of this tile relative to 0,0 of the world
};