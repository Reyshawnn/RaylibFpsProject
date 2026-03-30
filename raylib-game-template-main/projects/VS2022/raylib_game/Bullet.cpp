#include "Bullet.h"


void Bullet::updateBullet()
{
    float dt{ GetFrameTime() };

    if(state == BulletState::travel)
    {
        position += (dir * velocity * dt);
    }

    box.min = { position.x - size.x * 0.5f,
    position.y - size.y * 0.5f,
    position.z - size.z * 0.5f
    };

    box.max = { position.x + size.x * 0.5f,
    position.y + size.y * 0.5f,
    position.z + size.z * 0.5f
    };
    
}


std::ostream& operator<<(std::ostream& out, const Bullet& bullet)
{
    out << bullet.position.x << " " << bullet.position.y << " " << bullet.position.z;
    return out;
}

void Bullet::reset()
{
    position.x = 0.0f;
    position.y = 0.0f;
    position.z = 0.0f;



    state = BulletState::idle;

}

void Launcher::fire() 
{
    if (ammoList.at(currentBullet).state == BulletState::idle)
    {
        ammoList.at(currentBullet).state = BulletState::fired;
        currentBullet++;

    }

    if (currentBullet > maxBullets)
    {
        currentBullet = 0;
    }
}

void AutomaticWeapon::fire()
{

}

void AutomaticWeapon::notifyIdle()
{

}

std::ostream& operator<<(std::ostream& out, BulletState state)
{
    switch (state)
    {
    case BulletState::idle:
        out << "idle";
        return out;

    case BulletState::fired:
        out << "fired";
        return out;

    case BulletState::travel:
        out << "travel";
        return out;

    case BulletState::hit:
        out << "hit";
        return out;

    }
}