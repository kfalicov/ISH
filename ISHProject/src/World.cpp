#include "World.h"
#include "Util.h"
#include "Player.h"
#include "Camera.h"

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

	centerChunkPos = player->currentChunk->chunkPos;
}

World::~World()
{

}
void World::loadChunks(vec2 center) {
	int minX = center[0] - chunkSquareRadius;
	int maxX = center[0] + chunkSquareRadius;
	int minY = center[1] - chunkSquareRadius;
	int maxY = center[1] + chunkSquareRadius;

	//For all chunks that SHOULD BE loaded...
	for (int chunkX = minX; chunkX <= maxX; chunkX++) {
		for (int chunkY = minY; chunkY <= maxY; chunkY++) {
			vec2* pos = new vec2(chunkX, chunkY);

			// TODO properly load/generate chunk
			if (getLoadedChunk(vec2(chunkX, chunkY)) == nullptr) { //If they are not already loaded, load them
				Chunk* newChunk = new Chunk(chunkX, chunkY);
				loadedChunks.insert(pair<vec2*, Chunk*>(pos, newChunk));
			}
			//std::cout << chunkX << ", " << chunkY << std::endl;
		}
	}

	//Set up NSEW chunks
	for (unordered_map<vec2*, Chunk*>::iterator it = loadedChunks.begin(); it != loadedChunks.end();) {
		vec2 p = (*it->first);

		//if a chunk is loaded and it shouldn't be, unload it
		if (p[0] < minX || p[0] > maxX || p[1] < minY || p[1] > maxY) {
			it->second->Unload();
			it = loadedChunks.erase(it);
			continue;
		}

		if (it->second->getNorth() == nullptr) {
			it->second->setNorth(getLoadedChunk(p + vec2::N));
		}
		if (it->second->getEast() == nullptr) {
			it->second->setEast(getLoadedChunk(p + vec2::E));
		}
		if (it->second->getSouth() == nullptr) {
			it->second->setSouth(getLoadedChunk(p + vec2::S));
		}
		if (it->second->getWest() == nullptr) {
			it->second->setWest(getLoadedChunk(p + vec2::W));
		}

		it++;
	}
}

Chunk* World::getLoadedChunk(vec2 position) {
	for (unordered_map<vec2*, Chunk*>::iterator it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
		vec2* p = it->first;
		if ((*p) == position) {
			return it->second;
		}
	}
	return nullptr;
}
void World::Update(Game* game) {
	game->mainCamera->TrackTo(player->renderPos*PIXELS_PER_TILE + PIXELS_PER_TILE/2);

	if (centerChunkPos != player->currentChunk->chunkPos) {
		centerChunkPos = player->currentChunk->chunkPos;
		loadChunks(centerChunkPos);
	}
}

void World::Render(Game* game, float interpolation) {
	for (unordered_map<vec2*, Chunk*>::iterator it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
		it->second->Render(game, interpolation);
	}
}