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

Overworld* Overworld::Instance(int seed) {
	if (instance == 0) {
		instance = new Overworld();
		instance->world = new World(seed);
	}
	Game::mainCamera->setCorner(instance->lastCameraPos);
	return instance;
}

void Overworld::Init() {
	actionManager = ActionManager::Instance();

	lastCameraPos = vec2(0, 0);
	// TODO same with the world (this would likely just be a seed number, and maybe some other info)
	//int seed = rand();
}

void Overworld::Clean()
{
}

void Overworld::HandleEvents(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_BACKQUOTE) { // Tilde/Backquote key
			Game::ChangeState(Console::Instance(this));
			return;
		}
	}
	actionManager->HandleEvents(event);
}

void Overworld::Update()
{
	world->Update();
	actionManager->Update(game);
	Game::mainCamera->Update();
	lastCameraPos = Game::mainCamera->getPos();
}

void Overworld::Render(float interpolation)
{
	world->Render(interpolation);
	actionManager->Render(interpolation);
}