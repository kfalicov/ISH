#pragma once
#include "GameState.h"
#include "Game.h"
#include <string>

using std::string;

class Console : public GameState
{
public:
	static Console* Instance(static GameState* state);

	void Init() override;
	void Clean() override;

	void HandleEvents(Game *game, SDL_Event event) override;
	void Update(Game *game) override;
	void Render(Game *game, float interpolation) override;

	void ParseCommand(string command);

	const char* getName() override { return "Console"; }

	~Console();

protected:
	static Console* instance;
	GameState* oldState;
	string currentCommand;
	vector<string> commands;

	Console();
};