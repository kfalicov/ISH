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
	if (s != NULL) {
		game->mainCamera->RenderSprite((*s), tilePos);
	}
	if (transparent.size() > 0) {
		game->mainCamera->RenderSprite((*transparent.back()->sprite), tilePos);
	}
}