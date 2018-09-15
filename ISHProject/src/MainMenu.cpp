#include "MainMenu.h"
#include "Sprite.h"
#include "AssetHandler.h"
#include "Camera.h"
#include "Console.h"
#include "Util.h"

MainMenu* MainMenu::instance;

MainMenu::MainMenu()
{
	Init();
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
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_BACKQUOTE) { // Tilde/Backquote key
			ChangeState(game, Console::Instance(this));
			return;
		}
	}
}

void MainMenu::Update(Game *game)
{
}

void MainMenu::Render(Game *game, float interpolation)
{
	Sprite* spr = game->assetHandler->GetSprite("Assets/AnimTest.png", AssetHandler::Tiles::GRASS);
	vec2 pos = vec2(0, 0);
	game->mainCamera->RenderSprite((*spr), pos);
}