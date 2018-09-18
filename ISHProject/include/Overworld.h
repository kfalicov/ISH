#pragma once
#include "GameState.h"
#include "Game.h"
#include "ActionManager.h"
#include "Player.h"
#include "World.h"

class Overworld : public GameState
{
public:
	static Overworld* Instance();
	void Init() override;
	void Clean() override;

	void HandleEvents(Game *game, SDL_Event event) override;
	void Update(Game *game) override;
	void Render(Game *game, float interpolation) override;

	const char* getName() override { return "Overworld"; }

	World* world;

	~Overworld();

protected:
	static Overworld *instance;
	ActionManager* actionManager;
	Overworld();
};