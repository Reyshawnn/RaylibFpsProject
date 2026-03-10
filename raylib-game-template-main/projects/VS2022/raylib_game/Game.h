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
private:
	float standHeight{ 1.0f };
	float bottomHeight{ 0.5f };

};

Vector3 WEAPON_OFFSET = {
	0.35f,  // right
   -0.30f,  // down
	0.90f   // forward
};

