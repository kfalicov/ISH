#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"
#include "GameState.h"
#include "AssetHandler.h"

Game::Game() {
}

Game::~Game() {
}

void Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int fullscreenFlag = 0;
	if (fullscreen) { fullscreenFlag = SDL_WINDOW_FULLSCREEN; }

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
		std::cout << "Subsystems initialized..." << std::endl;
		window = SDL_CreateWindow(title, xpos, ypos, width, height, fullscreenFlag);
		if (window) {
			std::cout << "Window created..." << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			std::cout << "Renderer created..." << std::endl;
		}

		assetHandler = AssetHandler::Instance();
		assetHandler->Init(this);
		isRunning = true;
	}
	else {
		isRunning = false;
	}
}

void Game::ChangeState(GameState *state) {
	activeState = state;
	std::cout << "Changing GameState to " << state->getName() << std::endl;
}

void Game::HandleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);

	activeState->HandleEvents(this, event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::Update() {
	activeState->Update(this);
}

void Game::Render(float interpolation) {
	SDL_RenderClear(renderer);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, 0, 300, 800, 300);
	SDL_RenderDrawLine(renderer, 400, 0, 400, 600);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	activeState->Render(this);

	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned..." << std::endl;
}

bool Game::Running() { return isRunning; }
