#include "raylib.h"

#include "raymath.h"

#include <iostream>

#include <array>

#include <vector>

#include "Random.h"

#include <format>

#include "Actor.h"


#include "Input.h"

#include "Physics.h"

#include "Collision.h"

#include "Map.h"

#include "Target.h"


/*

TODO LIST - 
Update code to use Actor.weapon object 

Update Bullet collision with walls (check collision then switch state) 

Update Game Type with drawing functions and whatnot 

Start on the UI class 



*/


//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Movement constants
#define GRAVITY         32.0f
#define MAX_SPEED       20.0f
#define CROUCH_SPEED     5.0f
#define JUMP_FORCE      12.0f
#define MAX_ACCEL      150.0f
// Grounded drag
#define FRICTION         0.86f
// Increasing air drag, increases strafing speed
#define AIR_DRAG         0.98f
// Responsiveness for turning movement direction to looked direction
#define CONTROL         15.0f
#define CROUCH_HEIGHT    0.0f
#define STAND_HEIGHT     1.0f
#define BOTTOM_HEIGHT    0.5f

#define NORMALIZE_INPUT  0

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Body structure



//struct Weapon //struct for weapon on screen
//{
//    Vector3 position;
//    Vector3 dir;
//
//};

//enum class BulletState
//{
//    idle,
//    fired,
//    travel,
//    hit,
//};
//
//struct Bullet
//{
//    Vector3 position{};
//    Vector3 velocity{ 55.0f,55.0f,55.0f };
//    Vector3 dir{};
//    BulletState state{ BulletState::idle };
//    Vector3 size{};
//    BoundingBox box;
//    int bulletID{};
//};

struct Tower
{
    Vector3 position;
    Vector3 size;
    Color color;
    int id;
    BoundingBox box;

};
//struct Body {
//    Vector3 position{};
//    Vector3 velocity{};
//    Vector3 dir{};
//    bool isGrounded{};
//    bool hasShot{};
//    BoundingBox box{};
//    Input input{};
//    Physics phys{};
//    std::array<Bullet, 4> ammoList{};
//
//};






// Weapon offset relative to camera (LOCAL space)
static const Vector3 WEAPON_OFFSET = {
    0.35f,  // right
   -0.30f,  // down
    0.90f   // forward
};

//
//DrawCubeV(Vector3{ -2.75f,8.15f,-55.0f }, Vector3{ 100.0f,17.0f,2.0f }, BROWN);
//DrawCubeV(Vector3{ 48.5f,8.15f,-5.00f }, Vector3{ 2.0f,17.0f,105.0f }, GRAY);
//DrawCubeV(Vector3{ 1.25f,8.15f,48.5f }, Vector3{ -108.0f,17.0f,2.0f }, BLACK);
//DrawCubeV(Vector3{ -55.25f,8.15f,1.25f }, Vector3{ 5.0f,17.0f,-110.50f }, SKYBLUE);


//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static Vector2 sensitivity = { 0.001f, 0.001f };

static Actor player = { 0 };
static Actor* playerPtr{ &player };
static Vector2 lookRotation = { 0 };
static float headTimer = 0.0f;
static float walkLerp = 0.0f;
static float headLerp = STAND_HEIGHT;
static Vector2 lean = { 0 };
static Weapon weapon{};
static Bullet currBullet{};
static bool bulletSpawn{ false };
static Physics PhysicsEngine{};
static Collision CollisionEngine{};
static std::vector<Actor*> actors{ playerPtr };

//Map
static const Vector3 towerSize = Vector3{ 16.0f, 32.0f, 16.0f };

static std::vector<Structure> towers{ 
Structure{{20.0f, 16.0f, 20.0f},towerSize,RED},
Structure{{-20.0f, 16.0f, 20.0f},towerSize,BLUE},
Structure{{20.0f, 16.0f, -20.0f},towerSize,GREEN},
Structure{{-20.0f, 16.0f, -20.0f},towerSize,YELLOW},
Structure{ { -2.75f, 8.15f, -55.0f },{ 100.0f, 17.0f, 2.0f },BROWN},
Structure{ { 48.5f, 8.15f, -5.0f },{ 2.0f, 17.0f, 105.0f }, GRAY},
Structure{{ 1.25f, 8.15f, 48.5f },{ 108.0f, 17.0f, 2.0f },BLACK, 3},
Structure{{ -55.25f, 8.15f, 1.25f },{ 5.0f, 17.0f, 110.5f },SKYBLUE, 4}};

