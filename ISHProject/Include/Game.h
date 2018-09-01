#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class GameState;

class Game
{
public:
	Game();
	~Game();

	void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void ChangeState(GameState *state);

	void HandleEvents();
	void Update();
	void Render(float interpolation);
	void Clean();

	bool Running();

	SDL_Renderer *renderer;
	SDL_Window *window;

private:
	GameState *activeState;
	bool isRunning;
};

