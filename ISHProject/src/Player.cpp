#include "Player.h"
#include "AssetHandler.h"
#include "ActionManager.h"

Player::Player() {
	Entity_Type = Entity_Type::PLAYER;
	tilePos = chunkPos = currentPos = oldPos = renderPos = facing = vec2();
	canMove = true;
	moveTicks = 0;

	//sprite = AssetHandler::Instance()->GetSprite("Assets/Lemon.png", 0);
	sprite = AssetHandler::Instance()->GetSprite("Assets/AnimTest.png", 0);
	sprite->setRectsFromIndices(16, 16, { 0,1,2,3 });

	ActionManager::Instance()->player = this;
	health = 1000;
	facing = vec2::W;
}

Player::~Player() {

}

void Player::updateRenderPosition(float interpolation) {
	renderPos = lerp(oldPos, currentPos, (moveTicks + interpolation) / animPeriod);
}

void Player::Move(vec2 dir) {
	if (dir != vec2(0, 0)) {
		facing = dir;
	}

	if (canMove) {
		oldPos = currentPos;

		vec2 oldTilePos = tilePos;
		Chunk* newChunk = currentChunk->getChunk(tilePos, dir);
		if (newChunk->getTile(tilePos)->opaque == nullptr) {
			moveTicks = 0;

			//getChunk will change tilePos to the new position if there is a space available
			currentChunk = newChunk;

			currentTile->opaque = nullptr;
			currentTile = currentChunk->getTile(tilePos);
			currentTile->opaque = this;

			//tilePos = currentTile->tilePos;
			chunkPos = currentChunk->chunkPos;
			currentPos = currentTile->tilePos;

			//std::cout << "Player moving to chunk: " << chunkPos << ", tile: " << tilePos << std::endl;
		}
		else {
			tilePos = oldTilePos;
		}
	}
}

//TODO split attack() call into melee_attack() or ranged_attack() depending on currently equipped weapon
void Player::Attack() {
	//std::cout << "Attacker's pos: " << currentPos << std::endl;
	//std::cout << "Under attack: " << facing << std::endl;
	vec2 tileToAttack = tilePos;
	Chunk c = (*currentChunk->getChunk(tileToAttack, facing));
	Tile t = (*c.getTile(tileToAttack));
	if (t.opaque) {
		t.opaque->TakeDamage(attack + melee_weapon->attack);
	}
}

void Player::TakeDamage(int damage) {
	std::cout << "Agent has taken " << damage << " damage!" << std::endl;
	health -= damage;
	std::cout << "Agent health now " << health << "." << std::endl;
}

void Player::Equip() {
	if (currentTile->transparent.size() > 0) {
		Entity* toEquip = currentTile->transparent.back();
		currentTile->transparent.pop_back();
		int et = toEquip->type();
		switch (et) {
		case MELEE_WEAPON:
			if (melee_weapon) {
				currentTile->transparent.insert(currentTile->transparent.end(), melee_weapon);
			}
			melee_weapon = toEquip;
			break;
		case RANGED_WEAPON:
			if (ranged_weapon) {
				currentTile->transparent.insert(currentTile->transparent.end(), ranged_weapon);
			}
			ranged_weapon = toEquip;
			break;
		case ARMOR_HEAD:
			if (head_armor) {
				currentTile->transparent.insert(currentTile->transparent.end(), head_armor);
			}
			head_armor = toEquip;
			break;
		case ARMOR_TORSO:
			if (torso_armor) {
				currentTile->transparent.insert(currentTile->transparent.end(), torso_armor);
			}
			torso_armor = toEquip;
			break;
		case ARMOR_LEGS:
			if (legs_armor) {
				currentTile->transparent.insert(currentTile->transparent.end(), legs_armor);
			}
			legs_armor = toEquip;
			break;
		default:
			currentTile->transparent.insert(currentTile->transparent.end(), toEquip);
		}
	}
}

void Player::Rummage() {
	if (currentTile->transparent.size() > 1) {
		Entity* temp = currentTile->transparent.back();
		currentTile->transparent.pop_back();
		currentTile->transparent.insert(currentTile->transparent.begin(), temp);
	}
}