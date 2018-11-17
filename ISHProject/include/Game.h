#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Util.h"
#include "GameState.h"

class GameState;

class Game
{
public:
	Game();
	~Game();

	void HandleEvents();
	void Update();
	void Render(float interpolation);
	void Clean();
	//sets the active state
	void setActiveState(GameState* state) { this->activeState = state; };

	bool Running();

	SDL_Renderer *renderer;
	SDL_Window *window;
	//AssetHandler generates sprites and reads from a file. AssetHandler needs to be given to the various generators
	/*AssetHandler *assetHandler;
	static Camera *mainCamera;
	SDL_Surface* UISurface;
	SDL_Rect camDestRect;
	float scaleMultiplier;
	vec2 camPosRelative;*/

	/*static const int TILE_SIZE = 16;*/

private:
	
	bool SDLInit(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	GameState* activeState; //activeState changes itself to the next state based on inputs it recieves

	bool isRunning;
};