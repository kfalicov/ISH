#pragma once
#include <vector>
#include <stdio.h>
#include "Util.h"

typedef void(*CallbackFunction)();


class Element {
public:
	Element* parent;
	vec2 expanding;

	Element(vec2 pos, vec2 anchor, vec2 size);
	~Element();
protected:
	std::vector<Element> children; //the heirarchy
private:
	vec2 anchor; //the point which ui elements are transformed about, measured from 0-1 and 0-1 (% of element size)
	vec2 pos;	//the position that the anchor is located in
	vec2 size;	//the width and height
};

class Button : public Element {
public:
	Button(vec2 pos, vec2 anchor, vec2 size);
	~Button();
	void click();
	//Usage: pass in a function name with no parenthesis, or create a lambda of the format "[]() {code;}"
	void setCallback(CallbackFunction fcn);
private:
	void* optr;
	CallbackFunction fptr; //the function to call on click
};