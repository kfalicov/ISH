#include "GameState.h"

GameState::GameState() {
	//TODO each state can have a different sized surface. For example, PlayState will have
	//a square surface to render the environment.
	initializeRenderSurface(800, 600);
}

void GameState::Render(std::vector<SDL_Surface*> &surfaces, float interpolation, bool forceReRender) {
	if (previous) {
		previous->Render(surfaces, interpolation);
	}

	if (needsRender || forceReRender) {
		//Clear the surface in order to be rendered again
		SDL_FillRect(surface, &surface->clip_rect,
			SDL_MapRGBA(surface->format, 0, 0, 0, 0));
		surface = RenderLayers(interpolation);
	}

	surfaces.push_back(surface);
}

MenuState::MenuState(Game* game, GameState* previous)
{
	this->game = game;
	this->previous = previous;
}

MenuState::~MenuState(){}

void MenuState::HandleEvents(SDL_Event event){
	if (event.type == SDL_KEYDOWN) {
		std::cout << "Changing from menu" << std::endl;
		game->setActiveState(new PlayState(game, this));
	}
}

void MenuState::Update(){
}

SDL_Surface * MenuState::RenderLayers(float interpolation)
{
	return nullptr;
}


PlayState::PlayState(Game* game, GameState* previous)
{
	this->game = game;
	this->previous = previous;
}

PlayState::~PlayState()
{
	
}

void PlayState::HandleEvents(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		std::cout << "Changing from play" << std::endl;
		game->setActiveState(new MenuState(game, this));
	}
}

void PlayState::Update()
{
	//std::cout << "On PlayState" << std::endl;
}

SDL_Surface* PlayState::RenderLayers(float interpolation)
{
	//Render World (background, items, entities) and return their combined surface.
	return nullptr;
}