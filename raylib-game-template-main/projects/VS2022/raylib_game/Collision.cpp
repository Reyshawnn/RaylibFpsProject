#include "Collision.h"

void Collision::groundCheck(Actor* actor)
{
    {
        if (actor->position.y <= 0.0f)
        {
            actor->position.y = 0.0f;
            actor->velocity.y = 0.0f;
            actor->isGrounded = true;
        }
    }
}

void Collision::collInit(Actor* actor,float dt)
{
    nextX =  actor->position.x + actor->velocity.x * dt;
    nextZ =  actor->position.z + actor->velocity.z * dt;


    actor->boxX.min =
    {
        nextX - 0.55f,
        actor->position.y,
        actor->position.z
    };

    actor->boxX.max =
    {
        nextX + 0.55f,
        actor->position.y,
        actor->position.z
    };

    actor->boxZ.min =
    {
        actor->position.x,
        actor->position.y,
        nextZ - 0.55f
    };

    actor->boxZ.max =
    {
        actor->position.x,
        actor->position.y,
        nextZ + 0.55f
    };

    actor->collideX = false;
    actor->collideZ = false;
}

void Collision::collInitAI(Actor* actor, float dt)
{
    nextX = actor->position.x + actor->velocity.x * dt;
    nextZ = actor->position.z + actor->velocity.z * dt;


    actor->boxX.min =
    {
        nextX - actor->size.x * 0.55f,
        actor->position.y - actor->size.y * 0.55f,
        actor->position.z - actor->size.y * 0.55f
    };

    actor->boxX.max =
    {
        nextX + actor->size.y * 0.55f,
        actor->position.y + actor->size.y * 0.55f,
        actor->position.z + actor->size.y * 0.55f
    };

    actor->boxZ.min =
    {
        actor->position.x - actor->size.x * 0.55f,
        actor->position.y - actor->size.y * 0.55f,
        nextZ - actor->size.y * 0.55f
    };

    actor->boxZ.max =
    {
        actor->position.x - actor->size.x * 0.55f,
        actor->position.y - actor->size.y * 0.55f,
        nextZ - actor->size.y * 0.55f
    };

    /*actor->boxX.min =
    {
        actor->position.x - actor->size.x * 0.55f,
        actor->position.y - actor->size.y * 0.55f,
        actor->position.z - actor->size.z * 0.55f
    };

    actor->boxX.max =
    {
        actor->position.x + actor->size.x * 0.55f,
        actor->position.y + actor->size.y * 0.55f,
        actor->position.z + actor->size.z * 0.55f
    };*/

    actor->collideX = false;
    actor->collideZ = false;
}

void Collision::collBuilding(Actor* actor, std::vector<Structure>& buildings)
{
    for (auto& t : buildings)
    {
        if (CheckCollisionBoxes(t.box, actor->boxX))
        {
            actor->collideX = true;
        }

        if (CheckCollisionBoxes(t.box, actor->boxZ))
        {
            actor->collideZ = true;
        }

        if (actor->collideX || actor->collideZ)
        {
            break;
        }
    }

}

void Collision::collUpdate(std::vector<Actor*>& actors,float dt)
{
    /*for(auto& actor: actors)
    {
        if (actor.collideX && actor.collideZ)
        {
            actor.position.y += actor.velocity.y * dt;
        }
        else if (actor.collideX && !actor.collideZ)
        {
            actor.position.y += actor.velocity.y * dt;
            actor.position.z = nextZ;
        }
        else if (actor.collideZ && !actor.collideX)
        {
            actor.position.x = nextX;
            actor.position.y += actor.velocity.y * dt;
        }
        else if (!actor.collideX && !actor.collideZ)
        {
            actor.position.x = nextX;
            actor.position.y += actor.velocity.y * dt;
            actor.position.z = nextZ;
        }

    }*/

    for (Actor* actor: actors)
    {

        if (actor->collideX && actor->collideZ)
        {
            actor->position.y += actor->velocity.y * dt;
        }
        else if (actor->collideX && !actor->collideZ)
        {
            actor->position.y += actor->velocity.y * dt;
            actor->position.z = nextZ;
        }
        else if (actor->collideZ && !actor->collideX)
        {
            actor->position.x = nextX;
            actor->position.y += actor->velocity.y * dt;
        }
        else if (!actor->collideX && !actor->collideZ)
        {
            actor->position.x = nextX;
            actor->position.y += actor->velocity.y * dt;
            actor->position.z = nextZ;
        }
    }

}


float Collision::getNextX() const
{
    return nextX;
}
float Collision::getNextZ() const
{
    return nextZ;
}


void Collision::bulletCheck(Target& target, Actor* actor,std::vector<Vector3>& locations, Actor& actor2)
{
    for (auto& bullet : actor->rocket.ammoList)
    {
        if ((CheckCollisionBoxes(bullet.box, target.box) && bullet.state != BulletState::idle))
        {
            int randNum{ GetRandomValue(0,3) };
            target.position = locations.at(randNum);
            target.updateTarget();
            bullet.state = BulletState::hit;
           // Send request to UI system hitString = std::format("Bullet Number {}, hit target!", bullet.bulletID);
        }

       
       


    }

    rayinfo = GetRayCollisionBox(actor->gun.ray, target.box);

    if (rayinfo.hit && actor->gun.state == rayState::fire)
    {
        int randNum{ GetRandomValue(0,3) };
        target.position = locations.at(randNum);
        target.updateTarget();
    }


    rayinfo = GetRayCollisionBox(actor->gun.ray, actor2.boxX);

    if (rayinfo.hit && actor->gun.state == rayState::fire)
    {
        std::cout << "hit enemy " << "\n";
    }

    rayinfo = GetRayCollisionBox(actor->gun.ray, actor2.boxZ);

    if (rayinfo.hit && actor->gun.state == rayState::fire)
    {
        std::cout << "hit enemy " << "\n";
    }




    
}


void Collision::bulletCheckAI(Actor* actor)
{
    for (auto& bullet : actor->rocket.ammoList)
    {
        
        if ((CheckCollisionBoxes(bullet.box, actor->enemies.at(0).boxX) && bullet.state != BulletState::idle) || (CheckCollisionBoxes(bullet.box, actor->enemies.at(0).boxZ) && bullet.state != BulletState::idle))
        {
            std::cout << bullet.position.x << " " << bullet.position.z << "\n";
            

        }
        std::cout << actor->enemies.at(0).boxX.max.x << "\n";

    }

    





}
void Collision::bulletWallCheck(Actor* actor, std::vector<Structure>& walls)
{
    for (auto& bullet : actor->rocket.ammoList)
    {
        for (auto& building : walls)
        {
            if (CheckCollisionBoxes(bullet.box, building.box) && bullet.state == BulletState::travel)
            {
                bullet.state = BulletState::hit;
            }
        }
    }
}


