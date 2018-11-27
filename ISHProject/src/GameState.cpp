#pragma once
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "AssetHandler.h"
#include "GameState.h"
#include "Environment.h"
#include "Sprite.h"

void GameState::Render(std::vector<SDL_Surface*> &surfaces, float interpolation, bool forceReRender) {
	if (previous) {
		previous->Render(surfaces, interpolation);
	}

	if (needsRender || forceReRender) {
		//Clear the surface in order to be rendered again
		SDL_FillRect(surface, &surface->clip_rect,
			SDL_MapRGBA(surface->format, 0, 0, 0, 0));
		surface = RenderLayers(interpolation);
		forceReRender = false;
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
	if (event.type == SDL_KEYDOWN) {
		std::cout << "Changing from menu" << std::endl;
		return new PlayState(assetHandler, nullptr);
	}
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

	environment = new Environment(assetHandler);
	environmentCamera = new Camera(vec2(0,0), 256, 256);
}

PlayState::~PlayState()
{
	
}

GameState* PlayState::Update(SDL_Event event)
{
	if (event.type == SDL_KEYDOWN) {
		std::cout << "Changing from play" << std::endl;
		return new MenuState(assetHandler, nullptr);
	}
	if (assetHandler->Update()) {
		//assetHandler->UpdateSpriteFrames(LIST OF SPRITES IN PLAYSTATE);
		//assetHandler->UpdateEntityAnimations(LIST OF ENTITIES IN PLAYSTATE);
	}

	needsRender = true;

	return this;
}

SDL_Surface* PlayState::RenderLayers(float interpolation)
{
	//Render World (background, items, entities) and return their combined surface.
	SDL_Surface* environmentSurface = RenderEnvironment();
	//Combine world surface with UI
	SDL_Surface* uiSurface = RenderUI();

	//Combine surfaces
	int width = environmentSurface->w;// std::max(environmentSurface->w, uiSurface->w);
	int height = environmentSurface->h;// std::max(environmentSurface->h, uiSurface->h);
	SDL_Surface* combinedSurface = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

	//Blit each layer to the combined layer
	SDL_BlitSurface(environmentSurface, &environmentSurface->clip_rect,
		combinedSurface, &combinedSurface->clip_rect);
	//SDL_BlitSurface(uiSurface, &uiSurface->clip_rect,
	//	combinedSurface, &combinedSurface->clip_rect);

	return combinedSurface;
}

SDL_Surface* PlayState::RenderEnvironment() {
	//Create a surface with the dimensions of the portion of the environment that is visible
	SDL_Surface* surface = SDL_CreateRGBSurface(0,
		environmentCamera->getSize()[0],
		environmentCamera->getSize()[1],
		32, 0, 0, 0, 0);

	//Get bounds of chunks to render
	int minX = environment->centerChunkPos[0] - environment->renderChunkSquareRadius;
	int maxX = environment->centerChunkPos[0] + environment->renderChunkSquareRadius;
	int minY = environment->centerChunkPos[1] - environment->renderChunkSquareRadius;
	int maxY = environment->centerChunkPos[1] + environment->renderChunkSquareRadius;

	//For each loaded chunk (both visible and not), render the chunk if it is within visible bounds
	for (std::unordered_map<vec2*, Chunk*>::iterator it = environment->loadedChunks.begin();
		it != environment->loadedChunks.end(); ++it) {
		Chunk c = (*it->second);

		//Render the chunk if the chunk is one of the visible chunks
		if (c.chunkPos[0] >= minX && c.chunkPos[0] <= maxX && c.chunkPos[1] >= minY && c.chunkPos[1] <= maxY) {
			//For each tile in the chunk, render the tile
			for (int x = 0; x < CHUNK_SIZE; ++x) {
				for (int y = 0; y < CHUNK_SIZE; ++y) {
					Tile t = (*c.getTile(vec2(x, y)));

					//std::cout << t.getPosition() << std::endl;

					//Get visible occupants of the tile
					std::vector<Entity*> tileOccupants = t.getTopOccupants();

					//If the tile has a background, render it
					if (t.getSprite() != nullptr) {
						//Put sprite onto environment surface
						Sprite* backgroundSprite = t.getSprite();
						SDL_Rect destRect = SDL_Rect();
						SDL_Rect srcRect = backgroundSprite->frames[backgroundSprite->currentFrameIndex];
						destRect.w = srcRect.w;
						destRect.h = srcRect.h;

						vec2 renderPos = t.getPixelPosition();
						destRect.x = renderPos[0] - environmentCamera->getPos()[0];
						destRect.y = renderPos[1] - environmentCamera->getPos()[1];

						//std::cout << "x: " << destRect.x << ", y: " << destRect.y << std::endl;
						SDL_BlitSurface(backgroundSprite->spriteSheet, &srcRect, surface, &destRect);
					}
					if (tileOccupants.size() > 0) { //transparent is a list of entities occupying the tile
						//Render the visible occupants of the tile
						//Game::Instance()->mainCamera->RenderSprite((*transparent.back()->sprite), tilePos*PIXELS_PER_TILE);
					}
				}
			}
		}
	}
	return surface;
}

SDL_Surface* PlayState::RenderUI() {
	SDL_Surface* surface = SDL_CreateRGBSurface(0, 256, 256, 32, 0, 0, 0, 0);
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
