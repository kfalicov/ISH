#pragma once
#include <vector>
#include <deque>
#include <algorithm>
#include "Tile.h"
#include "Util.h"

using namespace std;
class Tile;
#define CHUNK_SIZE 8
#define TILE_SPACING 0

class Chunk {
public:
	static enum dir { NORTH, EAST, SOUTH, WEST };
	Chunk();
	Chunk(int x, int y);
	~Chunk();	//saves data and removes chunk from memory

	void Render(Game* game, float interpolation);
	void Unload();

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

	// gets a list representation of the best path between position a and position b
	std::deque<vec2> AStarPath(vec2& a, vec2& b);

	

private:
	vector<Chunk*> neighbors;
	//vector< std::vector<Tile*> > grid; // a row-major matrix of tiles
	Tile*** tileGrid;
};

class Node {
public:
	vec2 data;
	Node* parent;
	double acost;	//distance from point a
	double bcost;	//distance to point b

	Node(vec2 place) { data = place; }
	bool operator < (const Node& other) { return acost + bcost < other.acost + other.bcost; }
	bool operator == (const Node& other) { return data == other.data; }
};