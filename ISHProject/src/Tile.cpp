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
	s = game->assetHandler->GetSprite("Assets/AnimTest.png", AssetHandler::Tiles::GRASS);
	game->mainCamera->RenderSprite((*s), tilePos);
}

//vec2 Tile::worldPos()
//{
//	return pos + (16 * chunk); //chunk no. * chunk width/height, plus offset within chunk
//}