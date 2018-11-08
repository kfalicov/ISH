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
	int seed = rand();
	world = new World(seed);
}

void Overworld::Clean()
{
}

void Overworld::HandleEvents(Game * game, SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_BACKQUOTE) { // Tilde/Backquote key
			Game::ChangeState(Console::Instance(this));
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

void Overworld::Render(float interpolation)
{
	world->Render(interpolation);
	actionManager->Render(interpolation);
}