#include "UI.h"
#include "AssetHandler.h"

Element::Element(vec2 pos, vec2 size, vec2 anchor)
{
	this->pos = pos;
	this->size = size;
	this->anchor = anchor;
	this->sprite = AssetHandler::GetSprite("Assets/AnimTest.png", AssetHandler::Tiles::GRASS);
}

Element::~Element()
{
}

vec2 Element::renderpos()
{
	return pos - prod(anchor, size);
}

Button::Button(vec2 pos, vec2 size, vec2 anchor)
	: Element(pos, size, anchor)
{
	
}

Button::~Button()
{
}

void Button::click()
{
	//(optr)->(fptr)();
	(*fptr)(); //calls the stored function
}

void Button::setCallback(CallbackFunction fcn){
	fptr = fcn; //sets the function to be called
}


