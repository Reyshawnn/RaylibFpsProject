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
    float nextX{ actor->position.x + actor->velocity.x * dt };
    float nextZ{ actor->position.z + actor->velocity.z * dt };


    actor->boxX.min =
    {
        actor->nextX - 0.5f,
        actor->position.y,
        actor->position.z
    };

    actor->boxX.max =
    {
        actor->nextX + 0.5f,
        actor->position.y,
        actor->position.z
    };

    actor->boxZ.min =
    {
        actor->position.x,
        actor->position.y,
        actor->nextZ - 0.5f
    };

    actor->boxZ.max =
    {
        actor->position.x,
        actor->position.y,
        actor->nextZ + 0.5f
    };
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

void Collision::collUpdate(std::vector<Actor>& actors,float dt)
{
    for(auto& actor: actors)
    {
        if (actor.collideX && actor.collideZ)
        {
            actor.position.y += actor.velocity.y * dt;
        }
        else if (actor.collideX && !actor.collideZ)
        {
            actor.position.y += actor.velocity.y * dt;
            actor.position.z = actor.nextZ;
        }
        else if (actor.collideZ && !actor.collideX)
        {
            actor.position.x = actor.nextX;
            actor.position.y += actor.velocity.y * dt;
        }
        else if (!actor.collideX && !actor.collideZ)
        {
            actor.position.x = actor.nextX;
            actor.position.y += actor.velocity.y * dt;
            actor.position.z = actor.nextZ;
        }
    }
}