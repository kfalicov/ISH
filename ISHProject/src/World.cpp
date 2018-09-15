#include "World.h"
#include "Util.h"

World::World()
{

}

World::~World()
{

}

// gets the chunk based on chunk coordinates
Chunk* World::getChunk(vec2 pos)
{
	Chunk* tempChunk = spawn;
	if (chunkx < 0) {
		int i = 0;
		while (i > chunkx) {
			tempChunk = tempChunk->getWest();
			i--;
		}
	}
	else
	{
		int i = 0;
		while (i < chunkx) {
			tempChunk = tempChunk->getEast();
			i++;
		}
	}
	if (chunkx < 0) {
		int i = 0;
		while (i > chunkx) {
			tempChunk = tempChunk->getWest();
			i--;
		}
	}
	else
	{
		int i = 0;
		while (i < chunkx) {
			tempChunk = tempChunk->getEast();
			i++;
		}
	}
	if (chunky < 0) {
		int i = 0;
		while (i > chunky) {
			tempChunk = tempChunk->getNorth();
			i--;
		}
	}
	else
	{
		int i = 0;
		while (i < chunky) {
			tempChunk = tempChunk->getSouth();
			i++;
		}
	}
}

/*
obtains a tile based on absolute world coordinates
*/
Tile* World::getTile(vec2 pos) 
{
	int offsetx = int(pos[0]) % 16;
	int offsety = int(pos[1]) % 16;
	int chunkx = (pos[0] - offsetx) / 16;
	int chunky = (pos[1] - offsety) / 16;
	
	return getChunk(vec2(chunkx, chunky)->getTile(vec2(offsetx, offsety));
}

/*
Alternate function
obtains a tile based on absolute world coordinates
*/
Tile* World::getTile(vec2 pos) 
{
	return spawn->getTile(pos); //this will be called recursively when pos is out of the spawn's bounds
}