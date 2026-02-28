#pragma once
#include "raylib.h"
#include "Bullet.h"
#include <array>
struct Actor
{
    Vector3 position;
    Vector3 velocity;
    Vector3 dir;

    //Change these bools to a state machine PlayerState enum
    bool isGrounded;
    bool hasShot;
    bool isCrouched;
    bool jumpPressed;

    std::array<Bullet, 4> ammoList{};

    bool collideX;
    bool collideZ;
    float nextX;
    float nextZ;
    BoundingBox boxX;
    BoundingBox boxZ;
};

class Action //Abstract class for Actions
{
public:
    virtual void execute(Actor& actor) = 0;
    virtual ~Action() {};
};


class JumpAction : public Action
{
public:
    void execute(Actor& actor) override
    {
        //Function to jump
    }
};

class ShootAction : public Action
{
public:
    void execute(Actor& actor) override
    {
        //Function to Shoot 
    }
};


class SprintAction : public Action
{
public:
    void execute(Actor& actor) override
    {
        //Function to Sprint
    }
};



class CrouchAction : public Action
{
public:
    void execute(Actor& actor) override
    {
        //Function to 
    }
};






