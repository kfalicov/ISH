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

	Player* player;
	vec2 centerChunkPos;

	void Update(Game* game);
	void Render(Game* game, float interpolation);

	void loadChunks();
	Chunk* getLoadedChunk(vec2 position);

	int seed;
	unordered_map<vec2*, Chunk*> loadedChunks;
private:
	int chunkSquareRadius = 2;
	int renderChunkSquareRadius = 1;
};