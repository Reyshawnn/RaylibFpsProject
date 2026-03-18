#include "Actor.h"

void Actor::bulletHandle(int index)
{
    if (ammoList.at(index).state == BulletState::idle)
    {

        return;
    }
    else if (ammoList.at(index).state == BulletState::fired)
    {
        ammoList.at(index).dir = weapon.dir;
        ammoList.at(index).position = weapon.position;
        ammoList.at(index).state = BulletState::travel;
    }
    else if (ammoList.at(index).state == BulletState::travel)
    {
        if (ammoList.at(index).position.x >= 50.0f || ammoList.at(index).position.x <= -50.0f)
        {
            ammoList.at(index).reset();
            
        }
        else if (ammoList.at(index).position.y >= 50.0f || ammoList.at(index).position.y <= 0.0f)
        {

            ammoList.at(index).reset();
        }

        else if (ammoList.at(index).position.z >= 50.0f || ammoList.at(index).position.z <= -50.0f)
        {

            ammoList.at(index).reset();
        }

    }
    else if (ammoList.at(index).state == BulletState::hit)
    {
        //something cool here maybe animation or something 
        ammoList.at(index).state = BulletState::idle;
    }

}

void Actor::setID(std::string_view name)
{
    ID = name;
}