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
	Button* b = new Button(vec2(20,50), vec2(50,50), vec2(0,0));
	b->setCallback(
		[]() {Game::ChangeState(Overworld::Instance()); } //bro this lambda is elegant af
		// ^ the [] represents the void function with () as empty params.
		// the actual function called is in the parenthesis
	);
	menuitems.push_back(b);
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
	}
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			vec2 mpos = vec2(event.button.x, event.button.y);// +Game::mainCamera->getPos();
			std::cout << "clicked at " << mpos << std::endl;
			for (auto e : menuitems) {
				if (mpos[0]> e->pos[0] && mpos[0] < (e->pos[0]+e->size[0]) &&
					mpos[1]> e->pos[1] && mpos[1] < (e->pos[1] + e->size[1])) {
					Button* item = dynamic_cast<Button*>(e);
					if (item != 0) {
						//item->click();
					}
				}
			}
		}
	}
}

void MainMenu::Update(Game *game)
{
	
}

void MainMenu::Render(float interpolation)
{
	Sprite* spr = AssetHandler::GetSprite("Assets/AnimTest.png", AssetHandler::Tiles::GRASS);
	vec2 pos = vec2(0, 0);
	Game::Instance()->mainCamera->RenderSprite((*spr), pos);
	for (auto e : menuitems) {
		Game::Instance()->mainCamera->RenderSprite(*spr, e->renderpos()+Game::mainCamera->getPos());
	}
	
}