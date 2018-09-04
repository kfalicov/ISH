#pragma once
#include "GameState.h"
#include "Game.h"

class MainMenu : public GameState
{
public:
	static MainMenu* Instance();
	void Init() override;
	void Clean() override;

	void HandleEvents(Game *game, SDL_Event event) override;
	void Update(Game *game) override;
	void Render(Game *game) override;

	const char* getName() override { return "MainMenu"; }

	~MainMenu();

protected:
	static MainMenu *instance;
	MainMenu();
};