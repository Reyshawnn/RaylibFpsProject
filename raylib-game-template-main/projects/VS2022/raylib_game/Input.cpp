#include "Input.h"

void Input::setInputs(const char side, const char forward)
{
#if defined(NORMALIZE_INPUT)
	if (side != 0 && forward != 0)
	{
		inputs = Vector2Normalize(inputs);
		return;
	}
#endif

	inputs.x = float(side);
	inputs.y = float(-forward);


}

Vector2 Input::getInputs() const
{
	return inputs;
}

void Input::setDirection(const float lookX)
{

	rightDir = Vector3{ cosf(-lookX),0.0f,sinf(-lookX) };
	forwardDir = Vector3{ sinf(lookX),0.0f,cosf(lookX) };
}

constexpr Vector3 Input::getRightDirection() const
{
	return rightDir;
}

constexpr Vector3 Input::getForwardDirection() const
{
	return forwardDir;
}

constexpr void Input::setFinalDirection()
{
	finalDir = Vector3{
			inputs.x * rightDir.x + inputs.y * forwardDir.x,
			0.0f,
			inputs.x * rightDir.z + inputs.y * forwardDir.z
	};
	
}


// Put this in final update function body->dir = Vector3Lerp(body->dir, desiredDir, CONTROL * dt);

