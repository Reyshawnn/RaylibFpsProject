#pragma once
#include "raylib.h"
#include "raymath.h"

#define NORMALIZE_INPUT  0
#define CONTROL         15.0f

class Input
{
public:
	Vector2 getInputs() const;
	void setInputs(const char side, const char forward);
	void setDirection(const float lookX);
	constexpr Vector3 getRightDirection() const;
	constexpr Vector3 getForwardDirection() const;
	/*constexpr*/ Vector3 getFinalDirection() const;
	/*constexpr*/ void setFinalDirection(float dt);
	

private:
	Vector2 inputs;
	Vector3 rightDir;
	Vector3 forwardDir;
	Vector3 finalDir;
	Vector3 nextDir;
};
 
