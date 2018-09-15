#include "Overworld.h"
#include "Sprite.h"
#include "AssetHandler.h"
#include "Camera.h"
#include "Util.h"

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
}

void Overworld::Clean()
{
}

void Overworld::HandleEvents(Game * game, SDL_Event event)
{
	actionManager->HandleEvents(game, event);
}

void Overworld::Update(Game *game)
{
	actionManager->Update(game);
}

void Overworld::Render(Game *game, float interpolation)
{
	Sprite* spr = game->assetHandler->GetSprite("Assets/AnimTest.png", AssetHandler::Tiles::GRASS);
	vec2 pos = vec2(0, 0);
	game->mainCamera->RenderSprite((*spr), pos);
}