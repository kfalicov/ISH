#pragma once
#include <vector>
#include <SDL.h>
#include <iostream>

#include "Game.h"

class Game;

class GameState
{
public:
	virtual void HandleEvents(SDL_Event event) {};
	virtual void Update() {};
	virtual SDL_Surface* Render(float interpolation, bool forceReRender = false) { return nullptr; };
	
	~GameState() {
	};
	GameState() {};

protected:
	//the last gamestate you were in, for switching purposes
	GameState* previous;
	
	Game* game;
	
	//an internal storage so that rerendering doesn't have to happen constantly
	SDL_Surface* surface;
	// whether the surface has been updated visually and needs to be re-written to a surface
	bool needsRender;
	
};

class MenuState : public GameState
{
public:
	MenuState() {};
	MenuState(Game* game, GameState* previous = nullptr);
	~MenuState();

	void HandleEvents(SDL_Event event) override;
	void Update() override;
private:
	//std::vector<Element*> menuitems;
};

class PlayState : public GameState
{
public:
	PlayState() {};
	PlayState(Game* game, GameState* previous = nullptr);
	~PlayState();

	void HandleEvents(SDL_Event event) override;
	void Update() override;
private:
};