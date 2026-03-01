#pragma once
#include "raylib.h"
#include "Actor.h"
#include "raymath.h"

// Movement constants
#define GRAVITY         32.0f
#define MAX_SPEED       20.0f
#define CROUCH_SPEED     5.0f
#define JUMP_FORCE      12.0f
#define MAX_ACCEL      150.0f
// Grounded drag
#define FRICTION         0.86f
// Increasing air drag, increases strafing speed
#define AIR_DRAG         0.98f
// Responsiveness for turning movement direction to looked direction
#define CROUCH_SPEED     5.0f

struct Actor;

class Physics
{
public:
	void calcVelocity(Actor* actor,float dt);
	void jumpCheck(Actor* actor,float dt);
	void horiVelo(Actor* actor, float dt); //constexpr soon?
	
	
private:
	Vector3 horizontalVelocity;
};

