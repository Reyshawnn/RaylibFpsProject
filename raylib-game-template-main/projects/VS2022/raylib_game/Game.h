#pragma once
#include "raylib.h"
#include "raymath.h"
#include "Actor.h"
#include "Physics.h"
#include "Collision.h"
#include "Bullet.h"
#include <vector>

class Game
{
public:
	void AttachWeaponToCamera(Actor* actor);
	void cameraSetup(Actor* actor);
	void UpdateCameraFPS(Actor* actor);
	void UpdateCamera(Actor* actor, float dt);
	void UpdateInputs(std::vector<Actor*>& actors,float yaw, char side, char forward, bool jumpPressed, bool crouchHold, float dt);
	void UpdatePhysandColl(std::vector<Actor*>& actors,Physics& PhysicsEngine,Collision& CollisionEngine,std::vector<Structure>& towers,float dt);
	void UpdateCollision(std::vector<Actor*>& actors,std::vector<Vector3>& locations ,std::vector<Structure>& towers,Collision& CollisionEngine, Target& t1);
	void drawWeapon(std::vector<Actor*>& actors);
	void updateBullet(std::vector<Actor*>& actors);


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

