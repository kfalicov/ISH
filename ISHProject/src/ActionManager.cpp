#include "ActionManager.h"
#include "Util.h"
#include "Camera.h"

ActionManager* ActionManager::instance;

ActionManager::ActionManager() {}

ActionManager::~ActionManager() {}

ActionManager* ActionManager::Instance()
{
	if (instance == 0) {
		instance = new ActionManager();
	}

	instance->turnBased = false;

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

void ActionManager::HandleEvents(Game* game, SDL_Event event)
{
	// TODO player movements and ActionManager::HandleEvents neater
	// Player should only be able to move if they are not currently moving
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_w) {
			//Move Player up
			player->Move(vec2::N);
			playerMoved = true;
		}
		else if (event.key.keysym.sym == SDLK_s) {
			player->Move(vec2::S);
			playerMoved = true;
		}		
		else if (event.key.keysym.sym == SDLK_a) {
			player->Move(vec2::W);
			playerMoved = true;
		}		
		else if (event.key.keysym.sym == SDLK_d) {
			player->Move(vec2::E);
			playerMoved = true;
		}
	}
}

void ActionManager::Update(Game* game)
{
	tickCounter++;
	for (std::vector<Entity*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		Entity e = *(*it);
		e.moveTicks++;
		if ((turnBased && playerMoved) ||
			(!turnBased && tickCounter % e.moveFreq == 0)) {
			e.moveTicks = 0;
			e.Move();
			playerMoved = false;
		}
	}
}

void ActionManager::Render(Game* game, float interpolation){
	for (std::vector<Entity*>::iterator it = actors.begin(); it != actors.end(); ++it) {
		(*it)->updateRenderPosition(interpolation);
		game->mainCamera->RenderSprite((*(*it)->sprite), (*it)->renderPos*PIXELS_PER_TILE);
	}
	player->updateRenderPosition(interpolation);
	game->mainCamera->RenderSprite((*player->sprite), player->renderPos*PIXELS_PER_TILE);
}

void ActionManager::Clean()
{
}
