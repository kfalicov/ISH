#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "../Include/Game.h"

Game *game = nullptr;

void cap_framerate(Uint32 starting_tick) {
	Uint32 current_tick = SDL_GetTicks();
	if ((1000 / game->GetFPS()) > current_tick - starting_tick) {
		SDL_Delay(1000 / game->GetFPS() - (current_tick - starting_tick));
	}
}

int main(int argc, char *argv[]) {
	Uint32 starting_tick;

	game = new Game();
	game->Init("ISH", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);
	game->SetFPS(30);

	while (game->Running()) {
		starting_tick = SDL_GetTicks();

		game->HandleEvents();
		game->Update();
		game->Render();

		cap_framerate(starting_tick);
	}

	game->Clean();
	delete game;
	return 0;
}