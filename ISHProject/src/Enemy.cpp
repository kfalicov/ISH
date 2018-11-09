#include "Enemy.h"
#include "ActionManager.h"
#include "AssetHandler.h"
#include "Overworld.h"

Enemy::Enemy() {
	chunkPos = facing = vec2();
	canMove = true;
	moveTicks = 0;

	sprite = AssetHandler::Instance()->GetSprite("Assets/DarkLemon.png", 0);
	tilePos = currentPos = oldPos = renderPos = vec2(3,3);
	moveFreq = 15;
	ActionManager::Instance()->Subscribe(this);
	health = 15;
}

Enemy::~Enemy() {

}

void Enemy::updateRenderPosition(float interpolation) {
	renderPos = lerp(oldPos, currentPos, (moveTicks + interpolation) / animPeriod);
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

	vec2 dest = Overworld::Instance()->world->player->currentPos; //a quick and ugly way to get the player's position by using the overworld instance
	deque<vec2> path = currentChunk->AStarPath(currentPos, dest);
	if (!path.empty()) {
		dir = path.front() - currentPos;
	}
	else {
		dir = vec2(0, 0);
	}
	

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

		facing = dir;
		//std::cout << "Enemy moving to chunk: " << chunkPos << ", tile: " << tilePos << std::endl;
	}
	else {
		tilePos = oldTilePos;
	}
}

void Enemy::Attack() {
	//std::cout << "Attacker's pos: " << currentPos << std::endl;
	//std::cout << "Under attack: " << facing << std::endl;
	vec2 tileToAttack = tilePos;
	Chunk c = (*currentChunk->getChunk(tileToAttack, facing));
	Tile t = (*c.getTile(tileToAttack));
	if (t.opaque) {
		t.opaque->TakeDamage(attackStrength);
	}
}


void Enemy::TakeDamage(int damage) {
	std::cout << "Agent has taken " << damage << " damage!" << std::endl;
	health -= damage;
	std::cout << "Agent health now " << health << "." << std::endl;
}