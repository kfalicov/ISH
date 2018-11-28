#pragma once
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include "AssetHandler.h"
#include "GameState.h"
#include "Environment.h"
#include "Sprite.h"
#include "Entity.h"

SDL_Surface* GameState::Render(float interpolation, bool forceReRender) {
	if (needsRender || forceReRender) {
		//Clear the surface in order to be rendered again
		SDL_FillRect(surface, &surface->clip_rect,
			SDL_MapRGBA(surface->format, 0, 0, 0, 0));
		//each GameState will need to implement the WriteSurface function
		WriteSurface(interpolation);
		forceReRender = false;
		needsRender = false;
	}

	//surfaces.push_back(surface);
	return surface;
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


GameState* MenuState::Update(SDL_Event m_event){

	if (m_event.type == SDL_KEYDOWN) {
		std::cout << "Changing from menu" << std::endl;
		return new PlayState(assetHandler, nullptr);
	}
	return this;
}

void MenuState::WriteSurface(float interpolation)
{
	if (previous) {
		//note: since this is a pointer to previous's surface, it should not be deleted as it is only
		//created once and reused.
		SDL_Surface* underSurface = previous->Render(interpolation);
		SDL_BlitSurface(underSurface, &underSurface->clip_rect,
			this->surface, &surface->clip_rect);
	}
	//TODO render the menu objects to the Surface overtop of underSurface
}

/*

	PLAY/WORLD GAMESTATE

*/

PlayState::PlayState(AssetHandler* assetHandler, GameState* previous)
	: GameState(assetHandler)
{
	this->previous = previous;

	environment = new Environment(assetHandler);

	//Create the player and add them to the world
	player = new Entity(environment->
		getLoadedChunk(vec2(0, 0))->
		getTile(vec2(0, 0)),
		true, "Player");
	player->addSprite(assetHandler->GetSprite("Assets/Lemon.png", 0, TILE_SIZE));

	environmentCamera = new Camera(vec2(0, 0), 256, 256);
	environmentCamera->setCenter(player->getCurrentTile()->getPixelPosition());

	environmentSurface = SDL_CreateRGBSurface(0,
		int(environmentCamera->getSize()[0]),
		int(environmentCamera->getSize()[1]),
		32, 0, 0, 0, 0);
}

PlayState::~PlayState()
{
	SDL_FreeSurface(environmentSurface);
	SDL_FreeSurface(uiSurface);
	delete environment;
	delete environmentCamera;
}

GameState* PlayState::Update(SDL_Event event)
{
	environmentCamera->Update();

	//Get the keystates
	const Uint8 *keystates = SDL_GetKeyboardState(NULL);
	if (keystates[SDL_SCANCODE_D]) {
		//this is the format for using keyboard inputs in this
		//if the player is moving, the state should re-render
		
		needsRender = true;
	}
	if (assetHandler->Update()) {
		//assetHandler->UpdateSpriteFrames(LIST OF SPRITES IN PLAYSTATE);
		//assetHandler->UpdateEntityAnimations(LIST OF ENTITIES IN PLAYSTATE);
		//if any of the sprites have changed, the state should re-render
		needsRender = true;
	}

	//if nothing changes, then needsRender should remain false.
	//temporary for diagnostic

	//Make the camera track to the player
	environmentCamera->TrackTo(player->getCurrentTile()->getPixelPosition() + TILE_SIZE / 2);

	needsRender = true;
	return this;
}

void PlayState::WriteSurface(float interpolation)
{
	//Render World (background, items, entities) to environmentSurface
	RenderEnvironment();
	//Render the entities of the world
	RenderEntities(interpolation);
	//Render UI to uiSurface
	RenderUI();

	SDL_BlitSurface(environmentSurface, &environmentSurface->clip_rect,
		surface, &surface->clip_rect);
}

void PlayState::RenderEnvironment() {
	//Create a surface with the dimensions of the portion of the environment that is visible

	//Get bounds of chunks to render
	int minX = int(environment->centerChunkPos[0] - environment->renderChunkSquareRadius);
	int maxX = int(environment->centerChunkPos[0] + environment->renderChunkSquareRadius);
	int minY = int(environment->centerChunkPos[1] - environment->renderChunkSquareRadius);
	int maxY = int(environment->centerChunkPos[1] + environment->renderChunkSquareRadius);

	//For each loaded chunk (both visible and not), render the chunk if it is within visible bounds
	for (std::unordered_map<vec2, Chunk*>::iterator it = environment->loadedChunks.begin();
		it != environment->loadedChunks.end(); ++it) {
		Chunk* c = (it->second);

		//Render the chunk if the chunk is one of the visible chunks
		if (c->chunkPos[0] >= minX && c->chunkPos[0] <= maxX && c->chunkPos[1] >= minY && c->chunkPos[1] <= maxY) {
			//For each tile in the chunk, render the tile
			for (int x = 0; x < CHUNK_SIZE; ++x) {
				for (int y = 0; y < CHUNK_SIZE; ++y) {
					Tile* t = (c->getTile(vec2(x, y)));

					//std::cout << t.getPosition() << std::endl;

					//Get visible occupants of the tile
					std::vector<Entity*> tileOccupants = t->getTopOccupants();

					//If the tile has a background, render it
					if (t->getSprite() != nullptr) {
						//Put sprite onto environment surface
						Sprite* backgroundSprite = t->getSprite();
						SDL_Rect destRect = SDL_Rect();
						SDL_Rect srcRect = backgroundSprite->frames[backgroundSprite->currentFrameIndex];
						destRect.w = srcRect.w;
						destRect.h = srcRect.h;

						vec2 renderPos = t->getPixelPosition();
						destRect.x = int(renderPos[0] - environmentCamera->getPos()[0]);
						destRect.y = int(renderPos[1] - environmentCamera->getPos()[1]);

						//std::cout << "x: " << destRect.x << ", y: " << destRect.y << std::endl;
						SDL_BlitSurface(backgroundSprite->spriteSheet, &srcRect, environmentSurface, &destRect);
					}
					if (tileOccupants.size() > 0) { //transparent is a list of entities occupying the tile
						//Render the visible occupants of the tile
						//Game::Instance()->mainCamera->RenderSprite((*transparent.back()->sprite), tilePos*PIXELS_PER_TILE);
					}
				}
			}
		}
	}
}

void PlayState::RenderEntities(float interpolation) {
	//Render the player
	RenderEntity(player, interpolation);
}

void PlayState::RenderEntity(Entity* e, float interpolation) {
	//Put sprite onto environment surface
	Sprite* backgroundSprite = e->getDisplaySprite();
	SDL_Rect destRect = SDL_Rect();
	SDL_Rect srcRect = backgroundSprite->frames[backgroundSprite->currentFrameIndex];
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;

	vec2 renderPos = lerp(e->getPreviousTile()->getPixelPosition(),
		e->getCurrentTile()->getPixelPosition(),
		(e->getUpdatesSinceMove() + interpolation) / e->getUpdatesPerMove());

	destRect.x = int(renderPos[0] - environmentCamera->getPos()[0]);
	destRect.y = int(renderPos[1] - environmentCamera->getPos()[1]);

	SDL_BlitSurface(backgroundSprite->spriteSheet, &srcRect, environmentSurface, &destRect);
}

void PlayState::RenderUI() {
	//TODO blit stuff to uiSurface
}

/*

	CONSOLE GAMESTATE

*/

ConsoleState::ConsoleState(AssetHandler* assetHandler, GameState* previous)
	: GameState(assetHandler)
{
	this->previous = previous;
	this->canType = false;
	/*
	This section is the addition of all of the functions that the console
	can recognize. The string that the function will use is the 'key' in
	the map. The function itself can be the name of an existing function
	so long as the return type is int and the argument is std::vector<string>
	or the function can be a lambda as seen below. Make sure to check for
	vector index OOB on the arguments list (or, we could add the appropriate
	catch statement in ParseCommand()).
	*/

	functions["hello"] = &ConsoleState::hello;
	functions["state"] = &ConsoleState::changeState;
	functions["turnbased"] = &ConsoleState::toggleTurnBased;
	functions["help"] = &ConsoleState::displayHelp;
	//TODO more functions
}

void ConsoleState::WriteSurface(float interpolation)
{
	if (previous) {
		//note: since this is a pointer to previous's surface, it should not be deleted as it is only
		//created once and reused.
		SDL_Surface* underSurface = previous->Render(interpolation);
		SDL_BlitSurface(underSurface, &underSurface->clip_rect,
			this->surface, &surface->clip_rect);
	}
	//TODO render the console to the Surface overtop of underSurface
}

ConsoleState::~ConsoleState()
{
}

GameState * ConsoleState::Update(SDL_Event event)
{	
	
	switch (event.type) {
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym) {
		case SDLK_ESCAPE: {
			std::cout << "exited console" << std::endl;
			canType = false;
			GameState* returnstate = this->previous;
			//sets the previous to null to indicate that the console is deactivated
			this->previous = nullptr;
			//returns to regular control
			SDL_StopTextInput();
			return returnstate;
		}
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
			commandIndex = (commandIndex + 1 > int(commands.size()) ? commands.size() : commandIndex + 1);
			if (commandIndex < int(commands.size())) {
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
				//returns to regular control
				SDL_StopTextInput();
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
	args = split(currentCommand, ' ');
	if (args.size() > 0) {
		std::string cin = args[0];
		args.erase(args.begin());
		if (functions.find(cin) == functions.end()) {
			consoleOutput.push_back("Invalid command");
			return;
		}
		(this->*(functions[cin]))();
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
