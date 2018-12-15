#pragma once
#include <iostream>
#include "Sprite.h"
#include "Environment.h"
#include "Entity.h"

Entity::Entity(Tile* spawnTile, bool solid, std::string name) {
	animations = std::vector<Sprite*>();
	this->solid = solid;
	this->name = name;
	this->displayAnimationIndex = 0;
	this->nextAnimationIndex = 0;
	this->currentTile = spawnTile;
	this->previousTile = spawnTile;
	this->visualMoveDuration = 8;
	this->updatesPerMove = 10;
	this->updatesSinceMove = updatesPerMove;
	this->nextTile = nullptr;
	this->facingTile = spawnTile;
	spawnTile->addOccupant(this);
}

Entity::~Entity()
{

}

bool Entity::Update()
{
	bool hasMoved = false;
	if (shouldChangeAnimation) {
		shouldChangeAnimation = false;
		if (nextAnimationIndex != displayAnimationIndex) {
			displayAnimationIndex = nextAnimationIndex;
			animations.at(displayAnimationIndex)->resetAnimation();
		}
	}
	//TODO movement stuff
	if (nextTile != currentTile && nextTile != nullptr) {
		if (updatesSinceMove >= updatesPerMove) {
		
			//this will be true if the tile is not occupied by another solid entity
			if (nextTile->addOccupant(this)) {
				updatesSinceMove = 0;
				previousTile = currentTile;
				currentTile->depart();
				currentTile = nextTile;
				
				hasMoved = true;
			}
			else {
				std::cout << "blocked in this direction" << std::endl;
			}
		}
	}
	nextTile = nullptr;
	updatesSinceMove++;
	return hasMoved;
}

void Entity::Attack() {
	//gets the enemy of the facing tile
	//TODO check if facingTile is not you
	Entity* enemy = facingTile->getTopOccupants().back();
	if(enemy->isSolid()) {
		float modifier = 1;
		if (this->getAttackType() == enemy->getResistanceType()) {
			modifier = 0.75;
		}
		//TODO better damage function.
		//this one adds 1 to make sure an attack always damages at least 1
		enemy->TakeDamage(int(attack*modifier)+1);
	}
}

void Entity::TakeDamage(int damage) {
	this->currentHealth = std::min(this->currentHealth+damage, this->maxHealth);
	//TODO check for death somewhere
	if (currentHealth < 0) {
		std::cout << "i died" << std::endl;
	}
}

void Entity::addSprite(Sprite* sprite) {
	animations.push_back(sprite);
}

void Entity::setNext(Tile * nextTile)
{
	if (nextTile == nullptr) {
		return;
	}
	if (nextTile->getPositionInWorld() == this->currentTile->getPositionInWorld()) {
		nextTile = nullptr;
	}
	else {
		this->nextTile = nextTile;
	}
}

Sprite* Entity::getDisplaySprite() {
	return animations.at(displayAnimationIndex);
}

void Entity::queueAnimationChange() {
	shouldChangeAnimation = true;
}

bool Entity::isSolid() {
	return solid;
}

bool Entity::movedToNewChunk()
{
	return !(this->currentTile->getParentChunk()->chunkPos == this->previousTile->getParentChunk()->chunkPos);
}

std::string Entity::toString() {
	return name;
}