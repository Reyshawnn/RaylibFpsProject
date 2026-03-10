#include "Game.h"
void Game::AttachWeaponToCamera(Actor* actor)
{
    // --------------------------------------------------
   // 1. Build camera basis (WORLD SPACE)
   // --------------------------------------------------

   // Forward direction (camera look vector)
    Vector3 forward = Vector3Normalize(
        Vector3Subtract(actor->camera.target, actor->camera.position)
    );

    // Right direction
    Vector3 right = Vector3Normalize(
        Vector3CrossProduct(forward, actor->camera.up)
    );

    // Up direction (already correct)
    Vector3 up = actor->camera.up;

    // --------------------------------------------------
    // 2. Convert local offset → world offset
    // --------------------------------------------------
    Vector3 worldOffset = { 0 };

    worldOffset = Vector3Add(worldOffset, Vector3Scale(right, WEAPON_OFFSET.x));
    worldOffset = Vector3Add(worldOffset, Vector3Scale(up, WEAPON_OFFSET.y));
    worldOffset = Vector3Add(worldOffset, Vector3Scale(forward, WEAPON_OFFSET.z));

    // --------------------------------------------------
    // 3. Final weapon position
    // --------------------------------------------------
    actor->weapon.position = Vector3Add(actor->camera.position, worldOffset);

    // --------------------------------------------------
    // 4. Weapon forward direction (for shooting)
    // --------------------------------------------------
    actor->weapon.dir = forward;
}

void Game::cameraSetup(Actor* actor)
{
    actor->camera.fovy = 60.0f;
    actor->camera.projection = CAMERA_PERSPECTIVE;
    actor->camera.position = Vector3{
        actor->position.x,
        actor->position.y + (bottomHeight + standHeight),
        actor->position.z,
    };
}

void Game::UpdateCameraFPS(Actor* actor)
{
    const Vector3 up = Vector3{ 0.0f, 1.0f, 0.0f };
    const Vector3 targetOffset = Vector3{ 0.0f, 0.0f, -1.0f };

    // Left and right
    Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, actor->lookRotation.x);

    // Clamp view up
    float maxAngleUp = Vector3Angle(up, yaw);
    maxAngleUp -= 0.001f; // Avoid numerical errors
    if (-(actor->lookRotation.y) > maxAngleUp) { actor->lookRotation.y = -maxAngleUp; }

    // Clamp view down
    float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
    maxAngleDown *= -1.0f; // Downwards angle is negative
    maxAngleDown += 0.001f; // Avoid numerical errors
    if (-(actor->lookRotation.y) < maxAngleDown) { actor->lookRotation.y = -maxAngleDown; }

    // Up and down
    Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

    // Rotate view vector around right axis
    float pitchAngle = -actor->lookRotation.y - lean.y;
    pitchAngle = Clamp(pitchAngle, -PI / 2 + 0.0001f, PI / 2 - 0.0001f); // Clamp angle so it doesn't go past straight up or straight down
    Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

    // Head animation
    // Rotate up direction around forward axis
    float headSin = sinf(headTimer * PI);
    float headCos = cosf(headTimer * PI);
    const float stepRotation = 0.01f;
    camera->up = Vector3RotateByAxisAngle(up, pitch, headSin * stepRotation + lean.x);

    // Camera BOB
    const float bobSide = 0.1f;
    const float bobUp = 0.15f;
    Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
    bobbing.y = fabsf(headCos * bobUp);

    camera->position = Vector3Add(camera->position, Vector3Scale(bobbing, walkLerp));
    camera->target = Vector3Add(camera->position, pitch);


}