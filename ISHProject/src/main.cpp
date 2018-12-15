#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "Game.h"

int main(int argc, char *argv[]) {
	const int TICKS_PER_SECOND = 30;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 10;

	Uint32 next_game_tick = SDL_GetTicks();
	int loops;
	float interpolation;

	Game* game = new Game();

	//Main game loop
	while (game->Running()) {
		loops = 0;
		while (next_game_tick < SDL_GetTicks() && loops < MAX_FRAMESKIP) {
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