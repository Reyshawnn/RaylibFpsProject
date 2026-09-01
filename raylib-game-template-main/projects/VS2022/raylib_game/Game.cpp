#include "Game.h"



void Game::AttachWeaponToCamera(Actor* actor)
{
    Vector3 WEAPON_POS = {
    0.35f,  // right
   -0.30f,  // down
    0.90f   // forward
    };

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

    worldOffset = Vector3Add(worldOffset, Vector3Scale(right, WEAPON_POS.x));
    worldOffset = Vector3Add(worldOffset, Vector3Scale(up, WEAPON_POS.y));
    worldOffset = Vector3Add(worldOffset, Vector3Scale(forward, WEAPON_POS.z));

    // --------------------------------------------------
    // 3. Final weapon position
    // --------------------------------------------------
    actor->rocket.position = Vector3Add(actor->camera.position, worldOffset);
    actor->gun.ray.position = actor->rocket.position;

    // --------------------------------------------------
    // 4. Weapon forward direction (for shooting)
    // --------------------------------------------------
    actor->rocket.dir = forward;
    actor->gun.ray.direction = forward;

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
    float pitchAngle = -actor->lookRotation.y - actor->lean.y;
    pitchAngle = Clamp(pitchAngle, -PI / 2 + 0.0001f, PI / 2 - 0.0001f); // Clamp angle so it doesn't go past straight up or straight down
    Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

    // Head animation
    // Rotate up direction around forward axis
    float headSin = sinf(headTimer * PI);
    float headCos = cosf(headTimer * PI);
    const float stepRotation = 0.01f;
    actor->camera.up = Vector3RotateByAxisAngle(up, pitch, headSin * stepRotation + actor->lean.x);

    // Camera BOB
    const float bobSide = 0.1f;
    const float bobUp = 0.15f;
    Vector3 bobbing = Vector3Scale(right, headSin * bobSide);
    bobbing.y = fabsf(headCos * bobUp);

    actor->camera.position = Vector3Add(actor->camera.position, Vector3Scale(bobbing, walkLerp));
    actor->camera.target = Vector3Add(actor->camera.position, pitch);


}

void Game::UpdateCamera(Actor* actor, float dt)
{
    headLerp = Lerp(headLerp, (actor->isCrouched ? bottomHeight : standHeight), 20.0f * dt);

    actor->camera.position = Vector3{
        actor->position.x,
        actor->position.y + (bottomHeight + headLerp),
        actor->position.z,
    };
}


void Game::UpdateInputs(std::vector<Actor*>& actors, float yaw, char side, char forward, bool jumpPressed, bool crouchHold, float dt)
{
    for(auto& Actor: actors)
    {

        if (Actor->ID == "Player")
        {
            Actor->input.setInputs(side, forward, jumpPressed);
            Actor->jumpPressed = Actor->input.getJump();
            Actor->input.setDirection(yaw);
            Actor->input.setFinalDirection(dt);
            Actor->dir = Actor->input.getFinalDirection();
            return;
        }
    }
    
}

void Game::UpdatePhysandColl(std::vector<Actor*>& actors, Physics& PhysicsEngine, Collision& CollisionEngine, std::vector<Structure>& towers, float dt)
{
    for (auto& Actor : actors)
    {
        PhysicsEngine.horiVelo(Actor, dt);
        PhysicsEngine.calcVelocity(Actor, dt);





        CollisionEngine.collInit(Actor, dt);
        CollisionEngine.collBuilding(Actor, towers);
        CollisionEngine.groundCheck(Actor);


        PhysicsEngine.jumpCheck(Actor, dt);
        CollisionEngine.collUpdate(actors, dt);
        return;
    }
}

void Game::UpdateCollision(std::vector<Actor*>& actors, std::vector<Vector3>& locations, std::vector<Structure>& towers, Collision& CollisionEngine, Target& t1)
{
    for (auto& actor : actors)
    {
        CollisionEngine.bulletCheck(t1, actor, locations,actor->enemies.at(0));
        CollisionEngine.bulletWallCheck(actor, towers);
        for (int i{}; i < actor->rocket.ammoList.size(); i++)
        {
            actor->bulletHandle(i);
            actor->rocket.ammoList.at(i).updateBullet();
        }
    }
}

