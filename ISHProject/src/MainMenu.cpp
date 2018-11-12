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
	Game::mainCamera->setCorner(vec2(0, 0));
	return instance;
}

void MainMenu::Init() {
	Button* b = new Button(vec2(128,64), vec2(16,16));
	b->sprite = AssetHandler::Instance()->GetSprite("Assets/Temps.png",
		AssetHandler::Temps::MEDIUM);
	b->setCallback(
		[]() {Game::ChangeState(Overworld::Instance()); } //bro this lambda is elegant af
		// ^ the [] represents the void function with () as empty params.
		// the actual function called is in the parenthesis
	);
	menuitems.push_back(b);

	spr = AssetHandler::Instance()->GetSprite("Assets/AnimTest.png", AssetHandler::Tiles::GRASS);
	spr->setRectsFromIndices(16, 16, { 0,1,2,3 });

}

void MainMenu::Clean()
{
}

void MainMenu::HandleEvents(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {
		if (event.key.keysym.sym == SDLK_BACKQUOTE) { // Tilde/Backquote key
			Game::ChangeState(Console::Instance(this));
			return;
		}
	}
	if (event.type == SDL_MOUSEBUTTONDOWN) {
		if (event.button.button == SDL_BUTTON_LEFT) {
			vec2 mpos = vec2(event.button.x, event.button.y)+Game::mainCamera->getPos()-Game::Instance()->camPosRelative;// +Game::mainCamera->getPos();
			mpos /= Game::Instance()->scaleMultiplier; //converts the mouse in window-space into pixel-space to align with buttons in the menu
			std::cout << "clicked at " << mpos << std::endl;
			for (auto e : menuitems) {
				if (mpos[0]> e->pos[0] && mpos[0] < (e->pos[0]+e->size[0]) &&
					mpos[1]> e->pos[1] && mpos[1] < (e->pos[1] + e->size[1])) {
					//Button* item = dynamic_cast<Button*>(e);
					e->click();
				}
			}
		}
	}
}

void MainMenu::Update()
{
	
}

void MainMenu::Render(float interpolation)
{
	for (auto e : menuitems) {
		//cout << e->renderpos() << endl;
		Game::mainCamera->RenderSprite(*(e->sprite), e->renderpos());
	}
	
}