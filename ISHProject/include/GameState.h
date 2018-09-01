#pragma once
#include "Game.h"
#include <vector>

using std::vector;

class GameState
{
public:
	virtual void Init() = 0;
	virtual void Clean() = 0;

	virtual void Pause() { isPaused = true; };
	virtual void Resume() { isPaused = false; };

	virtual void HandleEvents(Game *game, SDL_Event event) = 0;
	virtual void Update(Game *game) = 0;
	virtual void Render(Game *game) = 0;

	virtual void PushSubState(GameState *substate) { substates.push_back(substate); };
	virtual void PopSubState() { substates.pop_back(); };

	virtual const char* getName() = 0;

	void ChangeState(Game *game, GameState *state) {
		game->ChangeState(state);
	}

	~GameState();

protected:
	vector<GameState*> substates;
	bool isPaused = false;
	GameState();
};