#pragma once
#include <vector>
class Environment;
class Chunk;
class Tile;
class AssetHandler;
class vec2;


class Generator {
public:
	Generator();
	~Generator();
	Generator(AssetHandler* assetHandler);
	int radius() {
		return chunkLoadRadius;
	}
	//in the given Environment, loads new chunks in the given direction
	//TODO deletes unloaded chunks
	void loadChunks(Environment* e, vec2 dir);
	//creates all chunks around a starting position (used in construction of world the first time)
	void createAllChunks(Environment* e, vec2 pos);
	//takes in a newly created chunk, and uses it to create a bunch of tiles for that chunk
	std::vector<Tile*> generateTiles(Chunk* chunk);
private:
	AssetHandler* assetHandler;
	int chunkLoadRadius = 1;
};