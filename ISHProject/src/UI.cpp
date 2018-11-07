#include "UI.h"

Button::Button(vec2 pos, vec2 anchor, vec2 size)
	: Element(pos, anchor, size)
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

Element::Element(vec2 pos, vec2 anchor, vec2 size)
{

}

Element::~Element()
{
}
