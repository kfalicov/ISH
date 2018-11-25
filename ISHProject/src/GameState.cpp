#include "GameState.h"

GameState::GameState() {
	//TODO each state can have a different sized surface. For example, PlayState will have
	//a square surface to render the environment.
	initializeRenderSurface(800, 600);
	assetHandler = new AssetHandler();
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

MenuState::MenuState(GameState* previous)
{
	this->previous = previous;
}

MenuState::~MenuState(){}


GameState* MenuState::Update(SDL_Event event){
	if (event.type == SDL_KEYDOWN) {
		std::cout << "Changing from menu" << std::endl;
		return new PlayState(this);
	}
	return this;
}

SDL_Surface * MenuState::RenderLayers(float interpolation)
{
	return nullptr;
}


PlayState::PlayState(GameState* previous)
{
	this->previous = previous;
}

PlayState::~PlayState()
{
	
}

GameState* PlayState::Update(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {
		std::cout << "Changing from play" << std::endl;
		return new MenuState(this);
	}
	if (assetHandler->Update()) {
		//assetHandler->UpdateSpriteFrames(LIST OF SPRITES IN PLAYSTATE);
		//assetHandler->UpdateEntityAnimations(LIST OF ENTITIES IN PLAYSTATE);
	}
	return this;
}

SDL_Surface* PlayState::RenderLayers(float interpolation)
{
	//Render World (background, items, entities) and return their combined surface.
	return nullptr;
}