#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Game.h"
#include "GameState.h"
#include "Console.h"
#include "AssetHandler.h"
#include "Camera.h"

Game::Game() {
}

Game::~Game() {
}

void Game::Init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen) {
	int fullscreenFlag = 0;
	if (fullscreen) { fullscreenFlag = SDL_WINDOW_FULLSCREEN; }

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0 && IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG && TTF_Init() != -1) {
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
		mainCamera = new Camera(this, vec2(0,0), 256, 256);
		UISurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
		//Treat solid black as transparent for the UI.
		SDL_SetColorKey(UISurface, SDL_TRUE, SDL_MapRGBA(UISurface->format, 0, 0, 0, 0));
		isRunning = true;
		consoleActive = false;
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
	SDL_FillRect(mainCamera->cameraSurface, &mainCamera->cameraSurface->clip_rect,
		SDL_MapRGBA(mainCamera->cameraSurface->format, 0,0,0,0));
	SDL_FillRect(UISurface, &UISurface->clip_rect,
		SDL_MapRGBA(UISurface->format, 0,0,0,0));
	int w;
	int h;
	SDL_GetRendererOutputSize(renderer, &w, &h);

	activeState->Render(this, interpolation);

	SDL_Texture *camTex = SDL_CreateTextureFromSurface(renderer, mainCamera->cameraSurface);
	SDL_Rect windowRect = SDL_Rect();
	windowRect.x = windowRect.y = 0;
	windowRect.w = w;
	windowRect.h = h;

	float widthRatio = (float)windowRect.w / mainCamera->cameraSurface->clip_rect.w;
	float heightRatio = (float)windowRect.h / mainCamera->cameraSurface->clip_rect.h;
	float scaleMultiplier = (widthRatio < heightRatio) ? widthRatio : heightRatio;

	SDL_Rect camDestRect = SDL_Rect();
	camDestRect.w = scaleMultiplier * mainCamera->cameraSurface->clip_rect.w;
	camDestRect.h = scaleMultiplier * mainCamera->cameraSurface->clip_rect.h;
	camDestRect.x = (windowRect.w - camDestRect.w) / 2.0;
	camDestRect.y = (windowRect.h - camDestRect.h) / 2.0;

	//windowRect = mainCamera->cameraSurface->clip_rect; //Debug camera surface dimensions
	SDL_RenderCopy(renderer, camTex,
		&mainCamera->cameraSurface->clip_rect,
		&camDestRect);

	//Render UI
	SDL_Texture* UItex = SDL_CreateTextureFromSurface(renderer, UISurface);
	// UI is the same size as the window by default
	SDL_RenderCopy(renderer, UItex, &UISurface->clip_rect, &UISurface->clip_rect);

	//Debug Lines, Camera Bounding Box
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	//Top
	SDL_RenderDrawLine(renderer, camDestRect.x, camDestRect.y,
									camDestRect.x + camDestRect.w, camDestRect.y);
	//Left
	SDL_RenderDrawLine(renderer, camDestRect.x, camDestRect.y,
									camDestRect.x, camDestRect.y + camDestRect.h);
	//Right
	SDL_RenderDrawLine(renderer, camDestRect.x + camDestRect.w - 1, camDestRect.y,
									camDestRect.x + camDestRect.w - 1, camDestRect.y + camDestRect.h);
	//Bottom
	SDL_RenderDrawLine(renderer, camDestRect.x, camDestRect.y + camDestRect.h - 1,
									camDestRect.x + camDestRect.w, camDestRect.y + camDestRect.h - 1);
	SDL_SetRenderDrawColor(renderer, 255, 105, 180, 255);
	//Vertical Mid
	SDL_RenderDrawLine(renderer, camDestRect.x + camDestRect.w / 2, camDestRect.y,
		camDestRect.x + camDestRect.w / 2, camDestRect.y + camDestRect.h);
	//Horizontal Mid
	SDL_RenderDrawLine(renderer, camDestRect.x, camDestRect.y + camDestRect.h / 2,
		camDestRect.x + camDestRect.w, camDestRect.y + camDestRect.h / 2);

	//Window Center
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawLine(renderer, 0, windowRect.h / 2, windowRect.w, windowRect.h / 2);
	SDL_RenderDrawLine(renderer, windowRect.w / 2, 0, windowRect.w / 2, windowRect.h);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	SDL_RenderPresent(renderer);
	SDL_DestroyTexture(camTex);
	SDL_DestroyTexture(UItex);
}

void Game::Clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Cleaned..." << std::endl;
}

bool Game::Running() { return isRunning; }
