#pragma once
#include "Util.h"
#include "Game.h"
#include "Sprite.h"

#define PIXELS_PER_TILE 16

class Camera
{
public:
	Camera();
	Camera(vec2 center, int w, int h);
	~Camera();

	void Update();
	void RenderSprite(Sprite sprite, vec2 position);

	void TrackTo(vec2 target);
	vec2 getPos();
	vec2 getCenter();
	vec2 getSize();

	//sets the center of the camera
	void setCenter(vec2 dest);

	//sets the top left corner
	void setCorner(vec2 dest);

	Game* game;
	SDL_Surface* cameraSurface;

private:
	double followSpeed = 0.1;
	vec2 camPosition; // The top left of the camera
	vec2 size;
	vec2 target; // Where the camera is moving to over time
};