void Game::drawWeapon(std::vector<Actor*>& actors)
{
    for (auto& actor : actors)
    {
        switch (actor->currentWeapon->info.shape)
        {
        case weaponShape::square:
            DrawCubeV(actor->rocket.position, actor->rocket.info.vecSize, actor->currentWeapon->info.color);
            break;
        case weaponShape::sphere:
            DrawSphere(actor->rocket.position, actor->rocket.info.radiSize, actor->currentWeapon->info.color);
            break;
        /*case weaponShape::triangle:
            DrawTria*/
        }
    }
}


void Game::updateBullet(std::vector<Actor*>& actors)
{
    for (auto& actor : actors)
    {
        //if(actor->currentWeapon->info.type==weaponType::launcher)
       // {
            for (auto& bullet : actor->rocket.ammoList) // ->Game.h
            {
                if (bullet.state == BulletState::travel)
                {
                    DrawSphere(bullet.position, 1.0f, RED);

                }
            }
       // }
       // else if (actor->currentWeapon->info.type == weaponType::automatic)
      //  {
            if (actor->gun.state == rayState::fire)
            {
                DrawRay(actor->gun.ray, RED);
                
            }
       // }
    }
   
}







void Game::setupTiles(Map& map)
{
    const int floorExtent = 100;
    const float tileSize = 5.0f;
    const Color tileColor1 = Color{ 150, 200, 200, 255 };
   
    for (int y = -floorExtent; y < floorExtent; y++)
    {
        for (int x = -floorExtent; x < floorExtent; x++)
        {
            if ((y & 1) && (x & 1))
            {
                // Vector2 position{ x * tileSize,y * tileSize }; //Take this positions and store them as nodes 
                 //DrawPlane(Vector3{ x * tileSize, 0.0f, y * tileSize }, Vector2{ tileSize, tileSize }, tileColor1);
                if (map.tiles.size() < 100000)
                {
                    map.tiles.push_back(Tile{ Vector3{ x * tileSize, 0.0f, y * tileSize },Vector2{5.0f,5.0f},RED, idcount });
                    idcount++;
                }
                

                //DrawTexture(map.textures.at(0), position.x, position.y, GREEN);
            }
            else if (!(y & 1) && !(x & 1))
            {
                // Vector2 position{ x * tileSize,y * tileSize };
                // DrawPlane(Vector3{ x * tileSize, 0.0f, y * tileSize }, Vector2{ tileSize, tileSize }, LIGHTGRAY);
                if (map.tiles.size() < 100000)
                {
                    map.tiles.push_back(Tile{ Vector3{ x * tileSize, 0.0f, y * tileSize },Vector2{5.0f,5.0f},BLUE,idcount });
                    idcount++;
                }
                
                //DrawTexture(map.textures.at(0), position.x, position.y, GREEN);
            }

            else
            {
                if (map.tiles.size() < 100000)
                {
                    map.tiles.push_back(Tile{ Vector3{ x * tileSize, 0.0f, y * tileSize },Vector2{5.0f,5.0f},GREEN,idcount });
                    idcount++;
                }
            }
        }
    }
}
void Game::drawMap(Map& map)
{
        const int floorExtent = 100;
        const float tileSize = 5.0f;
        const Color tileColor1 = Color{ 150, 200, 200, 255 };
        int idcount{};

        //walls

        for (auto& w : map.walls)
        {
            DrawCubeV(w.position, w.size, w.color);
            DrawBoundingBox(w.box, YELLOW);
        }

        // Floor tiles
       
      /*  for (const Tile& tile : map.tiles)
        {
            DrawPlane(tile.position,tile.size,tile.color);
        }*/

        /*for (auto& floor : map.tiles)
        {
            DrawTexture(map.textures.at(0),)
        }*/

        const Vector3 towerSize = Vector3{ 16.0f, 32.0f, 16.0f }; 
        const Color towerColor = Color{ 150, 200, 200, 255 };




        for (auto& t : map.buildings)
        {
            DrawCubeV(t.position, t.size, t.color);
            //DrawCubeV(t.box.max, t.size, t.color);
            DrawBoundingBox(t.box, BLACK);
        }


        /*for (auto& w : walls)
        {
            DrawLine3D(w.box.min, w.box.max, YELLOW);
        }*/


        // Red sun
        DrawSphere(Vector3{ 300.0f, 300.0f, 0.0f }, 100.0f, Color{ 255, 0, 0, 255 });

        
    }

void Game::drawEnemies(Actor& actor)
{
    DrawCubeV(actor.position, Vector3{5.0f,10.0f,5.0f}, RED);
}