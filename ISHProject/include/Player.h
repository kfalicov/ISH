#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	Player();
	~Player();

	void Move(vec2 dir);
	void updateRenderPosition(float interpolation) override;
	void Attack() override;
	void setAttackStrength(int attack) override;
	void TakeDamage(int damage) override;
	void Equip();

	Item* weapon;
};