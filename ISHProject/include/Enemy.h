#pragma once
#include "Entity.h"
#include "AssetHandler.h"
#include "ActionManager.h"

class Enemy : public Entity {
public:
	Enemy();
	~Enemy();

	void Move() override;
private:
};