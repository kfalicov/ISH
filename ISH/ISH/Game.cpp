#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>

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

		framerate = 60;
		isRunning = true;
	}
	else {
		isRunning = false;
	}
}

void Game::HandleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::Update() {
}

void Game::Render() {
	SDL_RenderClear(renderer);
	
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, 0, 300, 800, 300);
	SDL_RenderDrawLine(renderer, 400, 0, 400, 600);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_Surface *surface = IMG_Load("loaded.png");
	SDL_Rect srcRect = surface->clip_rect;
	SDL_Rect destRect = surface->clip_rect;
	destRect.h = 100;
	destRect.w = 100;
	SDL_Texture *S = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_RenderCopy(renderer, S, &srcRect, &destRect);
	SDL_DestroyTexture(S);
	SDL_FreeSurface(surface);

	SDL_RenderPresent(renderer);
}

void Game::Clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned..." << std::endl;
}

bool Game::Running() { return isRunning; }

void Game::SetFPS(int fps) { framerate = fps; }
int Game::GetFPS() { return framerate; }
