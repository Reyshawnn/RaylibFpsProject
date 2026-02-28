#include "Movement.h"

constexpr void Movement::setDirection(const Vector3& dir)
{
	finalDir = dir;
}

constexpr void Movement::setPosition(const float positionX, const float positionY, const float positionZ)
{
	position.x = positionX;
	position.y = positionY;
	position.z = positionZ;
}

constexpr Vector3 Movement::getPosition() const
{
	return position;
}


//constexpr Vector3 Movement::getForwardDirection() const
//{
//	return forwardDir;
//}
//
//constexpr Vector3 Movement::getRightDirection() const
//{
//	return rightDir;
//}


