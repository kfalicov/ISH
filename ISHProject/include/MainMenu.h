#pragma once
#include "GameState.h"
#include "Game.h"
#include "UI.h"

class MainMenu : public GameState
{
public:
	static MainMenu* Instance();
	void Init() override;
	void Clean() override;

	void HandleEvents(SDL_Event event) override;
	void Update() override;
	void Render(float interpolation) override;

	const char* getName() override { return "MainMenu"; }

	~MainMenu();

protected:
	static MainMenu* instance;
	MainMenu();

	std::vector<Element*> menuitems;

	Sprite* spr;
};