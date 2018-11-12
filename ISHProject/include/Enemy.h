#pragma once
#include "Entity.h"
#include "AssetHandler.h"
#include "ActionManager.h"

class Enemy : public Entity {
public:
	Enemy();
	~Enemy();

	void Move() override;
	void updateRenderPosition(float interpolation) override;
	void Attack() override;
	void TakeDamage(int damage) override;
};