#pragma once
#include "raylib.h"
#include "raymath.h"
#include "iostream"




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
    friend std::ostream& operator<<(std::ostream&, const Bullet& bullet);
    void reset();


};

struct Weapon
{
    Vector3 position;
    Vector3 dir;
};

std::ostream& operator<<(std::ostream& out, BulletState state);