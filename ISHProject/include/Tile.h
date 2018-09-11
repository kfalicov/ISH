#pragma once
#include <vector>
#include "Util.h"
#include "Sprite.h"
#include "Entity.h"

class Tile
{
public:
	Tile();
	~Tile();
	vec2 pos;
	Sprite s;
	std::vector<Entity*> transparent;
	Entity* opaque;
};