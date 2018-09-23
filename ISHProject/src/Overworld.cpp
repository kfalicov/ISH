#include "Overworld.h"
#include "Sprite.h"
#include "AssetHandler.h"
#include "Camera.h"
#include "Util.h"
#include "Console.h"

Overworld* Overworld::instance;

Overworld::Overworld()
{
	Init();
}

Overworld::~Overworld()
{
	delete instance;
}

Overworld* Overworld::Instance() {
	if (instance == 0) {
		instance = new Overworld();
	}
	return instance;
}

void Overworld::Init() {
	actionManager = ActionManager::Instance();

	// TODO same with the world (this would likely just be a seed number, and maybe some other info)
	//int seed = rand();
	int seed = 1337;
	world = new World(seed);
}

void Overworld::Clean()
{
}

void Overworld::HandleEvents(Game * game, SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_BACKQUOTE) { // Tilde/Backquote key
			ChangeState(game, Console::Instance(this));
			return;
		}
	}
	actionManager->HandleEvents(game, event);
}

void Overworld::Update(Game *game)
{
	world->Update(game);
	actionManager->Update(game);
}

void Overworld::Render(Game *game, float interpolation)
{
	world->Render(game, interpolation);
	actionManager->Render(game, interpolation);
}