//Collision globals
static bool hitTowerX{ false };
static bool hitTowerZ{ false };

static bool hitWallX{ false };
static bool hitWallZ{ false };



static Target t1{};
static std::vector<Vector3> targetLocations{ {10.0f,20.0f,5.0f},{20.0f,10.0f,20.0f}, {5.0f,0.0f,22.5f}, {0.0f,0.0f,0.0f} };
static std::string hitString{};

//Map -> make a .data file format (prob json)
static Structure wallOne{
    { -2.75f, 8.15f, -55.0f },
    { 100.0f, 17.0f, 2.0f },
    BROWN, 1
};

static Structure wallTwo{
    { 48.5f, 8.15f, -5.0f },
    { 2.0f, 17.0f, 105.0f },
    GRAY, 2
};

static Structure wallThree{
    { 1.25f, 8.15f, 48.5f },
    { 108.0f, 17.0f, 2.0f },   
    BLACK, 3
};

static Structure wallFour{
    { -55.25f, 8.15f, 1.25f },
    { 5.0f, 17.0f, 110.5f },   
    SKYBLUE, 4
};


static std::vector<Structure> walls{ wallOne,wallTwo,wallThree,wallFour };






//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void DrawLevel(void); //Map
static void UpdateCameraFPS(Camera* camera, Weapon* weapon); //Sys?
static void UpdateBody(Actor* actor, float yaw, char side, char forward, bool jumpPressed, bool crouchHold); //Game
void AttachWeaponToCamera(Weapon* weapon, const Camera& camera); //Game
void DrawCameraDebug(const Camera& camera, const Weapon& weapon); //Debug








