#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "Game.h"
#include "MainMenu.h"
#include "Overworld.h"
#include "Util.h"

Game *game = nullptr;

int main(int argc, char *argv[]) {
	const int TICKS_PER_SECOND = 30;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 10;

	Uint32 next_game_tick = SDL_GetTicks();
	int loops;
	float interpolation;

	game = Game::Instance();
	game->ChangeState(MainMenu::Instance()); //Start the game on the main menu gamestate

	//Main game loop
	while (game->Running()) {
		loops = 0;
		while (SDL_GetTicks() > next_game_tick && loops < MAX_FRAMESKIP) {
			game->HandleEvents();
			game->Update();

			next_game_tick += SKIP_TICKS;
			loops++;
		}

		interpolation = float(SDL_GetTicks() + SKIP_TICKS - next_game_tick)
			/ float(SKIP_TICKS);

		game->Render(interpolation);
	}

	game->Clean();
	delete game;
	return 0;
}