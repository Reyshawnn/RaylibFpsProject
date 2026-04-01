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


enum class weaponType
{
    automatic,
    launcher,
};

enum class rayState
{
    ready,
    fire,
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
    weaponShape shape{ weaponShape::sphere };
    weaponType type{};
    Color color{};
    float radiSize{ 0.15f };
    Vector3 vecSize{ 0.15f ,0.15f ,0.15f };
    
};

struct Weapon //maybe make this not a abstract base class to have a vector of weapons
{
    Vector3 position{};
    Vector3 dir{};
    tempWeapInfo info{};
    std::string name{};
    Weapon(weaponShape s, weaponType t, Color c, std::string n)
        :info{ s,t,c },
         name{ n }
    {

    }
    virtual void fire() = 0;
    virtual void notifyIdle() = 0;
};

struct Launcher : public Weapon
{
    
public:
    Launcher()
        :Weapon(weaponShape::sphere,weaponType::launcher,RED, "Weapon: Rocket Launcher")
    {
        
    }

    void fire() override;
    void setMaxBullets(int max);
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


        currentBulletsStr = static_cast<char>(counter + 48);
    }
//private -> make getter functions
    std::vector<Bullet> ammoList;
    std::string currentBulletsStr{};
    std::string maxBulletsStr{};
    int currentBullet{};
    int maxBullets{};
    

    
};


struct AutomaticWeapon : public Weapon
{
public:
    AutomaticWeapon()
        :Weapon(weaponShape::square,weaponType::automatic,BLUE, "Weapon: Semi Auto Rifle")
    {

    }
    void fire() override;
    void notifyIdle() override;
    void reset();

//private
    Ray ray;
    rayState state{};
};




std::ostream& operator<<(std::ostream& out, BulletState state);