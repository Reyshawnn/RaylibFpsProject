#include "Input.h"

void Input::setInputs(const char side, const char forward, bool jumpPressed)
{
	if (jumpPressed)
	{
		jump = true;
	}
	else
	{
		jump = false;
	}


	inputs.x = (float)side;
	inputs.y = (float)-forward;

#if defined(NORMALIZE_INPUT)
	if (side != 0 && forward != 0)
	{
		inputs = Vector2Normalize(inputs);
		return;
	}
#endif

	
	


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

/*constexpr*/ void Input::setFinalDirection(float dt)
{
	nextDir = Vector3{
			inputs.x * rightDir.x + inputs.y * forwardDir.x,
			0.0f,
			inputs.x * rightDir.z + inputs.y * forwardDir.z
	};

	finalDir = Vector3Lerp(nextDir, finalDir, CONTROL * dt);

	
}

/*constexpr*/ Vector3 Input::getFinalDirection() const
{
	return finalDir;
}

bool Input::getJump() const
{
	return jump;
}

// Put this in final update function body->dir = Vector3Lerp(body->dir, desiredDir, CONTROL * dt);

