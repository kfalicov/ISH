#pragma once
#include "Sprite.h"
#include "Entity.h"
#include "Environment.h"
#include "AssetHandler.h"

Tile::Tile(vec2 pos, Chunk* parent) {
	this->pos = pos;
	this->parent = parent;
	this->displayIndex = -1;
	this->sprite = nullptr; //TODO give this tile a sprite somehow
}
Tile::~Tile()
{
	//TODO
	//Does anything need to be done here?
	//we can't delete parent since if the tile gets to this it means parent is already being deleted, and calling delete
	//would lead to an infinite loop.
	//we can't delete sprite since it's a reference to something stored in AssetHandler.
	//everything else is a non-pointer and would automatically be discarded.
}
bool Tile::addOccupant(Entity* e) {
	//if there are already occupants
	if (occupants.size() > 0) {
		//if there is an obstacle
		if (occupants.back()->isSolid() && e->isSolid()) {
			return false;
		}
	}
	occupants.push_back(e);
	//increment display index to reflect last transparent item
	if (!e->isSolid()) {
		displayIndex++;
	}
	return true;
}
void Tile::cycleItems() {
	//since occupants size is > 1, displayIndex is guaranteed to be >= 0
	if (occupants.size() > 1){
		int numOfItems = occupants.size() - 1;
		displayIndex = (displayIndex + 1) % numOfItems;
	}
}
Entity* Tile::removeOccupant(int index = -1) {
	//if you passed in an index for manual removal, use it
	index = index < 0 ? displayIndex : index;
	//verify index in range
	if (index >= 0 && index < int(occupants.size())) {
		Entity* removed = occupants.at(index);
		//decrement display index if entity removed is an item
		if (!(removed->isSolid())) {
			displayIndex--;
		}
		occupants.erase(occupants.begin() + index);
		return removed;
	}
	return nullptr;
}
vec2 Tile::getWorldPosition() {
	//pixel position based on coordinate in chunk and tile pixel dimensions
	return this->pos;
}
vec2 Tile::getPixelPosition() {
	return this->pos * TILE_SIZE;
}

Sprite* Tile::getSprite() {
	return sprite;
}
std::vector<Entity*> Tile::getTopOccupants() {
	std::vector<Entity*> topFew;
	if (occupants.size() > 0) {
		//if there are entities on the tile
		if (displayIndex >= 0) {
			//and if there is a transparent entity to display
			topFew.push_back(occupants.at(displayIndex));
		}
		if (occupants.back()->isSolid()) {
			//the solid entity in the list
			topFew.push_back(occupants.back());
		}
	}
	return topFew;
}
Chunk* Tile::getParentChunk()
{
	return parent;
}

void Tile::setSprite(Sprite* sprite) {
	this->sprite = sprite;
}

Chunk::Chunk(int x, int y)
{
	this->chunkPos = vec2(double(x), double(y));
	neighbors = std::vector<Chunk*>(4);
}

Chunk::~Chunk()
{
	//clears out the tile grid.
	for (int i = 0; i < tileGrid.size(); i++) {
		delete tileGrid[i];
	}
	//TODO chunk destructor
	//is there anything more that needs to be deleted?
}

void Chunk::setTiles(std::vector<Tile*> tileGrid) {
	this->tileGrid = tileGrid;
}

Chunk* Chunk::getNorth() { return neighbors[dir::NORTH]; }
Chunk* Chunk::getEast() { return neighbors[dir::EAST]; }
Chunk* Chunk::getSouth() { return neighbors[dir::SOUTH]; }
Chunk* Chunk::getWest() { return neighbors[dir::WEST]; }

void Chunk::setNorth(Chunk* N) {
	neighbors[dir::NORTH] = N;
}

void Chunk::setEast(Chunk* E) {
	neighbors[dir::EAST] = E;
}

void Chunk::setSouth(Chunk* S) {
	neighbors[dir::SOUTH] = S;
}

void Chunk::setWest(Chunk* W) {
	neighbors[dir::WEST] = W;
}
/*
	returns the tile relative to calling chunk's 0,0
	A recursive x-first seek for the target tile
*/
Tile* Chunk::getTile(vec2 tilePos)
{
	//a fast vector access of a tile
	return tileGrid[(int)tilePos[0]*CHUNK_SIZE + (int)tilePos[1]];
}

