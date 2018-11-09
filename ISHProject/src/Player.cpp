#include "Player.h"
#include "AssetHandler.h"
#include "ActionManager.h"

Player::Player() {
	tilePos = chunkPos = currentPos = oldPos = renderPos = facing = vec2();
	canMove = true;
	moveTicks = 0;

	sprite = AssetHandler::Instance()->GetSprite("Assets/Lemon.png", 0);
	ActionManager::Instance()->player = this;
	health = 1000;
	facing = vec2::W;
}

Player::~Player() {

}

void Player::updateRenderPosition(float interpolation) {
	renderPos = lerp(oldPos, currentPos, (moveTicks + interpolation) / animPeriod);
}

void Player::Move(vec2 dir) {
	if (dir != vec2(0, 0)) {
		facing = dir;
	}

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

void Player::Attack() {
	//std::cout << "Attacker's pos: " << currentPos << std::endl;
	//std::cout << "Under attack: " << facing << std::endl;
	vec2 tileToAttack = tilePos;
	Chunk c = (*currentChunk->getChunk(tileToAttack, facing));
	Tile t = (*c.getTile(tileToAttack));
	if (t.opaque) {
		t.opaque->TakeDamage(attackStrength);
	}
}

void Player::setAttackStrength(int attack) {
	attackStrength = attack;
}

void Player::TakeDamage(int damage) {
	std::cout << "Agent has taken " << damage << " damage!" << std::endl;
	health -= damage;
	std::cout << "Agent health now " << health << "." << std::endl;
}

void Player::Equip() {
	if (currentTile->transparent.size() > 0) {
		if (weapon) {
			currentTile->transparent.insert(currentTile->transparent.begin(), weapon);
			weapon = nullptr;
		}
		weapon = currentTile->transparent.back();
		currentTile->transparent.pop_back();
		setAttackStrength(weapon->attackStrength);
	}
	else {
		if (weapon) {
			currentTile->transparent.insert(currentTile->transparent.begin(), weapon);
			weapon = nullptr;
		}
	}
}