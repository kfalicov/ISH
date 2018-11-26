#pragma once
#include <iostream>
#include <iomanip>
#include <sstream>
#include "AssetHandler.h"
#include "GameState.h"

void GameState::Render(std::vector<SDL_Surface*> &surfaces, float interpolation, bool forceReRender) {
	if (previous) {
		previous->Render(surfaces, interpolation);
	}

	if (needsRender || forceReRender) {
		//Clear the surface in order to be rendered again
		SDL_FillRect(surface, &surface->clip_rect,
			SDL_MapRGBA(surface->format, 0, 0, 0, 0));
		surface = RenderLayers(interpolation);
	}

	surfaces.push_back(surface);
}

/*

	MENU GAMESTATE

*/

MenuState::MenuState(AssetHandler* assetHandler, GameState* previous)
	: GameState(assetHandler)
{
	this->previous = previous;
}

MenuState::~MenuState(){}


GameState* MenuState::Update(SDL_Event event){
	return this;
}

SDL_Surface * MenuState::RenderLayers(float interpolation)
{
	return nullptr;
}

/*

	PLAY/WORLD GAMESTATE

*/

PlayState::PlayState(AssetHandler* assetHandler, GameState* previous)
	: GameState(assetHandler)
{
	this->previous = previous;
}

PlayState::~PlayState()
{
	
}

GameState* PlayState::Update(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {
		std::cout << "Changing from play" << std::endl;
		return new MenuState(assetHandler, this);
	}
	if (assetHandler->Update()) {
		//assetHandler->UpdateSpriteFrames(LIST OF SPRITES IN PLAYSTATE);
		//assetHandler->UpdateEntityAnimations(LIST OF ENTITIES IN PLAYSTATE);
	}
	return this;
}

SDL_Surface* PlayState::RenderLayers(float interpolation)
{
	//Render World (background, items, entities) and return their combined surface.
	return nullptr;
}

/*

	CONSOLE GAMESTATE

*/

ConsoleState::ConsoleState(AssetHandler* assetHandler, GameState* previous)
	: GameState(assetHandler)
{
	this->previous = previous;
	this->canType = false;
	SDL_StartTextInput();
	/*
	This section is the addition of all of the functions that the console
	can recognize. The string that the function will use is the 'key' in
	the map. The function itself can be the name of an existing function
	so long as the return type is int and the argument is std::vector<string>
	or the function can be a lambda as seen below. Make sure to check for
	vector index OOB on the arguments list (or, we could add the appropriate
	catch statement in ParseCommand()).
	*/

	functions["hello"] = [](std::vector<std::string>, std::string &msg) {
		msg = "hello there";
		std::cout << "\r\ntest command execution";
		return 0; //completed successfully
	};
	functions["help"] = [](std::vector<std::string>, std::string &msg) {
		msg = "commands: [hello|state]";
		return 0; //completed successfully
	};
	//TODO more functions
}

SDL_Surface * ConsoleState::RenderLayers(float interpolation)
{
	return nullptr;
}

ConsoleState::~ConsoleState()
{
}

GameState * ConsoleState::Update(SDL_Event event)
{
	switch (event.type) {
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_BACKSPACE:
			if (currentCommand.length() > 0) { currentCommand.pop_back(); }
			break;
		case SDLK_RETURN:
			commands.push_back(currentCommand);
			consoleOutput.push_back("> " + currentCommand);
			commandIndex = commands.size();
			parseCommand(currentCommand);
			currentCommand = "";
			std::cout << "" << std::endl;
			break;
		case SDLK_UP:
			commandIndex = (commandIndex - 1 < 0 ? commands.size() - 1 : commandIndex - 1);
			if (commands.size() > 0) {
				currentCommand = commands.at(commandIndex);
			}
			else {
				currentCommand = "";
			}
			break;
		case SDLK_DOWN:
			commandIndex = (commandIndex + 1 > commands.size() ? commands.size() : commandIndex + 1);
			if (commandIndex < commands.size()) {
				currentCommand = commands.at(commandIndex);
			}
			else {
				currentCommand = "";
			}
			break;
		}
		//diagnostic printouts to the actual VS console
		std::cout << '\r' << std::setw(42);
		std::cout << '\r' <<currentCommand << std::flush;
		break;
	case SDL_TEXTINPUT:
		//you have to release the 'toggle' key in order to type
		//otherwise the console keeps exiting instantly
		if (canType) {
			//the character used for console toggle
			//(should be moved to an input mapping file somewhere)
			if (event.text.text[0] == '`') {
				std::cout << "exited console" << std::endl;
				//prevents typing on next console entry until a key has been released
				canType = false;
				GameState* returnstate = previous;
				//sets the previous to null to indicate that the console is deactivated
				this->previous = nullptr;
				return returnstate;
			}
			currentCommand += event.text.text;
			std::cout << '\r'  << currentCommand << std::flush;
		}
		break;
	case SDL_KEYUP:
		//on key release, you can start typing.
		canType = true;
		break;
	}
	return this;
}

void ConsoleState::parseCommand(std::string command) {
	std::vector<std::string> args = split(currentCommand, ' ');
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
				std::cout << "fail: " << std::endl;
			}
			//execute the function at that place in the map
		}
	}
}

std::vector<std::string> ConsoleState::split(const std::string& s, char delimiter)
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
