#pragma once
#include "raylib.h"
#include "raymath.h"
#include "iostream"
#include <vector>





enum class BulletState
{
    idle,
    fired,
    travel,
    hit,
};

enum class weaponShape
{
    square,
    sphere,
    triangle,
    cylinder,
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

enum class FireType
{
    FullAuto,
    SemiAuto,
    Burst,
};



struct weaponInfo //Class for weapon info (name,firing style,texture,sound) etc
{
    std::string name;
    FireType type;
    //texture
    //sound data

    
};

struct tempWeapInfo //Throwaway class for sizes and color of weapon shapes
{
    Color color{ RED };
    float radiSize{ 15.0f };
    Vector3 vecSize{};
    weaponShape shape{weaponShape::sphere};
};

struct Weapon //maybe make this not a abstract base class to have a vector of weapons
{
    Vector3 position;
    Vector3 dir;
    tempWeapInfo info;
    virtual void fire() = 0;
    virtual void notifyIdle() = 0;
};

struct Launcher : public Weapon
{
    std::vector<Bullet> ammoList;
    std::string numBullets{};
    int currentBullet;
    int maxBullets;
    
    void fire() override;

    void notifyIdle() override //-> notify UI system of ammo count
    {
        int counter{};
        for (auto& bullet : ammoList)
        {
            if (bullet.state == BulletState::idle)
            {
                counter += 1;
            }
        }


        numBullets = static_cast<char>(counter + 48);
    }
};


struct AutomaticWeapon : public Weapon
{
    Ray ray;
    RayCollision rayInfo;
    void fire() override;
    void notifyIdle() override;


};




std::ostream& operator<<(std::ostream& out, BulletState state);