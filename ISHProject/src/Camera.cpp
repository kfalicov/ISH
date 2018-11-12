#include "Camera.h"
#include "Chunk.h"

Camera::Camera()
{

}

Camera::Camera(vec2 center, int w, int h) {
	setCenter(center);
	size = vec2(w, h);
	cameraSurface = SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0);
}

Camera::~Camera()
{

}

void Camera::Update() {
	setCenter(lerp(getCenter(), target, followSpeed));
}

void Camera::RenderSprite(Sprite sprite, vec2 position)
{
	SDL_Rect destRect = SDL_Rect();
	SDL_Rect srcRect = sprite.frames[sprite.currentFrameIndex];
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;
	destRect.x = position[0] - camPosition[0];
	destRect.y = position[1] - camPosition[1];
	//std::cout << getCenter()[0] << std::endl;

	SDL_BlitSurface(sprite.spriteSheet, &srcRect, cameraSurface, &destRect);
}

void Camera::TrackTo(vec2 dest)
{
	target = dest;
}

vec2 Camera::getPos()
{
	return camPosition;
}

vec2 Camera::getCenter()
{
	return camPosition + (size / 2);// -(CHUNK_SIZE / 2)*PIXELS_PER_TILE; //vec2 automatically does the math for us!
}

vec2 Camera::getSize()
{
	return size;
}

void Camera::setCenter(vec2 dest)
{
	camPosition = dest - (size / 2); //vec2 automatically does the math for us!
}
