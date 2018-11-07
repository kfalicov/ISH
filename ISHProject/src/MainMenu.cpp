#include "MainMenu.h"
#include "Sprite.h"
#include "AssetHandler.h"
#include "Camera.h"
#include "Console.h"
#include "Overworld.h"
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
	b = new Button(vec2(0,0), vec2(0,0), vec2(0,0));
	b->setCallback(
		[]() {Game::ChangeState(Overworld::Instance()); } //bro this lambda is elegant af
		// ^ the [] represents the void function with () as empty params.
		// the actual function called is in the parenthesis
	);
}

void MainMenu::Clean()
{
}

void MainMenu::HandleEvents(Game * game, SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_BACKQUOTE) { // Tilde/Backquote key
			Game::ChangeState(Console::Instance(this));
			return;
		}
		else {
			b->click();
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