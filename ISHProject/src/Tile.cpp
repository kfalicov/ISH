#include "Tile.h"
#include "Camera.h"
#include "Game.h"
#include "AssetHandler.h"

Tile::Tile() {

}

Tile::Tile(int x, int y) {
	tilePos = vec2(x, y);
}

Tile::~Tile() {

}

void Tile::Render(Game* game, float interpolation) {
	// TODO move sprite initialization to constructor so we don't waste hella memory
	if (s != nullptr) {
		game->mainCamera->RenderSprite((*s), tilePos*PIXELS_PER_TILE);
	}
	if (transparent.size() > 0) {
		game->mainCamera->RenderSprite((*transparent.back()->sprite), tilePos*PIXELS_PER_TILE);
	}
}