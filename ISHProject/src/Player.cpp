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
	currentTile->opaque = NULL;
	currentTile = currentChunk->getTile(tilePos + dir);//move up
	currentTile->opaque = this;
	tilePos = currentTile->tilePos;
	chunkPos = currentChunk->chunkPos;
}