#pragma once
#include "raylib.h"
#include "Bullet.h"
#include "Input.h"
#include <array>



/*
TODO:
- Add a array of weapon objects intersting problem!!

*/
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


    //Collision Fields
    bool collideX;
    bool collideZ;
    BoundingBox boxX;
    BoundingBox boxZ;

    
    Input input;
    
    Launcher weapon; //object for drawing weapon in player model space
    std::vector<Launcher> inventory; 


    Camera camera;

    Vector2 lookRotation;
    Vector2 lean;
    std::string ID;

    void bulletHandle(int index);
    void setID(std::string_view name);
    void setupWeapon();
    void addWeapons();
    
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






