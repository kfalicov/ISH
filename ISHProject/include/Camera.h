#pragma once
#include "Util.h"
#include "Game.h"
#include "Sprite.h"

#define PIXELS_PER_TILE 16

class Camera
{
public:
	Camera();
	Camera(Game* game, vec2 center, int w, int h);
	~Camera();

	void RenderSprite(Sprite sprite, vec2 position);

	void TrackTo(vec2 target);
	vec2 getPos();
	vec2 getCenter();
	vec2 getSize();
	void setCenter(vec2 dest);

	Game* game;
	SDL_Surface* cameraSurface;

private:
	vec2 camPosition; // The top left of the camera
	vec2 size;
	vec2 target; // Where the camera is moving to over time
};

