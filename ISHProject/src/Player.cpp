#include "Player.h"
#include "AssetHandler.h"
#include "ActionManager.h"

Player::Player() {
	sprite = AssetHandler::Instance()->GetSprite("Assets/Lemon.png", 0);

	chunkPos = vec2(0,0);
	tilePos = vec2(0,0);

	ActionManager::Instance()->player = this;
}

Player::~Player() {

}

void Player::Move(vec2 dir) {
	if (renderPos == currentPos) {
		oldPos = currentPos;

		moveTicks = 0;

		//getChunk will change tilePos to the new position if there is a space available
		currentChunk = currentChunk->getChunk(tilePos, dir);

		currentTile->opaque = nullptr;
		currentTile = currentChunk->getTile(tilePos);
		currentTile->opaque = this;

		//tilePos = currentTile->tilePos;
		chunkPos = currentChunk->chunkPos;
		currentPos = currentTile->tilePos;

		//std::cout << "Player moving to chunk: " << chunkPos << ", tile: " << tilePos << std::endl;
	}
}