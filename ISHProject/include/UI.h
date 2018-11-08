#pragma once
#include <vector>
#include <stdio.h>
#include "Util.h"
#include "Sprite.h"

typedef void(*CallbackFunction)();

// A display element on the UI. can contain text or images.
class Element {
public:
	Element* parent;
	vec2 expanding;
	Sprite* sprite;

	Element(vec2 pos, vec2 size, vec2 anchor = vec2(0,0));
	~Element();
	vec2 renderpos();
	virtual std::string type() { return "element"; };

	vec2 anchor; //the point which ui elements are transformed about, measured from 0-1 and 0-1 (% of element size)
	vec2 pos;	//the position that the anchor is located in
	vec2 size;	//the width and height
protected:
	std::vector<Element> children; //the heirarchy
private:
	
};

// a more specific display element that contains a function to call on click.
class Button : public Element {
public:
	Button(vec2 pos, vec2 size, vec2 anchor = vec2(0,0));
	~Button();
	void click();
	//Usage: pass in a function name with no parenthesis, or create a lambda of the format "[]() {code;}"
	void setCallback(CallbackFunction fcn);
	std::string type() override { return "button"; };
private:
	void* optr;
	CallbackFunction fptr; //the function to call on click
};