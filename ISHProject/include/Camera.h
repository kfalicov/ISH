#pragma once
#include "Util.h"
class Camera
{
public:
	Camera();
	~Camera();
	void TrackTo(vec2 target);
	vec2 setCenter(vec2 dest);
	vec2 getPos();
	vec2 getCenter();
	vec2 getSize();
private:
	vec2 position;
	vec2 size;
	vec2 target;
};

