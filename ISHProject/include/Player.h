#pragma once
#include "Entity.h"

class Player : public Entity {
public:
	Player();
	~Player();

	void Move(vec2 dir);
	void updateRenderPosition(float interpolation) override;
	void Attack() override;
	void TakeDamage(int damage) override;
	void Equip();
	void Rummage();

	Sprite* idle;
	Sprite* moving;

	Entity* melee_weapon;
	Entity* ranged_weapon;
	Entity* head_armor;
	Entity* torso_armor;
	Entity* legs_armor;
};