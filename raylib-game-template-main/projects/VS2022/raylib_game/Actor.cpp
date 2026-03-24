#include "Actor.h"

void Actor::bulletHandle(int index)
{
    if (weapon.ammoList.at(index).state == BulletState::idle)
    {

        return;
    }
    else if (weapon.ammoList.at(index).state == BulletState::fired)
    {
        weapon.ammoList.at(index).dir = weapon.dir;
        weapon.ammoList.at(index).position = weapon.position;
        weapon.ammoList.at(index).state = BulletState::travel;
    }
    else if (weapon.ammoList.at(index).state == BulletState::travel)
    {
        if (weapon.ammoList.at(index).position.x >= 50.0f || weapon.ammoList.at(index).position.x <= -50.0f)
        {
            weapon.ammoList.at(index).reset();
            
        }
        else if (weapon.ammoList.at(index).position.y >= 50.0f || weapon.ammoList.at(index).position.y <= 0.0f)
        {

            weapon.ammoList.at(index).reset();
        }

        else if (weapon.ammoList.at(index).position.z >= 50.0f || weapon.ammoList.at(index).position.z <= -50.0f)
        {

            weapon.ammoList.at(index).reset();
        }

    }
    else if (weapon.ammoList.at(index).state == BulletState::hit)
    {
        //something cool here maybe animation or something 
        weapon.ammoList.at(index).state = BulletState::idle;
    }

}

void Actor::setID(std::string_view name)
{
    ID = name;
}

void Actor::setupWeapon()
{
    for (int i{}; i < 4; i++)
    {
        weapon.ammoList.push_back(Bullet{});
    }

}