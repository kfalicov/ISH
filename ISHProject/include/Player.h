#pragma once
#include "Entity.h"

class Player : public Agent {
public:
	Player();
	~Player();

	Item* weapon;
	void Move(vec2 dir);
private:
};