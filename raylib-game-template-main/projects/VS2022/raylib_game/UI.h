#pragma once
#include "raylib.h"
#include "string"
#include "raymath.h"

class UI
{
public:
	void drawAmmoUI(std::string& string) const;
	void drawMapUI() const;
	void drawExtraHudUI(Vector3 velData, std::string& hitString) const;
	

};

