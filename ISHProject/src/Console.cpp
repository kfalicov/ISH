#include "Console.h"
#include <SDL_ttf.h>

Console* Console::instance;

Console::Console()
{
	Init();
}

Console::~Console()
{
	delete instance;
}

Console* Console::Instance(GameState* state) {
	if (instance == 0) {
		instance = new Console();
	}
	instance->oldState = state;
	SDL_StartTextInput();
	return instance;
}

void Console::Init() {
}

void Console::Clean()
{
}

void Console::HandleEvents(Game* game, SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {
		if(event.key.keysym.sym == SDLK_BACKQUOTE) { //Change back to old state
			SDL_StopTextInput();
			game->ChangeState(oldState);
		}
		else if (event.key.keysym.sym == SDLK_RETURN) {
			ParseCommand(currentCommand);
			commands.push_back(currentCommand);
			currentCommand = "";
		}
		else if (event.key.keysym.sym == SDLK_BACKSPACE) {
			if (currentCommand.length() > 0) {
				currentCommand.pop_back();
			}
		}
	}

	if (event.type == SDL_TEXTINPUT) {
		currentCommand += event.text.text;
	}
}

void Console::Update(Game *game)
{
}

void Console::Render(Game *game, float interpolation)
{
	oldState->Render(game, interpolation);

	TTF_Font* consoleFont = TTF_OpenFont("Assets/opensans.ttf", 20);
	SDL_Color fontColor = { 255, 255, 255 };
	int INC = 25;
	int renderY = game->UISurface->clip_rect.h - (INC*2);
	for (std::vector<string>::reverse_iterator it = commands.rbegin(); it != commands.rend(); ++it) {
		SDL_Surface* commandSurface = TTF_RenderText_Solid(consoleFont, (*it).c_str(), fontColor);
		SDL_Rect r = SDL_Rect();
		r.x = 10;
		r.y = renderY;
		r.w = 100;
		r.h = INC;

		renderY -= INC;

		SDL_BlitSurface(commandSurface, &commandSurface->clip_rect, game->UISurface, &r);
		SDL_FreeSurface(commandSurface);
	}

	SDL_Surface* cS = TTF_RenderText_Solid(consoleFont, currentCommand.c_str(), fontColor);
	SDL_Rect r = SDL_Rect();
	r.x = 10;
	r.y = game->UISurface->clip_rect.h - INC;
	r.w = 100;
	r.h = INC;
	SDL_BlitSurface(cS, &cS->clip_rect, game->UISurface, &r);
	SDL_FreeSurface(cS);
	TTF_CloseFont(consoleFont);
	consoleFont = NULL;
}

void Console::ParseCommand(string command) {

}