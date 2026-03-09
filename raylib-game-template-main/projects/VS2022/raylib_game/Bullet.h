#pragma once
#include "raylib.h"
#include "raymath.h"




enum class BulletState
{
    idle,
    fired,
    travel,
    hit,
};

struct Bullet
{
    Vector3 position{};
    Vector3 velocity{ 55.0f,55.0f,55.0f };
    Vector3 dir{};
    BulletState state{ BulletState::idle };
    Vector3 size{};
    BoundingBox box;
    int bulletID{};
    void updateBullet();

};

struct Weapon
{
    Vector3 position;
    Vector3 dir;
};
