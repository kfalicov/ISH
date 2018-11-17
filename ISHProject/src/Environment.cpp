#include "Environment.h"

Tile::Tile(vec2 pos, Chunk* parent) {
	this->pos = pos;
	this->parent = parent;
};
bool Tile::addOccupant(Entity* e) {
	//if there are already occupants
	if (occupants.size > 0) {
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
	if (index >= 0 && index < occupants.size()) {
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
vec2 Tile::getPosition() {
	//pixel position based on coordinate in chunk and tile pixel dimensions
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
}
Chunk * Tile::getParentChunk()
{
	return parent;
}

Chunk::Chunk(int x, int y)
{
	chunkPos = vec2(x, y);
	neighbors = std::vector<Chunk*>(4);
	//TODO instantialize a 16x16 chunk
	//set chunk pos
	//create array of tiles who are members of the chunk
	//new Tile(vec2(0-15,0-15), chunk's pos)
	tileGrid = new Tile**[CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		tileGrid[i] = new Tile*[CHUNK_SIZE];
	}

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			tileGrid[x][y] = new Tile((chunkPos[0] * (CHUNK_SIZE + TILE_SPACING)) + x, (chunkPos[1] * (CHUNK_SIZE + TILE_SPACING)) + y);
			//tileGrid[x][y]->s = AssetHandler::Instance()->GetSprite("Assets/AnimTest.png", 0);
			tileGrid[x][y]->s = worldGen.getBackgroundSprite(
				tileGrid[x][y]->tilePos[0],
				tileGrid[x][y]->tilePos[1]);
			//std::cout << tileGrid[x][y]->tilePos << std::endl;
		}
	}
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
	return tileGrid[(int)tilePos[0]][(int)tilePos[1]];
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
Chunk* Chunk::getChunk(vec2& tilePos, vec2 direction) {
	vec2 newPos = tilePos + direction;
	if (newPos[0] >= CHUNK_SIZE) {
		if (getEast() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::E;
			tilePos = newPos;
			return getEast();
		}
		else {
			return this;
		}
	}
	if (newPos[0] < 0) {
		if (getWest() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::W;
			tilePos = newPos;
			return getWest();
		}
		else {
			return this;
		}
	}
	if (newPos[1] >= CHUNK_SIZE) {
		if (getSouth() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::S;
			tilePos = newPos;
			return getSouth();
		}
		else {
			return this;
		}
	}
	if (newPos[1] < 0) {
		if (getNorth() != nullptr) {
			newPos -= CHUNK_SIZE * vec2::N;
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
	return max(abs(a[0] - b[0]), abs(a[1] - b[1]));
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

	deque<vec2> path;

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
				deque<Node*>::iterator it = find_if(open.begin(), open.end(), IsNode(temp->data));
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
			tileGrid[x][y]->Render(interpolation);
		}
	}
}