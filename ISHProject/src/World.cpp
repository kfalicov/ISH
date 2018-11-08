#include "World.h"
#include "Util.h"
#include "Player.h"
#include "Enemy.h"
#include "Camera.h"
#include "ActionManager.h"
#include "WorldGenerator.h"

World::World()
{

}

World::World(int seed) {
	worldGen = WorldGenerator(seed);
	// TODO init player from save file if applicable
	player = new Player();
	centerChunkPos = player->chunkPos;
	loadChunks();
	player->currentChunk = getLoadedChunk(player->chunkPos);
	player->currentTile = player->currentChunk->getTile(player->tilePos);
	player->currentTile->opaque = player;

	centerChunkPos = player->currentChunk->chunkPos;

	Enemy* enemy = new Enemy();
	enemy->currentChunk = getLoadedChunk(player->chunkPos);
	enemy->currentTile = enemy->currentChunk->getTile(enemy->tilePos);
	enemy->currentTile->opaque = enemy;

	Item* item = new Item();
	item->currentChunk = getLoadedChunk(player->chunkPos);
	item->currentTile = item->currentChunk->getTile(item->tilePos);
	item->currentTile->transparent.push_back(item);
}

World::~World()
{

}
void World::loadChunks() {
	int minX = centerChunkPos[0] - chunkSquareRadius;
	int maxX = centerChunkPos[0] + chunkSquareRadius;
	int minY = centerChunkPos[1] - chunkSquareRadius;
	int maxY = centerChunkPos[1] + chunkSquareRadius;

	//For all chunks that SHOULD BE loaded...
	for (int chunkX = minX; chunkX <= maxX; chunkX++) {
		for (int chunkY = minY; chunkY <= maxY; chunkY++) {
			vec2* pos = new vec2(chunkX, chunkY);

			// TODO properly load/generate chunk
			if (getLoadedChunk(vec2(chunkX, chunkY)) == nullptr) { //If they are not already loaded, load them
				Chunk* newChunk = new Chunk(worldGen, chunkX, chunkY);
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
	vec2 lead = player->facing*PIXELS_PER_TILE;
	lead *= 6; //number of tiles for camera to lead player (to show terrain in front)
	game->mainCamera->TrackTo((player->currentPos*PIXELS_PER_TILE + PIXELS_PER_TILE/2)); //+lead

	if (centerChunkPos != player->currentChunk->chunkPos) {
		centerChunkPos = player->currentChunk->chunkPos;
		loadChunks();
		for (std::vector<Agent*>::iterator it = ActionManager::Instance()->actors.begin(); it != ActionManager::Instance()->actors.end(); ++it) {
			Chunk* entityChunk = getLoadedChunk((*it)->chunkPos);
			if (entityChunk != nullptr) {
				(*it)->currentChunk = entityChunk;
				(*it)->currentTile = (*it)->currentChunk->getTile((*it)->tilePos);
			}
			else {
				(*it)->currentChunk = nullptr;
				(*it)->currentTile = nullptr;
			}
		}
	}
}

void World::Render(float interpolation) {
	int minX = centerChunkPos[0] - renderChunkSquareRadius;
	int maxX = centerChunkPos[0] + renderChunkSquareRadius;
	int minY = centerChunkPos[1] - renderChunkSquareRadius;
	int maxY = centerChunkPos[1] + renderChunkSquareRadius;

	for (unordered_map<vec2*, Chunk*>::iterator it = loadedChunks.begin(); it != loadedChunks.end(); ++it) {
		Chunk c = (*it->second);
		if (c.chunkPos[0] >= minX && c.chunkPos[0] <= maxX && c.chunkPos[1] >= minY && c.chunkPos[1] <= maxY) {
			it->second->Render(interpolation);
		}
	}
}