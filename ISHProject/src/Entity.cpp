#pragma once
#include "Entity.h"
#include "AssetHandler.h"

Entity::Entity() {
	AssetHandler::Instance()->subscribeEntity(this);
}

MeleeWeapon::MeleeWeapon()
{
	Entity_Type = Entity_Type::MELEE_WEAPON;
	sprite = AssetHandler::Instance()->GetSprite("Assets/Items.png", 0);
	tilePos = currentPos = oldPos = renderPos = vec2(5, 5);
	attack = 5;
}
MeleeWeapon::~MeleeWeapon() {}

RangedWeapon::RangedWeapon() {
	Entity_Type = Entity_Type::RANGED_WEAPON;
	sprite = AssetHandler::Instance()->GetSprite("Assets/Items.png", 1);
	tilePos = currentPos = oldPos = renderPos = vec2(5, 5);
	attack = 4;
}
RangedWeapon::~RangedWeapon() {}

HeadArmor::HeadArmor() {
	Entity_Type = Entity_Type::ARMOR_HEAD;
	sprite = AssetHandler::Instance()->GetSprite("Assets/Items.png", 2);
	tilePos = currentPos = oldPos = renderPos = vec2(5, 5);
	defense = 3;
}
HeadArmor::~HeadArmor() {}

TorsoArmor::TorsoArmor() {
	Entity_Type = Entity_Type::ARMOR_TORSO;
	sprite = AssetHandler::Instance()->GetSprite("Assets/Items.png", 3);
	tilePos = currentPos = oldPos = renderPos = vec2(5, 5);
	defense = 3;
}
TorsoArmor::~TorsoArmor() {}

LegsArmor::LegsArmor() {
	Entity_Type = Entity_Type::ARMOR_LEGS;
	sprite = AssetHandler::Instance()->GetSprite("Assets/Items.png", 4);
	tilePos = currentPos = oldPos = renderPos = vec2(5, 5);
	defense = 3;
}
LegsArmor::~LegsArmor() {}