#pragma once
#include "Util.h"
#include "Sprite.h"
#include "Tile.h"
#include "Chunk.h"

class Tile;
class Chunk;

class Entity {
public:
	Entity();
	virtual ~Entity();

	Sprite* sprite;
	Chunk* currentChunk;
	Tile* currentTile;
	bool opaque;	// Does the entity take up physical space?
	vec2 tilePos;
	vec2 chunkPos;
};

class Agent : public Entity {
public:
	Agent();
	~Agent();

	int animPeriod = 3;	// Number of updates to visually transition from a to b
	int moveFreq = 3; // the speed at which movement can happen. read as "once every x ticks"
						// ^ must be higher than animPeriod
	int moveTicks = 0;	//"e"

	vec2 currentPos;// Current position (movement interpolation target)
	vec2 oldPos;	// Old position
	vec2 renderPos; //where to render the entity
	vec2 facing;

	bool canMove;
	void updateRenderPosition(float interpolation);
	virtual void Move();
	virtual void Attack();

	void Heal(int hp);
	void TakeDamage(int hp);
	void setAttackStrength(int attack);
protected:
	int health;
	int attackStrength;
};

class Item : public Entity {
public:
	Item();
	~Item();

	int attack;
};