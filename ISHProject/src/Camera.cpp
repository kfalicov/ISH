#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::TrackTo(vec2 target)
{
	target = target;
}

vec2 Camera::getPos()
{
	return position;
}

vec2 Camera::getCenter()
{
	return position + (size / 2); //vec2 automatically does the math for us!
}

vec2 Camera::getSize()
{
	return size;
}
