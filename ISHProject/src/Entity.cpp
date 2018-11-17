#include "..\include\Entity.h"

Entity::~Entity()
{
}

void Entity::Update()
{
	if (shouldChangeAnimation) {
		shouldChangeAnimation = false;
		if (nextAnimationIndex != displayAnimationIndex) {
			displayAnimationIndex = nextAnimationIndex;
		}
	}
}

void Entity::queueAnimationChange() {
	shouldChangeAnimation = true;
}