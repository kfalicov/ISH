#include "Entity.h"

Entity::Entity() {
	tilePos = chunkPos = vec2();
}

Entity::~Entity() {

}

Agent::Agent() {
	tilePos = chunkPos = currentPos = oldPos = renderPos = facing = vec2();
	canMove = true;
	moveTicks = 0;
}

Agent::~Agent() {

}

void Agent::updateRenderPosition(float interpolation) {
	renderPos = lerp(oldPos, currentPos, (moveTicks + interpolation) / animPeriod);
}

void Agent::Move() {

}

void Agent::Attack() {
	//std::cout << "Attacker's pos: " << currentPos << std::endl;
	//std::cout << "Under attack: " << facing << std::endl;
	vec2 tileToAttack = tilePos;
	Chunk c = (*currentChunk->getChunk(tileToAttack, facing));
	Tile t = (*c.getTile(tileToAttack));
	if (dynamic_cast<Agent*>(t.opaque)) {
		Agent* a = dynamic_cast<Agent*>(t.opaque);
		a->TakeDamage(attackStrength);
		//delete a;
	}
}

void Agent::Heal(int hp) {
	health += hp;
}

void Agent::TakeDamage(int hp) {
	std::cout << "Agent has taken " << hp << " damage!" << std::endl;
	health -= hp;
	std::cout << "Agent health now " << health << "." << std::endl;
}

void Agent::setAttackStrength(int attack) {
	attackStrength = attack;
}

Item::Item()
{
	sprite = AssetHandler::Instance()->GetSprite("Assets/TestItem.png", 0);
	tilePos = vec2(5, 5);

	attack = 5;
}

Item::~Item()
{
}