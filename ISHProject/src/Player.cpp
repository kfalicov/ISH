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

// entities move as soon as this is called. It is up to the caller to provide intervals
// to the movement, using the moveTicks and the moveFreq
void Player::Move(vec2 dir) {
	oldPos = currentPos;

	//getChunk will change tilePos to the new position if there is a space available
	currentChunk = currentChunk->getChunk(tilePos, dir);

	currentTile->opaque = NULL;
	currentTile = currentChunk->getTile(tilePos);
	currentTile->opaque = this;

	//tilePos = currentTile->tilePos;
	chunkPos = currentChunk->chunkPos;
	currentPos = currentTile->tilePos;

	std::cout << "Player moving to chunk: " << chunkPos << ", tile: " << tilePos << std::endl;
}