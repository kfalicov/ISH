#include "World.h"
#include "Util.h"

World::World()
{

}

World::~World()
{

}

Tile* World::getTile(vec2 pos) {
	return spawn->getTile(pos);
}