#pragma once
#include "raylib.h"
#include "Actor.h"
#include "Map.h"
#include "Target.h"

//Change std::vector parameters for actors Structures can stay 
class Collision
{
public:
	void groundCheck(Actor* actor);
    void collInit(Actor* actor,float dt); 
	void collBuilding(Actor* actor, std::vector<Structure>&buildings);
	void collUpdate(std::vector<Actor*>& actors, float dt);
	void bulletCheck(Target& target, Actor* actor, std::vector<Vector3>& locations);
	float getNextX() const;
	float getNextZ() const;


private:
	float nextX;
	float nextZ;
};

