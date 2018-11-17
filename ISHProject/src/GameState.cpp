#include "GameState.h"

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