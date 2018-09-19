#include "World.h"
#include "Util.h"
#include "Player.h"

World::World()
{

}

World::World(int seed) {
	this->seed = seed;
	// TODO init player from save file if applicable
	player = new Player();
	loadChunks(player->chunkPos);
	player->currentChunk = getLoadedChunk(player->chunkPos);
	player->currentTile = player->currentChunk->getTile(player->tilePos);
	player->currentTile->opaque = player;
}

World::~World()
{

}

void World::loadChunks(vec2 center) {
	for (int chunkX = center[0] - chunkSquareRadius; chunkX <= center[0] + chunkSquareRadius; chunkX++) {
		for (int chunkY = center[1] - chunkSquareRadius; chunkY <= center[1] + chunkSquareRadius; chunkY++) {
			vec2* pos = new vec2(chunkX, chunkY);

			// TODO properly load/generate chunk
			Chunk* newChunk = new Chunk(chunkX, chunkY);
			loadedChunks.insert(pair<vec2*, Chunk*>(pos, newChunk));
			//std::cout << chunkX << ", " << chunkY << std::endl;
		}
	}

	for (unordered_map<vec2*, Chunk*>::iterator it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
		vec2* p = it->first;
		if(it->second->getNorth() == NULL)
			it->second->setNorth(getLoadedChunk((*p) + vec2::N));
		if (it->second->getEast() == NULL)
			it->second->setEast(getLoadedChunk((*p) + vec2::E));
		if (it->second->getSouth() == NULL)
			it->second->setSouth(getLoadedChunk((*p) + vec2::S));
		if (it->second->getWest() == NULL)
			it->second->setWest(getLoadedChunk((*p) + vec2::W));
	}
}

Chunk* World::getLoadedChunk(vec2 position) {
	for (unordered_map<vec2*, Chunk*>::iterator it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
		vec2* p = it->first;
		if ((*p) == position) {
			return it->second;
		}
	}
	return NULL;
}

void World::Render(Game* game, float interpolation) {
	for (unordered_map<vec2*, Chunk*>::iterator it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
		it->second->Render(game, interpolation);
	}
}

///*
//obtains a tile based on absolute world coordinates
//*/
//Tile* World::getTile(vec2 pos) 
//{
//	int offsetx = int(pos[0]) % 16;
//	int offsety = int(pos[1]) % 16;
//	int chunkx = (pos[0] - offsetx) / 16;
//	int chunky = (pos[1] - offsety) / 16;
//	
//	return getChunk(vec2(chunkx, chunky))->getTile(vec2(offsetx, offsety));
//}

/*
Alternate function
obtains a tile based on absolute world coordinates
*/
//Tile* World::getTile(vec2 pos) 
//{
//	return spawn->getTile(pos); //this will be called recursively when pos is out of the spawn's bounds
//}