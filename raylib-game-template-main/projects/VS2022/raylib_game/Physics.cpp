#include "Physics.h"
void Physics::horiVelo(Actor* actor, float dt)
{
    float resistance = actor->isGrounded ? FRICTION : AIR_DRAG;

    horizontalVelocity = Vector3{
        actor->velocity.x * resistance,
        0.0f,
        actor->velocity.z * resistance
    };

    if (Vector3Length(horizontalVelocity) < MAX_SPEED * 0.01f)
        horizontalVelocity = { 0 };
}


void Physics::jumpCheck(Actor* actor, float dt)
{
    if (!actor->isGrounded)
        actor->velocity.y -= GRAVITY * dt;

    if (actor->isGrounded && actor->jumpPressed)
    {
        actor->velocity.y = JUMP_FORCE;
        actor->isGrounded = false;
    }
}

void Physics::calcVelocity(Actor* actor, float dt)
{
    
    float currentSpeed = Vector3DotProduct(horizontalVelocity, actor->dir);

    float maxSpeed = actor->isCrouched ? CROUCH_SPEED : MAX_SPEED;

    float accel = Clamp(
        maxSpeed - currentSpeed,
        0.0f,
        MAX_ACCEL * dt
    );

    horizontalVelocity.x += actor->dir.x * accel;
    horizontalVelocity.z += actor->dir.z * accel;

    actor->velocity.x = horizontalVelocity.x;
    actor->velocity.z = horizontalVelocity.z;
}