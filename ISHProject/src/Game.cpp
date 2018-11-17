#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Game.h"
#include "GameState.h"

Game::Game() {
	//initialize SDL and then our stuff
	if (SDLInit("ISH", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false)) {
		activeState = new MenuState(this);
		isRunning = true;
	}
	else {
		//TODO some error message
		isRunning = false;
	}
}

Game::~Game() {
}

bool Game::SDLInit(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int fullscreenFlag = 0;
	if (fullscreen) { fullscreenFlag = SDL_WINDOW_FULLSCREEN; }

	//Initialize SDL2, SDL Image, and SDL ttf
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG && TTF_Init() != -1) {
		std::cout << "Subsystems initialized..." << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, fullscreenFlag);
		if (window) {
			std::cout << "Window created..." << std::endl;
		}
		else { return false; }
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Renderer created..." << std::endl;
		}
		else { return false; }
		return true;
	}
	return false;
}

void Game::HandleEvents() {
	//the list of all currently pressed keys and buttons
	SDL_Event event;
	SDL_PollEvent(&event);

	activeState->HandleEvents(event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::Update() {
	//activeState.Update();
}

void Game::Render(float interpolation) {
	SDL_RenderClear(renderer);

	int w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	SDL_Rect windowRect = SDL_Rect();
	windowRect.x = windowRect.y = 0;
	windowRect.w = w;
	windowRect.h = h;

	std::vector<SDL_Surface*> surfaces;
	activeState->Render(surfaces, interpolation);

	for (SDL_Surface* layer : surfaces) {
		SDL_Texture* layerTexture = SDL_CreateTextureFromSurface(renderer, layer);

		//Scale each layer accordingly to fit the window
		float widthRatio = (float)windowRect.w / layer->clip_rect.w;
		float heightRatio = (float)windowRect.h / layer->clip_rect.h;
		int scaleMultiplier = (int)floor((widthRatio < heightRatio) ? widthRatio : heightRatio);
		//Disable scaling (DEBUG)
		//scaleMultiplier = 1;
		SDL_Rect layerDestRect;
		layerDestRect.w = scaleMultiplier * layer->clip_rect.w;
		layerDestRect.h = scaleMultiplier * layer->clip_rect.h;
		layerDestRect.x = int((windowRect.w - layerDestRect.w) / 2.0);
		layerDestRect.y = int((windowRect.h - layerDestRect.h) / 2.0);

		SDL_LockSurface(layer);
		SDL_SetSurfaceRLE(layer, true);
		SDL_RenderCopy(renderer, layerTexture,
			&layer->clip_rect,
			&layerDestRect);
		SDL_SetSurfaceRLE(layer, false);
		SDL_UnlockSurface(layer);

		SDL_DestroyTexture(layerTexture);
	}

	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	
}

bool Game::Running() { return isRunning; }