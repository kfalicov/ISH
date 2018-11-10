#pragma once
#include "Util.h"
#include "Sprite.h"
#include "Tile.h"
#include "Chunk.h"
#include <string>

class Tile;
class Chunk;

class Entity {
public:
	~Entity() {};

	static const enum Entity_Type {PLAYER, ENEMY, 
		MELEE_WEAPON, RANGED_WEAPON, 
		ARMOR_HEAD, ARMOR_TORSO, ARMOR_LEGS};

	//string name;
	Sprite* sprite;
	Chunk* currentChunk;
	Tile* currentTile;
	bool opaque;	// Does the entity take up physical space?
	vec2 tilePos;
	vec2 chunkPos;

	int animPeriod = 3;	// Number of updates to visually transition from a to b
	int moveFreq = 3; // the speed at which movement can happen. read as "once every x ticks"
						// ^ must be higher than animPeriod
	int moveTicks = 0;	//"e"

	vec2 currentPos;// Current position (movement interpolation target)
	vec2 oldPos;	// Old position
	vec2 renderPos; //where to render the entity
	vec2 facing;

	bool canMove;
	virtual void updateRenderPosition(float interpolation) {};
	virtual void Move() {};
	virtual void Attack() {};

	virtual void Heal(int hp) {};
	virtual void TakeDamage(int damage) {};

	Entity_Type type() { return Entity_Type; };

	int health;
	int attack;
	int defense;
	int durability;

protected:
	Entity_Type Entity_Type;
	Entity() {};
};

class MeleeWeapon : public Entity {
public:
	MeleeWeapon();
	~MeleeWeapon();
};
class RangedWeapon : public Entity {
public:
	RangedWeapon();
	~RangedWeapon();
};

class HeadArmor : public Entity {
public:
	HeadArmor();
	~HeadArmor();
};
class TorsoArmor : public Entity {
public:
	TorsoArmor();
	~TorsoArmor();
};
class LegsArmor : public Entity {
public:
	LegsArmor();
	~LegsArmor();
};