//A helper function that gets the neighbors of the input coordinates
std::vector<vec2> Chunk::neighborsOf(vec2 tilePos)
{
	std::vector<vec2> tneighbors;
	tneighbors.push_back(tilePos + vec2(-1, 0));
	tneighbors.push_back(tilePos + vec2(1, 0));
	tneighbors.push_back(tilePos + vec2(0, -1));
	tneighbors.push_back(tilePos + vec2(0, 1));
	return tneighbors;
}

//Gets the chunk to load based on a world position. Updates the tilepos to be relative to that chunk's internal coordinate system.
Chunk* Chunk::getChunk(vec2 tilePos, vec2 direction) {
	vec2 newPos = tilePos + direction;
	if (newPos[0] >= CHUNK_SIZE) {
		if (getEast() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::EAST;
			tilePos = newPos;
			return getEast();
		}
		else {
			return this;
		}
	}
	if (newPos[0] < 0) {
		if (getWest() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::WEST;
			tilePos = newPos;
			return getWest();
		}
		else {
			return this;
		}
	}
	if (newPos[1] >= CHUNK_SIZE) {
		if (getSouth() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::SOUTH;
			tilePos = newPos;
			return getSouth();
		}
		else {
			return this;
		}
	}
	if (newPos[1] < 0) {
		if (getNorth() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::NORTH;
			tilePos = newPos;
			return getNorth();
		}
		else {
			return this;
		}
	}

	tilePos = newPos;
	return this;
}

double Chunk::manhattan(vec2 a, vec2 b)
{
	return abs(a[0] - b[0]) + abs(a[1] - b[1]);
}

double Chunk::psquared(vec2 a, vec2 b)
{
	return ((a[0] - b[0])*(a[0] - b[0])) + ((a[1] - b[1])*(a[1] - b[1]));
}

double Chunk::diagonal(vec2 a, vec2 b)
{
	return std::max(abs(a[0] - b[0]), abs(a[1] - b[1]));
}

/*
	Gets the best path between (a, b]
*/
std::deque<vec2> Chunk::AStarPath(vec2 & a, vec2 & b)
{
	std::deque<Node*> open;
	std::deque<Node*> closed;
	Node* start = new Node(a);
	start->acost = 0;
	start->bcost = HEURISTIC(a, b);

	std::deque<vec2> path;

	open.push_back(start); //starts with one visible tile, the origin tile
	bool foundPath = false;

	Node* current = start;
	while (!open.empty()) {
		current = open.front();
		open.pop_front();
		if (current->data == b) {
			foundPath = true;
			break;
		}
		closed.push_back(current);
		std::vector<vec2> surroundings = neighborsOf(current->data);
		for (vec2 v : surroundings) {

			Node* temp = new Node(v);

			//check if temp is a valid tile to traverse, and if it hasn't been visited already
			if (std::find_if(closed.begin(), closed.end(), IsNode(temp->data)) == closed.end()) {
				temp->acost = current->acost + 1;	//TODO the cost doesn't have to be 1, instead it can be dependent on tile terrain
				temp->bcost = HEURISTIC(v, b);

				temp->parent = current;
				std::deque<Node*>::iterator it = find_if(open.begin(), open.end(), IsNode(temp->data));
				if (it == open.end()) { //if the node is not found in the currently visible nodes list the iterator will have reached the end
					open.push_back(temp);
					sort(open.begin(), open.end(), PointerCompare());
				}
				//this might only be useful if negative cost paths are possible (please don't)
				//else if (temp->acost < (*it)->acost) { //the node is in the visible list but you've found a better way to access it
				//	(*it)->acost = temp->acost;
				//}
			}
		}

	}
	if (!foundPath) {
		//TODO behavior when no path is found
		//at the moment, nothing
		//path.push_front(a);
	}
	Node* traversal = current;
	while (!(traversal == start) && traversal->parent != nullptr) {
		path.push_front(traversal->data);
		traversal = traversal->parent;
	}
	return path;
}

