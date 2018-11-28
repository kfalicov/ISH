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
	this->updatesSinceMove = updatesPerMove;
	this->nextTile = nullptr;
	this->facingTile = spawnTile;
	spawnTile->addOccupant(this);
}

Entity::~Entity()
{

}

void Entity::Update()
{
	if (shouldChangeAnimation) {
		shouldChangeAnimation = false;
		if (nextAnimationIndex != displayAnimationIndex) {
			displayAnimationIndex = nextAnimationIndex;
			animations.at(displayAnimationIndex)->resetAnimation();
		}
	}
	//TODO movement stuff
	if (updatesSinceMove >= updatesPerMove) {
		if (nextTile != currentTile && nextTile != nullptr) {
			std::cout << nextTile->getWorldPosition() << std::endl;
			updatesSinceMove = 0;
			previousTile = currentTile;
			currentTile = nextTile;
			nextTile = nullptr;
		}
	}
	updatesSinceMove++;
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
	this->nextTile = nextTile;
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

std::string Entity::toString() {
	return name;
}