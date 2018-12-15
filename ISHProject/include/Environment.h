#include <vector>
#include <deque>
#include <algorithm>
#include <deque>
#include "Util.h"

#define HEURISTIC diagonal
constexpr auto CHUNK_SIZE = 4; //number of tiles per chunk (n * n);
constexpr auto TILE_SIZE = 16; //edge length of tile sprites, can be used for spacing;

//forward declarations (for all pointer types used in method signatures)
class Chunk;
class Entity;
class Sprite;
class AssetHandler;
struct SDL_Surface;

class Tile
{
public:
	//creates a Tile with the given coordinate (bounded by 0-15) and knowing the chunk it belongs to
	Tile(vec2 pos, Chunk* parent);
	//destroys the tile TODO
	~Tile();

	//returns whether it successfully added an entity to this tile
	bool addOccupant(Entity* e);

	//cycles through the displayed entities on the tile
	void cycleItems();
	//removes the current display item unless a specific index is passed in
	Entity* removeOccupant(int index=-1);
	void depart();

	//gets the pixel position for Chunk's render
	vec2 getPixelPositionInChunk();
	//gets the pixel position for render relative to world
	vec2 getPixelPositionInWorld();
	//returns the position of the tile in world space
	vec2 getPositionInWorld();
	//returns the position of the tile within the chunk
	vec2 getPositionInChunk();

	//returns the tile's background sprite
	Sprite* getSprite();
	//returns the last transparent entity and the solid entity (if either is applicable)
	std::vector<Entity*> getTopOccupants();
	Chunk* getParentChunk();

	void setSprite(Sprite* sprite);

private:
	//Which item in the tile to display (out of all non-solids)
	//is -1 if no items to display on the tile.
	int displayIndex;
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
	Chunk(vec2 pos);
	//TODO save data and remove chunk from memory
	~Chunk();

	void setTiles(std::vector<Tile*> tileGrid);

	SDL_Surface* RenderTerrain();

	vec2 chunkPos; // the chunk coordinate

	Chunk* getNorth();	//get the chunk to the north of this one
	Chunk* getEast();	//get the chunk to the east of this one
	Chunk* getSouth();	//get the chunk to the south of this one
	Chunk* getWest();	//get the chunk to the west of this one

	//sets the chunk's neighbors to each other
	static void pairHorizontal(Chunk* west, Chunk* east);
	static void pairVertical(Chunk* north, Chunk* south);

	void setNorth(Chunk* N);
	void setEast(Chunk* E);
	void setSouth(Chunk* S);
	void setWest(Chunk* W);

	Tile* getTile(vec2 tilePos);
	std::vector<Tile*> getTileGrid() { return tileGrid; }
	std::vector<vec2> neighborsOf(vec2 tilePos);

	Tile* getAdjacentTile(Tile* currentTile, vec2 direction);

	//various heuristics for distance of pathfinding
	double manhattan(vec2 a, vec2 b);	//gets distance along "blocks"
	double psquared(vec2 a, vec2 b);	//gets direct distance squared using pythagorean theorem
	double diagonal(vec2 a, vec2 b);	//moves along whichever axis is the largest

	// gets a queue representation of the best path between position a and position b
	std::deque<vec2> AStarPath(vec2& a, vec2& b);

private:
	enum dir { NORTH, EAST, SOUTH, WEST };
	std::vector<Chunk*> neighbors;
	std::vector<Tile*> tileGrid; // all of the tiles in the current Chunk
	SDL_Surface* terrainSurface;
	
	//Tile*** tileGrid;
};

class Environment {
public:
	//TODO make the Environment class essentially the world storage
	//initialize it with the assethandler, and it will pass along the assets to the tiles it creates.
	Environment(int radius);
	~Environment();

	void Update();

	Chunk* getCenterChunk() { return loadedChunks[int((loadDistX*loadDistY) / 2)]; }


	void setChunks(std::deque<Chunk*> chunks) { loadedChunks = chunks; }
	void connectNeighbors();

	//shifts the loaded chunk space east by 1 chunk and inserts newchunks
	void moveEast(std::deque<Chunk*> newChunks);

	//shifts the loaded chunk space south by 1 chunk and inserts newchunks
	void moveSouth(std::deque<Chunk*> newChunks);

	//shifts the loaded chunk space west by 1 chunk and inserts newchunks
	void moveWest(std::deque<Chunk*> newChunks);

	//shifts the loaded chunk space north by 1 chunk and inserts newchunks
	void moveNorth(std::deque<Chunk*> newChunks);
	vec2 getTopLeft() { return loadedChunks.front()->chunkPos; }
	vec2 getBottomRight() { return loadedChunks.back()->chunkPos; }
	SDL_Surface* RenderTerrain();
	SDL_Surface* RenderEntities(float interpolation);
	void addEntity(Entity* e) { entities.push_back(e); }
private:
	int loadDistX;
	int loadDistY;
	std::deque<Chunk*> loadedChunks;
	//the ground of the environment
	SDL_Surface* terrainSurface;
	//all of the entities of the environment
	SDL_Surface* entitySurface;
	//set to true if loadedChunks changes
	bool needsGroundRender;
	//set to true if entities have moved
	bool needsEntityRender;

	std::vector<Entity*> entities;
};

//HELPER FUNCTIONS FOR PATHFINDING

class Node {
public:
	vec2 data;
	Node* parent;
	double acost;	//distance from point a
	double bcost;	//distance to point b

	Node(vec2 place) { data = place; parent = nullptr; acost = 0.; bcost = 0.; }
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