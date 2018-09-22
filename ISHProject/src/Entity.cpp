#include "Entity.h"

Entity::Entity() {
	tilePos = chunkPos = currentPos = oldPos = renderPos = vec2();
	canMove = true;
	moveTicks = 0;
}

Entity::~Entity() {

}

void Entity::updateRenderPosition(float interpolation) {
	renderPos = lerp(oldPos, currentPos, (moveTicks + interpolation) / animPeriod);
}

void Entity::Move() {

}