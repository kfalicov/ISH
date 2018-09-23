#include "Player.h"
#include "AssetHandler.h"
#include "ActionManager.h"

Player::Player() {
	sprite = AssetHandler::Instance()->GetSprite("Assets/Lemon.png", 0);
	ActionManager::Instance()->player = this;
}

Player::~Player() {

}

void Player::Move(vec2 dir) {
	if (canMove) {
		oldPos = currentPos;

		vec2 oldTilePos = tilePos;
		Chunk* newChunk = currentChunk->getChunk(tilePos, dir);
		if (newChunk->getTile(tilePos)->opaque == nullptr) {
			moveTicks = 0;

			//getChunk will change tilePos to the new position if there is a space available
			currentChunk = newChunk;

			currentTile->opaque = nullptr;
			currentTile = currentChunk->getTile(tilePos);
			currentTile->opaque = this;

			//tilePos = currentTile->tilePos;
			chunkPos = currentChunk->chunkPos;
			currentPos = currentTile->tilePos;

			//std::cout << "Player moving to chunk: " << chunkPos << ", tile: " << tilePos << std::endl;
		}
		else {
			tilePos = oldTilePos;
		}
	}
}