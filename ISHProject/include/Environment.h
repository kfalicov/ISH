#pragma once
#include <vector>
#include <deque>
#include <algorithm>
#include "Util.h"
#include "Sprite.h"
#include "Entity.h"

#define HEURISTIC diagonal
#define CHUNK_SIZE 8 //number of tiles per chunk (n * n)
#define TILE_SIZE 16 //edge length of tile sprites, can be used for spacing

class Entity;

class Tile
{
public:
	//creates a Tile with the given coordinate (bounded by 0-15) and knowing the position of its chunk
	Tile(vec2 pos, Chunk* parent);
	//destroys the tile TODO
	~Tile() {};

	//returns whether it successfully added an entity to this tile
	bool addOccupant(Entity* e);

	//cycles through the displayed entities on the tile
	void cycleItems();
	//removes the current display item unless a specific index is passed in
	Entity* removeOccupant(int index=-1);

	//returns the position of the tile in pixel-space of the Chunk
	vec2 getPosition();
	//returns the tile's background sprite
	Sprite* getSprite();
	//returns the last transparent entity and the solid entity (if either is applicable)
	std::vector<Entity*> getTopOccupants();
	Chunk* getParentChunk();

private:
	//Which item in the tile to display (out of all non-solids)
	//is -1 if no items to display on the tile.
	int displayIndex = -1;
	//the occupants in the tile. The last item
	//should be checked if solid.
	std::vector<Entity*> occupants;
	//the index {[0-15], [0-15]} in the chunk
	vec2 pos; 
	//the sprite to use as the background image of the tile
	Sprite* sprite;
	//which chunk this sprite belongs to
	Chunk* parent;
};

class Chunk {
public:
	static enum dir { NORTH, EAST, SOUTH, WEST };
	Chunk() {};
	Chunk(int x, int y);
	//saves data and removes chunk from memory
	~Chunk() {/*TODO*/};	

	void Render(float interpolation);

	vec2 chunkPos; // the chunk coordinate

	Chunk* getNorth();	//get the chunk to the north of this one
	Chunk* getEast();	//get the chunk to the east of this one
	Chunk* getSouth();	//get the chunk to the south of this one
	Chunk* getWest();	//get the chunk to the west of this one

	void setNorth(Chunk* N);
	void setEast(Chunk* E);
	void setSouth(Chunk* S);
	void setWest(Chunk* W);

	Tile* getTile(vec2 tilePos);
	std::vector<vec2> neighborsOf(vec2 tilePos);

	Chunk* getChunk(vec2& tilePos, vec2 direction);

	//various heuristics for distance of pathfinding
	double manhattan(vec2 a, vec2 b);	//gets distance along "blocks"
	double psquared(vec2 a, vec2 b);	//gets direct distance squared using pythagorean theorem
	double diagonal(vec2 a, vec2 b);	//moves along whichever axis is the largest

	// gets a queue representation of the best path between position a and position b
	std::deque<vec2> AStarPath(vec2& a, vec2& b);



private:
	std::vector<Chunk*> neighbors;
	//vector< std::vector<Tile*> > grid; // a row-major matrix of tiles
	Tile*** tileGrid;
};

/*
HELPER FUNCTIONS FOR PATHFINDING
*/

class Node {
public:
	vec2 data;
	Node* parent;
	double acost;	//distance from point a
	double bcost;	//distance to point b

	Node(vec2 place) { data = place; }
	bool operator < (const Node& other) { return acost + bcost > other.acost + other.bcost; }
	bool operator == (const Node& other) { return data == other.data; }
};

struct PointerCompare {
	bool operator()(const Node* l, const Node* r) {
		return  l->acost + l->bcost < r->acost + r->bcost;
	}
};

struct IsNode
{
	vec2 m_value;

	IsNode(vec2 value) : m_value(value) {}

	bool operator()(const Node *cls) const
	{
		return (cls->data == m_value);
	}
};