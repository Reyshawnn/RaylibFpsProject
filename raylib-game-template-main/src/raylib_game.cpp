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

#include "Game.h"


/*

TODO LIST - 

Start on the UI class 

Command design pattern implementation 

A 2nd weapon (Maybe draw a blue cube) and give it a ray collision implementation (like a automatic weapon) 

add basic sound / textures

Put the draw level class into Game 

Research making my own map files 


*/

struct Tower
{
    Vector3 position;
    Vector3 size;
    Color color;
    int id;
    BoundingBox box;

};









//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static Vector2 sensitivity = { 0.001f, 0.001f };

static Actor player = { 0 };
static Actor* playerPtr{ &player };
static Vector2 lookRotation = { 0 };
static Vector2 lean = { 0 };
static Weapon weapon{};
static Physics PhysicsEngine{};
static Collision CollisionEngine{};
static std::vector<Actor*> actors{ playerPtr };
static Game game{};

//Map
static const Vector3 towerSize = Vector3{ 16.0f, 32.0f, 16.0f };

static std::vector<Structure> towers{ 
//Towers
Structure{{20.0f, 16.0f, 20.0f},towerSize,RED},
Structure{{-20.0f, 16.0f, 20.0f},towerSize,BLUE},
Structure{{20.0f, 16.0f, -20.0f},towerSize,GREEN},
Structure{{-20.0f, 16.0f, -20.0f},towerSize,YELLOW},
//Walls
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


    game.cameraSetup(&player);




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

    game.UpdateCameraFPS(&player);



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

    player.setID("Player");

    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update -- Multiple things are being updated Player Inputs, New position calculations,etc
        //----------------------------------------------------------------------------------
        Vector2 mouseDelta = GetMouseDelta();
        float delta = GetFrameTime();
        player.lookRotation.x -= mouseDelta.x * sensitivity.x;
        player.lookRotation.y += mouseDelta.y * sensitivity.y;





        //Input Handling
        char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
        char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
        bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
        


         
        game.UpdateInputs(actors,player.lookRotation.x, sideway, forward, IsKeyPressed(KEY_SPACE), crouching, delta);
        game.UpdatePhysandColl(actors, PhysicsEngine, CollisionEngine, towers, delta);
        game.UpdateCollision(actors, targetLocations, towers, CollisionEngine, t1);
        game.UpdateCamera(&player, delta);





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

 


        player.lean.x = Lerp(player.lean.x, sideway * 0.02f, 10.0f * delta);
        player.lean.y = Lerp(player.lean.y, forward * 0.015f, 10.0f * delta);
    

        game.UpdateCameraFPS(&player);
        game.AttachWeaponToCamera(&player);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player.camera);


        DrawLevel();

        DrawSphere(player.weapon.position, 0.15f, RED); //weapon draw -> player.h
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


        std::cout << player.ammoList.at(0).state << "\n";
       
        
  
        EndDrawing();

        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
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






