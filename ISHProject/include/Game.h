#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Util.h"

class AssetHandler;
class GameState;
class Camera;

class Game
{
public:
	static Game* Instance();
	~Game();

	void Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void HandleEvents();
	void Update();
	void Render(float interpolation);
	void Clean();

	bool Running();

	SDL_Renderer *renderer;
	SDL_Window *window;
	AssetHandler *assetHandler;
	static Camera *mainCamera;
	SDL_Surface* UISurface;
	SDL_Rect camDestRect;
	float scaleMultiplier;
	vec2 camPosRelative;

	static const int TILE_SIZE = 16;

	static void ChangeState(GameState* state);

private:
	static Game* instance;
	Game();
	static GameState* activeState;
	bool isRunning;
};