void Chunk::Render(float interpolation) {
	for (int x = 0; x < CHUNK_SIZE; ++x) {
		for (int y = 0; y < CHUNK_SIZE; ++y) {
			//surface.blit(tileGrid[x][y]->getSprite(), tileGrid[x][y]->displayPos);
		}
	}
}

Environment::Environment(AssetHandler* assetHandler, int seed) {
	this->assetHandler = assetHandler;
	centerChunkPos = vec2(0, 0);
	loadChunks();
}

Environment::~Environment() {
	
}

void Environment::Update() {

}

std::vector<Tile*> Environment::Generator::generateTiles(AssetHandler* assetHandler, Chunk* chunk) {
	//TODO instantialize a 16x16 chunk
	//set chunk pos
	//create array of tiles who are members of the chunk
	//new Tile(vec2(0-15,0-15), chunk's pos)
	std::vector<Tile*> tiles;
	for (int i = 0; i < CHUNK_SIZE*CHUNK_SIZE; i++) {
		int x = i % CHUNK_SIZE;
		int y = int(i / CHUNK_SIZE);
		Tile* tempTile = new Tile(vec2(
			chunk->chunkPos[0] * CHUNK_SIZE + x,
			chunk->chunkPos[1] * CHUNK_SIZE + y), chunk);
		if ((x % 2 == 0 && y % 2 == 0) || (x % 2 == 1 && y % 2 == 1)) {
			Sprite* s = assetHandler->GetSprite("Assets/Temps.png", 2, TILE_SIZE);
			tempTile->setSprite(s);
		}
		else {
			Sprite* s = assetHandler->GetSprite("Assets/Temps.png", 1, TILE_SIZE);
			tempTile->setSprite(s);
		}
		tiles.push_back(tempTile);
	}
	return tiles;
}

void Environment::loadChunks() {
	int minX = int(centerChunkPos[0] - chunkSquareRadius);
	int maxX = int(centerChunkPos[0] + chunkSquareRadius);
	int minY = int(centerChunkPos[1] - chunkSquareRadius);
	int maxY = int(centerChunkPos[1] + chunkSquareRadius);

	//For all chunks that SHOULD BE loaded...
	for (int chunkX = minX; chunkX <= maxX; chunkX++) {
		for (int chunkY = minY; chunkY <= maxY; chunkY++) {
			vec2 pos = vec2(chunkX, chunkY);

			//if this key isn't already found in the map
			if (loadedChunks.find(pos) == loadedChunks.end()) { //If they are not already loaded, load them
				Chunk* newChunk = new Chunk(chunkX, chunkY);
				newChunk->setTiles(Generator::generateTiles(assetHandler, newChunk));
				loadedChunks.insert(std::pair<vec2, Chunk*>(pos, newChunk));
			}
			//std::cout << chunkX << ", " << chunkY << std::endl;
		}
	}

	//Set up NSEW chunks
	for (std::unordered_map<vec2, Chunk*>::iterator it = loadedChunks.begin(); it != loadedChunks.end();) {
		vec2 p = (it->first);

		//if a chunk is loaded and it shouldn't be, unload it
		if (p[0] < minX || p[0] > maxX || p[1] < minY || p[1] > maxY) {
			//use chunk destructor
			delete it->second;
			it = loadedChunks.erase(it);
			continue;
		}

		if (it->second->getNorth() == nullptr) {
			it->second->setNorth(getLoadedChunk(p + vec2::NORTH));
		}
		if (it->second->getEast() == nullptr) {
			it->second->setEast(getLoadedChunk(p + vec2::EAST));
		}
		if (it->second->getSouth() == nullptr) {
			it->second->setSouth(getLoadedChunk(p + vec2::SOUTH));
		}
		if (it->second->getWest() == nullptr) {
			it->second->setWest(getLoadedChunk(p + vec2::WEST));
		}

		it++;
	}
}

Chunk* Environment::getLoadedChunk(vec2 position) {
	std::unordered_map<vec2, Chunk*>::iterator it = loadedChunks.find(position);
	if (it != loadedChunks.end()) {
		return it->second;
	}
	return nullptr;
}