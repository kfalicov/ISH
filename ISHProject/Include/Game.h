#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Game
{
public:
	Game();
	~Game();

	void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool Running();

	void SetFPS(int fps);
	int GetFPS();

	SDL_Renderer *renderer;
	SDL_Window *window;

private:
	bool isRunning;
	int framerate;
};

