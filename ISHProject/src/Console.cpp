#include "Console.h"
#include "MainMenu.h"
#include "Overworld.h"
#include <sstream>
#include <iostream>

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

	/*
	This section is the addition of all of the functions that the console
	can recognize. The string that the function will use is the 'key' in
	the map. The function itself can be the name of an existing function
	so long as the return type is int and the argument is std::vector<string>
	or the function can be a lambda as seen below. Make sure to check for
	vector index OOB on the arguments list (or, we could add the appropriate
	catch statement in ParseCommand()).
	*/

	functions["hello"] = [](std::vector<string>, std::string &msg){
		msg = "hello there";
		return 0; //completed successfully
	};
	//changes state
	functions["state"] = [](std::vector<string> args, std::string &msg) {
		if (args.size() == 1) { //only needs the first argument
			if (args[0] == "menu") {
				Game::ChangeState(MainMenu::Instance());
				Game::ChangeState(Console::Instance(MainMenu::Instance()));
				msg = "switched to menu";
				return 0;//completed successfully
			}
			else if (args[0] == "overworld") {
				Game::ChangeState(Overworld::Instance());
				Game::ChangeState(Console::Instance(Overworld::Instance()));
				msg = "switched to overworld";
				return 0;//completed successfully
			}
		}
		msg = "usage: state [menu|overworld]";
		return 1;
	};

	functions["turnbased"] = [](std::vector<string> args, std::string &msg) {
		if (args.size() == 1) {
			if (args[0] == "true") {
				ActionManager::turnBased = true;
				msg = "turnbased = true";
				return 0;
			}
			else if (args[0] == "false") {
				ActionManager::turnBased = false;
				msg = "turnbased = false";
				return 0;
			}
		}
		msg = "usage: turnbased [bool]";
		return 1;
	};

	functions["help"] = [](std::vector<string>, std::string &msg) {
		msg = "commands: [hello|state]";
		return 0; //completed successfully
	};
	//TODO more functions
}

void Console::Clean()
{
}

void Console::HandleEvents(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		switch (event.key.keysym.sym) {
		case SDLK_BACKQUOTE:	//Change back to old state
			SDL_StopTextInput();
			Game::Instance()->ChangeState(oldState);
			return;
		case SDLK_RETURN:
			if (currentCommand.length() > 0) {
				commands.push_back(currentCommand);
				consoleOutput.push_back("> " + currentCommand);
				ParseCommand(currentCommand);
				commandIndex = 0;
				currentCommand = "";
			}
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

void Console::Update()
{
}

void Console::Render(float interpolation)
{
	oldState->Render(0);

	int INC = 25;
	int renderY = Game::Instance()->UISurface->clip_rect.h - (INC*2);

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

		SDL_BlitSurface(commandSurface, &commandSurface->clip_rect, Game::Instance()->UISurface, &r);
		SDL_FreeSurface(commandSurface);
	}

	SDL_Surface* cS = TTF_RenderText_Solid(consoleFont, ("> " + currentCommand).c_str(), fontColor);
	SDL_Rect r = SDL_Rect();
	r.x = 10;
	r.y = Game::Instance()->UISurface->clip_rect.h - INC;
	r.w = 100;
	r.h = INC;
	SDL_BlitSurface(cS, &cS->clip_rect, Game::Instance()->UISurface, &r);
	SDL_FreeSurface(cS);
}

void Console::ParseCommand(string command) {
	std::vector<string> args = split(currentCommand, ' ');
	if (args.size() > 0) {
		std::string cin = args[0];
		args.erase(args.begin());
		if (functions.find(cin) == functions.end()) {
			consoleOutput.push_back("Invalid command");
		}
		else {
			try {
				std::string msg = "Command Failure";
				int errorcode = (*functions[cin])(args, msg);
				if (errorcode != 0) {
					//msg = "Invalid arguments";
					//this functionality was replaced by making the message reflect
					//correct usage. However, the functions still return nonzero when
					//they fail, so that can be used for something if desired.
				}
				consoleOutput.push_back(msg);
			}
			catch (const std::exception& e) {
				std::cout << "fail: "<< std::endl;
			}
			//execute the function at that place in the map
		}
	}
}

std::vector<std::string> Console::split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}