//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization - Game class init() function
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;
    int framecount{};

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera fps");

    // Initialize camera variables
    // NOTE: UpdateCameraFPS() takes care of the rest
    Camera camera = { 0 };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.position = Vector3{
        player.position.x,
        player.position.y + (BOTTOM_HEIGHT + headLerp),
        player.position.z,
    };


   // currBullet.velocity = Vector3{ 60.0f,60.f,60.f };



    Vector3 towerPos = Vector3{ 20.0f, 16.0f, 20.0f };



    for (auto& t : towers) //setBoundingBox() function -> Map.h
    {
        t.box.min = { t.position.x - t.size.x * 0.5f,
        t.position.y - t.size.y * 0.5f,
         t.position.z - t.size.z * 0.5f
        };

        t.box.max = { t.position.x + t.size.x * 0.5f,
        t.position.y + t.size.y * 0.5f,
        t.position.z + t.size.z * 0.5f
        };
    }

    for (auto& w : walls)
    {
        w.box.min = { w.position.x - w.size.x * 0.5f,
               w.position.y - w.size.y * 0.5f,
               w.position.z - w.size.z * 0.5f
        };

        w.box.max = { w.position.x + w.size.x * 0.5f,
        w.position.y + w.size.y * 0.5f,
        w.position.z + w.size.z * 0.5f
        };


    }

    size_t ammoIndex{};

    UpdateCameraFPS(&camera, &weapon); // Update camera parameters



    t1.box.min =
    {
        t1.position.x - t1.size.x * 0.5f,
        t1.position.y - t1.size.y * 0.5f,
        t1.position.z - t1.size.z * 0.5f
    };

    t1.box.max =
    {
        t1.position.x + t1.size.x * 0.5f,
        t1.position.y + t1.size.y * 0.5f,
        t1.position.z + t1.size.z * 0.5f
    };


    for (size_t i{}; i < player.ammoList.size(); i++)
    {
        player.ammoList.at(i).bulletID = static_cast<int>(i);
    }



    DisableCursor();        // Limit cursor to relative movement inside the window

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update -- Multiple things are being updated Player Inputs, New position calculations,etc
        //----------------------------------------------------------------------------------
        Vector2 mouseDelta = GetMouseDelta();
        lookRotation.x -= mouseDelta.x * sensitivity.x;
        lookRotation.y += mouseDelta.y * sensitivity.y;





        //Input Handling
        char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
        char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
        bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
        


         


        UpdateBody(&player, lookRotation.x, sideway, forward, IsKeyPressed(KEY_SPACE),crouching);



        float delta = GetFrameTime();

        headLerp = Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT), 20.0f * delta);

        camera.position = Vector3{
            player.position.x,
            player.position.y + (BOTTOM_HEIGHT + headLerp),
            player.position.z,
        };

        if (player.isGrounded && ((forward != 0) || (sideway != 0)))
        {
            headTimer += delta * 3.0f;
            walkLerp = Lerp(walkLerp, 1.0f, 10.0f * delta);
            camera.fovy = Lerp(camera.fovy, 55.0f, 5.0f * delta);
        }
        else
        {
            walkLerp = Lerp(walkLerp, 0.0f, 10.0f * delta);
            camera.fovy = Lerp(camera.fovy, 60.0f, 5.0f * delta);
        }




        if (IsKeyPressed(KEY_ENTER)) //Input handling
        {

            if (player.ammoList.at(ammoIndex).state == BulletState::idle)
            {
                player.ammoList.at(ammoIndex).state = BulletState::fired;
                ammoIndex++;

            }

            if (ammoIndex > 3)
            {
                ammoIndex = 0;
            }

        }

        for (auto& bullet : player.ammoList) 
        {

            if (bullet.state == BulletState::idle)
            {
                continue;
            }
            else if (bullet.state == BulletState::fired)
            {
                bullet.dir = weapon.dir;
                bullet.position = weapon.position;
                bullet.state = BulletState::travel;
            }
            else if (bullet.state == BulletState::travel)
            {
                if (bullet.position.x >= 50.0f || bullet.position.x <= -50.0f)
                {

                    bullet.state = BulletState::hit;
                }
                else if (bullet.position.y >= 50.0f || bullet.position.y <= 0.0f)
                {

                    bullet.state = BulletState::hit;
                }

                else if (bullet.position.z >= 50.0f || bullet.position.z <= -50.0f)
                {

                    bullet.state = BulletState::hit;
                }

            }
            else if (bullet.state == BulletState::hit)
            {
                //something cool here maybe animation or something 
                bullet.state = BulletState::idle;
            }



        }


        lean.x = Lerp(lean.x, sideway * 0.02f, 10.0f * delta);
        lean.y = Lerp(lean.y, forward * 0.015f, 10.0f * delta);
    

        UpdateCameraFPS(&camera, &weapon);
        AttachWeaponToCamera(&weapon, camera);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);


        DrawLevel();

        DrawSphere(weapon.position, 0.15f, RED); //weapon draw -> player.h
        DrawCubeV(t1.position, t1.size, BLUE); //target draw -> 

        for (auto& bullet : player.ammoList) // ->Game.h
        {
            if (bullet.state == BulletState::travel)
            {
                DrawSphere(bullet.position, 1.0f, RED);
                
            }
        }












        //DrawCameraDebug(camera, weapon);
        EndMode3D();

        // Draw info box -- UI
        DrawRectangle(5, 5, 330, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(5, 5, 330, 100, BLUE);

        DrawText("Camera controls:", 15, 15, 10, BLACK);
        DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
        DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
        DrawText(TextFormat("- Velocity Len: (%06.3f)", Vector2Length(Vector2{ player.velocity.x, player.velocity.z })), 15, 60, 10, BLACK);
        DrawText("Last Target Hit: ", 15, 75, 10, BLACK);
        if (hitString.empty())
        {
            DrawText("N/A", 100, 75, 10, BLACK);
        }
        else
        {
        
            DrawText(hitString.c_str(), 100, 75, 10, BLACK);
        }



       
        
  
        EndDrawing();

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
// Update body considering current world state
static void UpdateBody(Actor* Actor, float yaw, char side, char forward, bool jumpPressed, bool crouchHold) //-> Game.h
{
    
    Actor->input.setInputs(side, forward,jumpPressed);
    float dt = GetFrameTime();
    Actor->jumpPressed = Actor->input.getJump();
    Actor->input.setDirection(yaw);
    Actor->input.setFinalDirection(dt);
    Actor->dir = Actor->input.getFinalDirection();

   


    PhysicsEngine.horiVelo(Actor, dt);
    PhysicsEngine.calcVelocity(Actor, dt);
   


  

    CollisionEngine.collInit(Actor, dt);
    CollisionEngine.collBuilding(Actor, towers);
    CollisionEngine.groundCheck(Actor);


    PhysicsEngine.jumpCheck(Actor, dt);
    CollisionEngine.collUpdate(actors, dt);
    
    
    for (auto& actor : actors)
    {
        CollisionEngine.bulletCheck(t1, actor, targetLocations);
        for (int i{}; i < actor->ammoList.size(); i++)
        {
           /* actor->bulletHandle(i);*/
            actor->ammoList.at(i).updateBullet();
        }
    }


}


// Update camera for FPS behaviour -->May remove because  I don't understand it lol
static void UpdateCameraFPS(Camera* camera, Weapon* weapon)
{
    const Vector3 up = Vector3{ 0.0f, 1.0f, 0.0f };
    const Vector3 targetOffset = Vector3{ 0.0f, 0.0f, -1.0f };

    // Left and right
    Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, lookRotation.x);

    // Clamp view up
    float maxAngleUp = Vector3Angle(up, yaw);
    maxAngleUp -= 0.001f; // Avoid numerical errors
    if (-(lookRotation.y) > maxAngleUp) { lookRotation.y = -maxAngleUp; }

    // Clamp view down
    float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
    maxAngleDown *= -1.0f; // Downwards angle is negative
    maxAngleDown += 0.001f; // Avoid numerical errors
    if (-(lookRotation.y) < maxAngleDown) { lookRotation.y = -maxAngleDown; }

    // Up and down
    Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

    // Rotate view vector around right axis
    float pitchAngle = -lookRotation.y - lean.y;
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

// Draw game level --> Game.h Will process data from Map Objects
static void DrawLevel(void)
{
    const int floorExtent = 100;
    const float tileSize = 5.0f;
    const Color tileColor1 = Color{ 150, 200, 200, 255 };

    //walls

    for (auto& w : walls)
    {
        DrawCubeV(w.position, w.size, w.color);
        DrawBoundingBox(w.box, YELLOW);
    }

    // Floor tiles
    for (int y = -floorExtent; y < floorExtent; y++)
    {
        for (int x = -floorExtent; x < floorExtent; x++)
        {
            if ((y & 1) && (x & 1))
            {
                DrawPlane(Vector3{ x * tileSize, 0.0f, y * tileSize }, Vector2{ tileSize, tileSize }, tileColor1);
            }
            else if (!(y & 1) && !(x & 1))
            {
                DrawPlane(Vector3{ x * tileSize, 0.0f, y * tileSize }, Vector2{ tileSize, tileSize }, LIGHTGRAY);
            }
        }
    }

    const Vector3 towerSize = Vector3{ 16.0f, 32.0f, 16.0f };
    const Color towerColor = Color{ 150, 200, 200, 255 };




    for (auto& t : towers)
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


static void shoot(Weapon* weapon)
{
    Vector3 bulletPos{ weapon->position };

    DrawSphere(bulletPos, 1.0f, RED);




}



void AttachWeaponToCamera(Weapon* weapon, const Camera& camera) //-> Game.h
{
    // --------------------------------------------------
    // 1. Build camera basis (WORLD SPACE)
    // --------------------------------------------------

    // Forward direction (camera look vector)
    Vector3 forward = Vector3Normalize(
        Vector3Subtract(camera.target, camera.position)
    );

    // Right direction
    Vector3 right = Vector3Normalize(
        Vector3CrossProduct(forward, camera.up)
    );

    // Up direction (already correct)
    Vector3 up = camera.up;

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
    weapon->position = Vector3Add(camera.position, worldOffset);

    // --------------------------------------------------
    // 4. Weapon forward direction (for shooting)
    // --------------------------------------------------
    weapon->dir = forward;
}

void DrawCameraDebug(const Camera& camera, const Weapon& weapon)
{
    const float AXIS_LEN = 2.0f;

    // Camera forward (RED)
    /*Vector3 forward = Vector3Normalize(
        Vector3Subtract(camera.target, camera.position)
    );*/

    Vector3 forward = { 0, 0, -1 };

    // Camera right (GREEN)
    Vector3 right = Vector3Normalize(
        Vector3CrossProduct(forward, camera.up)
    );

    // Camera up (BLUE)
    Vector3 up = Vector3Normalize(camera.up);


}



