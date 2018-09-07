#include "MainMenu.h"
#include "Sprite.h"
#include "AssetHandler.h"
#include "Camera.h"
#include "Util.h"

MainMenu* MainMenu::instance;

MainMenu::MainMenu()
{
}

MainMenu::~MainMenu()
{
	delete instance;
}

MainMenu* MainMenu::Instance() {
	if (instance == 0) {
		instance = new MainMenu();
	}
	return instance;
}

void MainMenu::Init() {

}

void MainMenu::Clean()
{
}

void MainMenu::HandleEvents(Game * game, SDL_Event event)
{
}

void MainMenu::Update(Game *game)
{
}

void MainMenu::Render(Game *game)
{
	Sprite* spr = game->assetHandler->GetSprite("Assets/AnimTest.png", AssetHandler::Tiles::GRASS);
	vec2 pos = vec2(0, 0);
	game->mainCamera->RenderSprite((*spr), pos);
}