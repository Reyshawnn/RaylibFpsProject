#pragma once
#include "raylib.h"
#include "Bullet.h"
#include "Input.h"
#include <array>

#define BOTTOM_HEIGHT    0.5f



struct Actor
{
    //Actor primitive fields
    Vector3 position;
    Vector3 velocity;
    Vector3 dir;

    //Change these bools to a state machine PlayerState enum
    bool isGrounded;
    bool hasShot;
    bool isCrouched;
    bool jumpPressed;

    std::array<Bullet, 4> ammoList;

    //Collision Fields
    bool collideX;
    bool collideZ;
    BoundingBox boxX;
    BoundingBox boxZ;

    
    Input input;
    
    Weapon weapon; //object for drawing weapon in player model space

    Camera camera;

    Vector2 lookRotation;

    void bulletHandle(int index);
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






