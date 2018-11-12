#pragma once
#include "ActionManager.h"
#include "Util.h"
#include "Camera.h"

ActionManager* ActionManager::instance;
bool ActionManager::turnBased;

ActionManager::ActionManager() {}

ActionManager::~ActionManager() {}

ActionManager* ActionManager::Instance()
{
	if (instance == 0) {
		instance = new ActionManager();
		instance->turnBased = true;
		instance->playerMoved = false;
	}
	return instance;
}

void ActionManager::Subscribe(Entity* e)
{
	actors.push_back(e);
}

void ActionManager::unSubscribe(Entity* e)
{
	for (std::vector<Entity*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		if ((*it) == e) {
			actors.erase(it);
			return;
		}
	}
}

void ActionManager::HandleEvents(SDL_Event event)
{
	// TODO player movements and ActionManager::HandleEvents neater
	// Player should only be able to move if they are not currently moving
	//SDL_PumpEvents();
	if (player->canMove) {
		if (keystates[SDL_SCANCODE_W]) {
			//Move Player up
			player->Move(vec2::N);
			player->canMove = false;
			player->moveTicks = 0;
			playerMoved = true;
		}
		else if (keystates[SDL_SCANCODE_S]) {
			player->Move(vec2::S);
			player->canMove = false;
			player->moveTicks = 0;
			playerMoved = true;
		}
		else if (keystates[SDL_SCANCODE_A]) {
			player->Move(vec2::W);
			player->canMove = false;
			player->moveTicks = 0;
			playerMoved = true;
		}
		else if (keystates[SDL_SCANCODE_D]) {
			player->Move(vec2::E);
			player->canMove = false;
			player->moveTicks = 0;
			playerMoved = true;
		}
		else if (keystates[SDL_SCANCODE_R] && event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			player->Rummage();
			player->canMove = false;
		}
		else if (keystates[SDL_SCANCODE_E] && event.type == SDL_KEYDOWN && event.key.repeat == 0) {
			player->Equip();
			player->canMove = false;
		}
		else if (keystates[SDL_SCANCODE_SPACE]) {
			player->Attack();
			player->canMove = false;
			player->moveTicks = 0;
			playerMoved = true;
		}
		else {
			//player->Move(vec2(0, 0));
			player->new_animation = player->idle;
			playerMoved = false;
		}
	}
	else if(player->moveTicks <= player->moveFreq) {
		player->moveTicks++;
	}
	else {
		player->canMove = true;
	}
}

void ActionManager::Update()
{
	tickCounter++;
	for (std::vector<Entity*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		Entity* e = (*it);
		if ((turnBased && playerMoved) || !turnBased) {
			if (e->currentChunk != nullptr && e->canMove) {
				e->canMove = false;
				e->moveTicks = 0;
				e->Move();
			}
			else if (e->moveTicks <= e->moveFreq) {
				e->moveTicks++;
			}
			else {
				e->canMove = true;
			}
		}
	}
}

void ActionManager::Render(float interpolation){
	for (std::vector<Entity*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		(*it)->updateRenderPosition(interpolation);
		Game::Instance()->mainCamera->RenderSprite((*(*it)->sprite), (*it)->renderPos*PIXELS_PER_TILE);
	}
	player->updateRenderPosition(interpolation);
	Game::Instance()->mainCamera->RenderSprite((*player->sprite), player->renderPos*PIXELS_PER_TILE);
}

void ActionManager::Clean()
{
}