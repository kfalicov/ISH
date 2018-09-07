#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class AssetHandler;
class GameState;
class Camera;

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
	AssetHandler *assetHandler;
	Camera *mainCamera;

	const int TILE_SIZE = 16;
private:
	GameState *activeState;
	bool isRunning;
};

