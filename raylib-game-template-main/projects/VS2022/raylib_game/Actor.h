#pragma once
#include "raylib.h"
#include "Bullet.h"
#include <array>
struct Actor
{
    Vector3 position{};
    Vector3 velocity{};
    Vector3 dir{};
    bool isGrounded{};
    bool hasShot{};
    BoundingBox box{};
    std::array<Bullet, 4> ammoList{};
};

