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
	//in the given Environment, loads all chunks about the given position
	void loadChunks(Environment* e, vec2 pos);
	//takes in a newly created chunk, and uses it to create a bunch of tiles for that chunk
	std::vector<Tile*> generateTiles(Chunk* chunk);
private:
	AssetHandler* assetHandler;
	int chunkLoadRadius = 1;
	void verifyNeighbors(Environment* e, vec2 pos);
};