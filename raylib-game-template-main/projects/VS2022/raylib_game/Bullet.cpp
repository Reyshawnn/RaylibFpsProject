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

void Bullet::reset(Vector3 pos)
{
    position.x = pos.x;
    position.y = pos.y;
    position.z = pos.z;



    state = BulletState::idle;

}

void Launcher::fire() 
{
    if (ammoList.at(currentBullet).state == BulletState::idle)
    {
        ammoList.at(currentBullet).state = BulletState::fired;
        currentBullet++;

    }

    if (currentBullet > maxBullets - 1)
    {
        currentBullet = 0;
    }
}

void AutomaticWeapon::fire()
{
    state = rayState::fire;
}

void AutomaticWeapon::notifyIdle()
{

}

void AutomaticWeapon::reset()
{
    state = rayState::ready;
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

void Launcher::setMaxBullets(int max)
{
    maxBullets = max;
    maxBulletsStr = static_cast<char>(max + 48);
}
