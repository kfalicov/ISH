#include "MainMenu.h"

MainMenu* MainMenu::instance;

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
	delete instance;
}

MainMenu* MainMenu::Instance() {
	if (instance == 0) {
		instance = new MainMenu();
	}
	return instance;
}

void MainMenu::Init() {

}

void MainMenu::Clean()
{
}

void MainMenu::HandleEvents(Game * game, SDL_Event event)
{
}

void MainMenu::Update(Game * game)
{
}

void MainMenu::Render(Game * game)
{
	SDL_Surface *surface = IMG_Load("Assets/loaded.png");
	SDL_Rect srcRect = surface->clip_rect;
	SDL_Rect destRect = surface->clip_rect;
	destRect.h = 100;
	destRect.w = 100;
	SDL_Texture *S = SDL_CreateTextureFromSurface(game->renderer, surface);
	SDL_RenderCopy(game->renderer, S, &srcRect, &destRect);
	SDL_DestroyTexture(S);
	SDL_FreeSurface(surface);
}