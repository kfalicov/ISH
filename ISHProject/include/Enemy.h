#pragma once
#include "Entity.h"
#include "AssetHandler.h"
#include "ActionManager.h"

class Enemy : public Agent {
public:
	Enemy();
	~Enemy();

	void Move() override;
private:
};