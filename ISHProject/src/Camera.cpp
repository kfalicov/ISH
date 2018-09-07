#include "Camera.h"

Camera::Camera()
{

}

Camera::Camera(Game* game, vec2 center, int w, int h) {
	this->game = game;
	setCenter(center);
	size = vec2(w, h);
	cameraSurface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
}

Camera::~Camera()
{

}

void Camera::RenderSprite(Sprite sprite, vec2 position)
{
	SDL_Rect destRect = SDL_Rect();
	SDL_Rect srcRect = sprite.srcRect;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
	destRect.x = (getCenter()[0] + position[0]);
	destRect.y = (getCenter()[1] + position[1]);
	//std::cout << getCenter()[0] << std::endl;

	SDL_BlitSurface(sprite.spriteSheet, &srcRect, cameraSurface, &destRect);
}

void Camera::TrackTo(vec2 dest)
{
	target = dest-(size/2); //offsets so that the camera is tracking by its own center
}

vec2 Camera::getPos()
{
	return camPosition;
}

vec2 Camera::getCenter()
{
	return camPosition + (size / 2); //vec2 automatically does the math for us!
}

vec2 Camera::getSize()
{
	return size;
}

void Camera::setCenter(vec2 dest)
{
	camPosition = dest - (size / 2); //vec2 automatically does the math for us!
}
