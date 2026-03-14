#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Actor.h"

class Game
{
public:
	void AttachWeaponToCamera(Actor* actor);
	void cameraSetup(Actor* actor);
	void UpdateCameraFPS(Actor* actor);
	void UpdateCamera(Actor* actor, float dt);

private:
	float standHeight{ 1.0f };
	float bottomHeight{ 0.5f };
	float headTimer{ 0.0f };
	float walkLerp{ 0.0f };
	float headLerp{ 1.0f};

};

//Vector3 WEAPON_POS = {
//	0.35f,  // right
//   -0.30f,  // down
//	0.90f   // forward
//};

