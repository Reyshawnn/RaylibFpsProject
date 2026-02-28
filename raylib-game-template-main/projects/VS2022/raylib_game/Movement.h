#pragma once
#include "raylib.h"
class Movement //final dir and final position
{
public:
	constexpr void setDirection(const Vector3& dir);
	constexpr Vector3 getRightDirection() const;
	constexpr Vector3 getForwardDirection() const;
	constexpr void setPosition(const float positionX, const float positionY, const float positionZ);
	constexpr Vector3 getPosition() const;



private:
	Vector3 finalDir;
	Vector3 position;
	

};

