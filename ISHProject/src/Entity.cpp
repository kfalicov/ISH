#include "Entity.h"

Item::Item()
{
	sprite = AssetHandler::Instance()->GetSprite("Assets/TestItem.png", 0);
	tilePos = vec2(5, 5);

	attackStrength = 5;
}

Item::~Item()
{
}