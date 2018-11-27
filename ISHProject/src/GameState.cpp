#pragma once
#include <iostream>
#include <algorithm>
#include "AssetHandler.h"
#include "GameState.h"
#include "Environment.h"
#include "Sprite.h"

GameState::GameState(AssetHandler* assetHandler) {
	//TODO each state can have a different sized surface. For example, PlayState will have
	//a square surface to render the environment.
	initializeRenderSurface(800, 600);
	this->assetHandler = assetHandler;
	this->previous = nullptr;
}

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

MenuState::MenuState(AssetHandler* assetHandler, GameState* previous)
	: GameState(assetHandler)
{
	this->previous = previous;
}

MenuState::~MenuState(){}


GameState* MenuState::Update(SDL_Event event){
	if (event.type == SDL_KEYDOWN) {
		std::cout << "Changing from menu" << std::endl;
		return new PlayState(assetHandler, this);
	}
	return this;
}

SDL_Surface * MenuState::RenderLayers(float interpolation)
{
	return nullptr;
}


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