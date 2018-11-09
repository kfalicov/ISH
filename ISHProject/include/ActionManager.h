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

	void HandleEvents(SDL_Event event);
	void Update();
	void Render(float interpolation);
	void Clean();

	static bool turnBased;
	bool playerMoved;

	Player* player;
	std::vector<Entity*> actors;

	~ActionManager();
private:
	ActionManager();
	static ActionManager* instance;
	int tickCounter = 0;

	const Uint8* keystates = SDL_GetKeyboardState(NULL);
};