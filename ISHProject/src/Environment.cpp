#pragma once
#include "Sprite.h"
#include "Entity.h"
#include "Environment.h"
#include "AssetHandler.h"
#include <iostream>

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
Entity* Tile::removeOccupant(int index) {
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
void Tile::depart() {
	if (occupants.size() > 0 && occupants.back()->isSolid()) {
		occupants.pop_back();
	}
}
vec2 Tile::getWorldPosition() {
	//pixel position based on coordinate in chunk and tile pixel dimensions
	return this->pos+(CHUNK_SIZE*parent->chunkPos);
}
vec2 Tile::getPosition() {
	return this->pos;
}
vec2 Tile::getPixelPosition() {
	return this->getWorldPosition() * TILE_SIZE;
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

Chunk::Chunk(const vec2 pos)
{
	this->chunkPos = pos;
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

void Chunk::pairHorizontal(Chunk * west, Chunk * east)
{
	if (west != nullptr) {
		west->setEast(east);
	}
	if (east != nullptr) {
		east->setWest(west);
	}
}
void Chunk::pairVertical(Chunk * north, Chunk * south)
{
	if (north != nullptr) {
		north->setSouth(south);
	}
	if (south != nullptr) {
		south->setNorth(north);
	}
}

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

//returns the tile relative to calling chunk's 0,0
Tile* Chunk::getTile(vec2 tilePos)
{
	//a fast vector access of a tile
	return tileGrid[(int)tilePos[1]*CHUNK_SIZE + (int)tilePos[0]];
}

//A helper function that gets the neighbors of the input coordinates
std::vector<vec2> Chunk::neighborsOf(vec2 tilePos)
{
	std::vector<vec2> tneighbors;
	tneighbors.push_back(tilePos + vec2::NORTH);
	tneighbors.push_back(tilePos + vec2::EAST);
	tneighbors.push_back(tilePos + vec2::SOUTH);
	tneighbors.push_back(tilePos + vec2::WEST);
	return tneighbors;
}

//Gets the tile to load based on a position and direction
Tile* Chunk::getAdjacentTile(Tile* currentTile, vec2 direction) {
	if (direction == vec2(0, 0)) {
		return currentTile;
	}
	vec2 newPos = (currentTile)->getWorldPosition() + direction;
	if (newPos[0] >= CHUNK_SIZE) {
		return currentTile->getParentChunk()->getEast()->getTile(newPos - CHUNK_SIZE * vec2::EAST);
	}
	if (newPos[0] < 0) {
		return currentTile->getParentChunk()->getWest()->getTile(newPos - CHUNK_SIZE * vec2::WEST);
	}
	if (newPos[1] >= CHUNK_SIZE) {
		return currentTile->getParentChunk()->getSouth()->getTile(newPos - CHUNK_SIZE * vec2::SOUTH);
	}
	if (newPos[1] < 0) {
		return currentTile->getParentChunk()->getNorth()->getTile(newPos - CHUNK_SIZE * vec2::NORTH);
	}
	return currentTile;
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

Environment::Environment() {
	//set everything to null
}

Environment::~Environment() {
	
}

void Environment::Update() {

}

/*
0  1  2
3  4  5
6  7  8
*/
//input deque must be sorted from lowest to highest Y
void Environment::moveEast(std::deque<Chunk*> newChunks)
{
	for (int i = loadDistX; i < (loadDistX*loadDistY); i += loadDistX) {
		loadedChunks[i] = newChunks.front();

		//setting neighbors
		newChunks.front()->setWest(loadedChunks[i-1]);
		loadedChunks[i - 1]->setEast(newChunks.front());

		newChunks.pop_front();
	}
	loadedChunks.pop_front();

	//sets neighbors of last item
	loadedChunks.back()->setEast(newChunks.front());
	newChunks.front()->setWest(loadedChunks.back());

	loadedChunks.push_back(newChunks.front());
}

void Environment::moveSouth(std::deque<Chunk*> newChunks)
{
	//adds a new row to the loaded queue
	//and pops the first row
	for (int i = 0; i < loadDistX; i++) {
		loadedChunks.push_back(newChunks.front());

		//setting neighbors
		newChunks.front()->setNorth(loadedChunks[((loadDistY - 1)*loadDistX) + i]);
		loadedChunks[((loadDistY - 1)*loadDistX) + i]->setSouth(newChunks.front());

		newChunks.pop_front();
		loadedChunks.pop_front();
	}
}

void Environment::moveWest(std::deque<Chunk*> newChunks)
{
	//setting neighbors
	Chunk::pairHorizontal(newChunks.front(), loadedChunks.front());
	loadedChunks.push_front(newChunks.front());

	for (int i = loadDistX; i < (loadDistX*loadDistY); i += loadDistX) {
		newChunks.pop_front();
		loadedChunks[i] = newChunks.front();

		//setting neighbors
		Chunk::pairHorizontal(newChunks.front(), loadedChunks[i+1]);
	}
	loadedChunks.pop_back();
}

void Environment::moveNorth(std::deque<Chunk*> newChunks)
{
	//adds a new row to the front of the queue
	//and pops the last row
	for (int i = 0; i < loadDistX; i++) {

		loadedChunks.push_front(newChunks.back());

		//setting neighbors
		newChunks.back()->setSouth(loadedChunks[loadDistX]);
		loadedChunks[loadDistX]->setNorth(newChunks.back());

		newChunks.pop_back();
		loadedChunks.pop_back();
	}
}
