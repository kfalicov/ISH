#include "Entity.h"

Entity::Entity() {
	tilePos = chunkPos = currentPos = oldPos = renderPos = vec2();
}

Entity::~Entity() {

}

void Entity::updateRenderPosition(float interpolation) {
	renderPos = lerp(oldPos, currentPos, (moveTicks + interpolation) / moveFreq);
	moveTicks++;
}

void Entity::Move() {

}