#pragma once
#include <vector>
#include "Util.h"
#include "Sprite.h"
#include "Entity.h"
#include "Game.h"

class Entity;
class Item;

class Tile
{
public:
	Tile();
	Tile(int x, int y); //creates a Tile with the given coordinate (bounded by 0-15) and knowing the position of its chunk
	~Tile();

	void Render(Game* game, float interpolation);

	vec2 tilePos; //the position 0-15 in the chunk
	Sprite* s;
	std::vector<Item*> transparent;
	Entity* opaque;
private:

	//vec2 worldPos(); // returns the absolute position of this tile relative to 0,0 of the world
};