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
	if (opaque != NULL) {
		game->mainCamera->RenderSprite((*opaque->sprite), tilePos);
		//std::cout << "Rendering opaque object at " << tilePos << std::endl;
	}
}

//vec2 Tile::worldPos()
//{
//	return pos + (16 * chunk); //chunk no. * chunk width/height, plus offset within chunk
//}