#pragma once
#include "raylib.h"
#include "Actor.h"
#include "Map.h"

class Collision
{
public:
	void groundCheck(Actor* actor);
    void collInit(Actor* actor,float dt);
	void collBuilding(Actor* actor, std::vector<Structure>&buildings);
	void collUpdate(std::vector<Actor>& actors, float dt);
};

