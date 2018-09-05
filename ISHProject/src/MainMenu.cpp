#include "MainMenu.h"
#include "Sprite.h"
#include "AssetHandler.h"

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
	Sprite *spr = game->assetHandler->GetSprite("Assets/AnimTest.png", AssetHandler::Tiles::GRASS);
	int pos[2] = { 200, 200 };
	game->assetHandler->RenderSprite(spr, pos);

	Sprite *spr2 = game->assetHandler->GetSprite("Assets/AnimTest.png", AssetHandler::Tiles::SAND);
	int pos2[2] = { 300, 200 };
	game->assetHandler->RenderSprite(spr2, pos2);
}