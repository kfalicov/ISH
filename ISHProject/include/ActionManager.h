#pragma once
#include <vector>
#include "Entity.h"
#include "Game.h"
#include "Player.h"

class ActionManager {
public:
	static ActionManager* Instance();

	void Subscribe(Entity* e);
	void unSubscribe(Entity* e);

	// TODO initialize player
	//void setPlayer(Entity* p);

	void HandleEvents(Game* game, SDL_Event event);
	void Update(Game* game);
	void Render(Game* game, float interpolation);
	void Clean();

	bool turnBased = false;;
	bool playerMoved = false;

	Player* player;

	~ActionManager();
private:
	ActionManager();
	static ActionManager* instance;
	int tickCounter = 0;

	std::vector<Entity*> actors;
};