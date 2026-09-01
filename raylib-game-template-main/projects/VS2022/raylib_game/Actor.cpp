#include "Actor.h"

void Actor::bulletHandle(int index)
{
    if (rocket.ammoList.at(index).state == BulletState::idle)
    {

        return;
    }
    else if (rocket.ammoList.at(index).state == BulletState::fired)
    {
        rocket.ammoList.at(index).dir = rocket.dir;
        rocket.ammoList.at(index).position = rocket.position;
        rocket.ammoList.at(index).state = BulletState::travel;
    }
    else if (rocket.ammoList.at(index).state == BulletState::travel)
    {
        if (rocket.ammoList.at(index).position.x >= 50.0f || rocket.ammoList.at(index).position.x <= -50.0f)
        {
            rocket.ammoList.at(index).reset(position);
            
        }
        else if (rocket.ammoList.at(index).position.y >= 50.0f || rocket.ammoList.at(index).position.y <= 0.0f)
        {

            rocket.ammoList.at(index).reset(position);
        }

        else if (rocket.ammoList.at(index).position.z >= 50.0f || rocket.ammoList.at(index).position.z <= -50.0f)
        {

            rocket.ammoList.at(index).reset(position);
        }

    }
    else if (rocket.ammoList.at(index).state == BulletState::hit)
    {
        //something cool here maybe animation or something 
        rocket.ammoList.at(index).state = BulletState::idle;
    }

}

void Actor::setID(std::string_view name)
{
    ID = name;
}

void Actor::setupWeapon()
{
    for (int i{}; i < rocket.maxBullets; i++)
    {
        rocket.ammoList.push_back(Bullet{});
    }

}


void Actor::addWeapons()
{
    //inventory.push_back(&weapon)
    currentWeapon = &rocket;
}


void Actor::switchWeapons()
{
    if (currentWeapon->info.type == weaponType::launcher)
    {
        currentWeapon = &gun;
    }
    else if (currentWeapon->info.type == weaponType::automatic)
    {
        currentWeapon = &rocket;
    }
        
}

void Actor::setSize(Vector3 size1)
{
    size = size1;
}
