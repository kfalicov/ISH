#include "Enemy.h"
#include "ActionManager.h"
#include "AssetHandler.h"

Enemy::Enemy() {
	sprite = AssetHandler::Instance()->GetSprite("Assets/DarkLemon.png", 0);
	tilePos = currentPos = oldPos = renderPos = vec2(3,3);
	ActionManager::Instance()->Subscribe(this);
}

Enemy::~Enemy() {

}

void Enemy::Move(){
	vec2 dir;
	int r = rand() % 4;
	if (r == 0) {
		dir = vec2::N;
	}
	else if (r == 1) {
		dir = vec2::S;
	}
	else if (r == 2) {
		dir = vec2::E;
	}
	else if (r == 3) {
		dir = vec2::W;
	}

	oldPos = currentPos;
	vec2 oldTilePos = tilePos;
	Chunk* newChunk = currentChunk->getChunk(tilePos, dir);
	if (newChunk->getTile(tilePos)->opaque == nullptr) {
		oldPos = currentPos;
		moveTicks = 0;

		//getChunk will change tilePos to the new position if there is a space available
		currentChunk = newChunk;

		currentTile->opaque = nullptr;
		currentTile = currentChunk->getTile(tilePos);
		currentTile->opaque = this;

		//tilePos = currentTile->tilePos;
		chunkPos = currentChunk->chunkPos;
		currentPos = currentTile->tilePos;

		//std::cout << "Enemy moving to chunk: " << chunkPos << ", tile: " << tilePos << std::endl;
	}
}