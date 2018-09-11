#pragma once
#include <vector>
#include "Entity.h"
#include "Game.h"

class ActionManager {
public:
	static ActionManager* Instance();

	void Subscribe(Entity* e);
	void unSubscribe(Entity* e);

	// TODO initialize player
	//void setPlayer(Entity* p);

	void HandleEvents(Game* game, SDL_Event event);
	void Update(Game* game);
	void Clean();

	bool turnBased;
	bool playerMoved;
	const float TICKS_PER_MOVE = 15;

	~ActionManager();
private:
	ActionManager();
	static ActionManager* instance;

	Entity* player;
	int tickCounter;

	std::vector<Entity*> actors;
};