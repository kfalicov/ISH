#pragma once
#include "Sprite.h"
#include "Environment.h"
//#include "AssetHandler.h"
#include <string>
#include <vector>

class Tile;

class Entity {
public:
	Entity(std::vector<Sprite*> animations, bool solid = false, std::string name = "entity");
	//TODO
	~Entity();

	virtual void Update();
	void Attack();
	virtual void TakeDamage(int damage);
	
	//These are used when calculating attack. Override these in the Player
	//to use the player's items instead
	virtual int getAttackType() {
		return this->attackType;
	};
	virtual int getResistanceType() {
		return this->resistanceType;
	}

	//returns a pointer to the sprite of the current animation
	//using the displayAnimationIndex and the animations vector
	Sprite* getDisplaySprite();

	//asset handler uses this to make all entities update animations simultaneously
	void queueAnimationChange();

	//returns whether the entity blocks other entities from sharing its space.
	bool isSolid();

	//TODO this goes in tile
	//virtual vec2 getRenderPosition(float interpolation) {};

	//returns the string representation of the object
	//append necessary information to the string for ID purposes
	std::string toString();

private:
	//The display name of the entity.
	std::string name;

	//Whether or not this entity takes up physical space on a tile.
	bool solid;

	//Index of current sprite animation.
	int displayAnimationIndex;
	//Index of next sprite animation, will update display index on next frame change.
	int nextAnimationIndex;
	//List of possible animations for entity to display.
	std::vector<Sprite*> animations;
	bool shouldChangeAnimation = false;

	//Number of updates to visually transition from currentTile to nextTile
	int visualMoveDuration = 8;
	//Elapsed updates since last move.
	int updatesSinceMove = 0;
	//Number of updates per move. Must be >= visualMoveDuration. "Time entity takes to move," lower = faster.
	int updatesPerMove = 8;

	//The tile that the entity is on.
	Tile* currentTile;
	//The tile that the entity is lerping from.
	Tile* previousTile;
	//The tile in the direction that the entity is facing.
	//This is useful as the "attack target" tile.
	Tile* facingTile;

	//The maximum health the entity can have.
	int maxHealth;
	//The current health of the entity.
	int currentHealth;
	//The base attack value of the entity.
	int attack;

	//Weapons of a given damage type deal less damage against armor of the same resistance type.
	enum damageType { SLASHING, PIERCING, CRUSHING };
	int attackType;
	int resistanceType;
};