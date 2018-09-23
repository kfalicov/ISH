#include "Console.h"

Console* Console::instance;

Console::Console()
{
	Init();
}

Console::~Console()
{
	TTF_CloseFont(consoleFont);
	consoleFont = NULL;
	delete instance;
}

Console* Console::Instance(GameState* state) {
	if (instance == 0) {
		instance = new Console();
	}
	instance->oldState = state;
	instance->currentCommand = "";
	SDL_StartTextInput();
	return instance;
}

void Console::Init() {
	consoleFont = TTF_OpenFont("Assets/opensans.ttf", 20);
}

void Console::Clean()
{
}

void Console::HandleEvents(Game* game, SDL_Event event)
{
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
		case SDLK_BACKQUOTE:	//Change back to old state
			SDL_StopTextInput();
			game->ChangeState(oldState);
			return;
		case SDLK_RETURN:
			commands.push_back(currentCommand);
			consoleOutput.push_back(currentCommand);
			ParseCommand(currentCommand);
			commandIndex = 0;
			currentCommand = "";
			break;
		case SDLK_BACKSPACE:
			if (currentCommand.length() > 0) {
				currentCommand.pop_back();
			}
			break;
		case SDLK_UP:
			if (commandIndex < commands.size()) {
				commandIndex++;
			}
			if (commandIndex != 0) {
				for (std::vector<string>::reverse_iterator it = commands.rbegin(); it != commands.rbegin() + commandIndex; ++it) {
					currentCommand = (*it);
				}
			}
			break;
		case SDLK_DOWN:
			if (commandIndex > 0) {
				commandIndex--;
			}
			if (commandIndex != 0) {
				for (std::vector<string>::reverse_iterator it = commands.rbegin(); it != commands.rbegin() + commandIndex; ++it) {
					currentCommand = (*it);
				}
			}
			else {
				currentCommand = "";
			}
			break;
		}
	}

	if (event.type == SDL_TEXTINPUT) {
		if (event.text.text[0] != '`') {
			currentCommand += event.text.text;
		}
	}
}

void Console::Update(Game *game)
{
}

void Console::Render(Game *game, float interpolation)
{
	oldState->Render(game, 0);

	int INC = 25;
	int renderY = game->UISurface->clip_rect.h - (INC*2);

	int printCount = 0;
	for (std::vector<string>::reverse_iterator it = consoleOutput.rbegin(); it != consoleOutput.rend(); ++it) {
		printCount++;
		if (printCount > 5) {
			break;
		}
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

	SDL_Surface* cS = TTF_RenderText_Solid(consoleFont, ("> " + currentCommand).c_str(), fontColor);
	SDL_Rect r = SDL_Rect();
	r.x = 10;
	r.y = game->UISurface->clip_rect.h - INC;
	r.w = 100;
	r.h = INC;
	SDL_BlitSurface(cS, &cS->clip_rect, game->UISurface, &r);
	SDL_FreeSurface(cS);
}

void Console::ParseCommand(string command) {

}