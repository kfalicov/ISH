#pragma once
#include <unordered_map>
#include "Chunk.h"
#include "Player.h"

using namespace std;

class World
{
public:
	World();
	World(int seed);
	~World();
	//Tile* getTile(vec2 pos);
	Player* player;
	vec2 centerChunkPos;

	void Update(Game* game);
	void Render(Game* game, float interpolation);

	void loadChunks(vec2 center);
	Chunk* getLoadedChunk(vec2 position);

	int seed;
	unordered_map<vec2*, Chunk*> loadedChunks;
private:
	int chunkSquareRadius = 1;
	//Chunk* currentChunk;
	//Chunk* spawn; //the 0,0 coordinate
};