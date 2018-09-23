#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	Player();
	~Player();

	void Move(vec2 dir);
private:
	
};