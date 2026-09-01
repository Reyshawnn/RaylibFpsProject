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

#include "UI.h"

#include "Debug.h"




/*

TODO LIST - 
 
     very soon - 
     how does weapon.dir get its dir again?
     Create the AOE effect of the rocket launcher
     make it more automatic type
     Rocket launcher and auto rifles need to act differently one held enter and one pressed enter
     add basic sound / textures
     make some complex shapes like ramps and stairs




     near future-
     Command design pattern implementation

    Future - 
    Put the draw level class into Game 
    Research making my own map files 
    create conversion function from weaponType to std::string for UI system

*/











//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static Vector2 sensitivity = { 0.001f, 0.001f };

static Actor player = { 0 };
static Actor* playerPtr{ &player };
static Actor enemy{};
static Actor* enemyPtr{ &enemy };
static Vector2 lookRotation = { 0 };
static Vector2 lean = { 0 };
static Physics PhysicsEngine{};
static Collision CollisionEngine{};
static std::vector<Actor*> actors{ playerPtr};
static std::vector<Actor*> actorsA{ enemyPtr };
static Game game{};
static UI ui{};

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






//target cube
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
Model mod{};
Map world{};











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

    InitWindow(screenWidth, screenHeight, "ReyshawnnFPS");


    game.cameraSetup(&player);




    Vector3 towerPos = Vector3{ 20.0f, 16.0f, 20.0f };



    //for (auto& t : towers) //setBoundingBox() function -> Map.h
    //{
    //    t.box.min = { t.position.x - t.size.x * 0.5f,
    //    t.position.y - t.size.y * 0.5f,
    //     t.position.z - t.size.z * 0.5f
    //    };

    //    t.box.max = { t.position.x + t.size.x * 0.5f,
    //    t.position.y + t.size.y * 0.5f,
    //    t.position.z + t.size.z * 0.5f
    //    };
    //}

    //for (auto& w : walls)
    //{
    //    w.box.min = { w.position.x - w.size.x * 0.5f,
    //           w.position.y - w.size.y * 0.5f,
    //           w.position.z - w.size.z * 0.5f
    //    };

    //    w.box.max = { w.position.x + w.size.x * 0.5f,
    //    w.position.y + w.size.y * 0.5f,
    //    w.position.z + w.size.z * 0.5f
    //    };


    //}
    std::string fileString{ "C:/Users/chido/OneDrive/Desktop/git fps/raylib-game-template-main/projects/VS2022/build/raylib_game/bin/x64/Debug/assets/textures/grass.jpg" };
    world.setBuildings(towers);
    world.setWalls(walls);
   /* world.addTexture(fileString);
    
    for (auto& floor : world.tiles)
    {
        floor.setTexture(world.textures.at(0));
    }*/

    size_t ammoIndex{};

    game.UpdateCameraFPS(&player);
    game.setupTiles(world);
    world.sortTiles();



    //Put this in a function in Target
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


    for (size_t i{}; i < player.rocket.ammoList.size(); i++)
    {
        player.rocket.ammoList.at(i).bulletID = static_cast<int>(i);
    }

    enemy.position = { 0.0f,0.0f,-50.0f };
  

    DisableCursor();        // Limit cursor to relative movement inside the window

    player.setID("Player");
    player.rocket.setMaxBullets(7);

    player.setupWeapon();

    player.addWeapons();


    enemy.setID("enemy");
    enemy.rocket.setMaxBullets(7);

    enemy.setupWeapon();

    enemy.addWeapons();

    enemy.setSize(Vector3{ 5.0f,10.0f,5.0f });

    player.enemies.push_back(enemy);
    enemy.enemies.emplace_back(player);
   


    SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update -- Multiple things are being updated Player Inputs, New position calculations,etc
        //----------------------------------------------------------------------------------
        Vector2 mouseDelta = GetMouseDelta(); //->Maybe bundle this in input object
        float delta = GetFrameTime();
        player.lookRotation.x -= mouseDelta.x * sensitivity.x;
        player.lookRotation.y += mouseDelta.y * sensitivity.y;





        //Input Handling
        char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
        char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
        bool crouching = IsKeyDown(KEY_LEFT_CONTROL);

        
        

        if (IsKeyPressed(KEY_ENTER))
        {
            /*
            maybe have to check weapon type and handle resetting shoot state from there
            so maybe IsKeyPressed && currentweapon = semi auto fire
            and isKeyHeld && currentweapon = full auto etc
            */

            player.currentWeapon->fire();

        }

        if (IsKeyPressed(KEY_P))
        {
            player.switchWeapons();
        }
         
      
        //player version 
        game.UpdateInputs(actors,player.lookRotation.x, sideway, forward, IsKeyPressed(KEY_SPACE), crouching, delta);
        game.UpdatePhysandColl(actors, PhysicsEngine, CollisionEngine, world.allStructures, delta);
        game.UpdateCollision(actors, targetLocations, world.allStructures, CollisionEngine, t1);
        game.UpdateCamera(&player, delta);
        
        //AI version
        CollisionEngine.collInitAI(&enemy, delta);
        CollisionEngine.bulletCheckAI(&player);


       
        

       




 

        //Do this in a game function for scalability 
        player.lean.x = Lerp(player.lean.x, sideway * 0.02f, 10.0f * delta);
        player.lean.y = Lerp(player.lean.y, forward * 0.015f, 10.0f * delta);
    

        game.UpdateCameraFPS(&player);
        game.AttachWeaponToCamera(&player);

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        // -> Could make this section two functions
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(player.camera);


       // DrawLevel(); //-> Game.h
        game.drawMap(world);

        game.drawWeapon(actors);
        //DrawSphere(player.rocket.position, player.rocket.info.radiSize, player.rocket.info.color);

        DrawCubeV(t1.position, t1.size, BLUE); //target draw -> Game.h
       

        game.updateBullet(actors);
        game.drawEnemies(enemy);
        DrawBoundingBox(enemy.boxX, YELLOW);
       // DrawBoundingBox(enemy.boxZ, YELLOW);










        //DrawCameraDebug(camera, weapon);
        EndMode3D();

        // Draw info box -- UI
        
        player.rocket.notifyIdle();
        ui.drawAmmoUI(player.rocket.currentBulletsStr, player.rocket.maxBulletsStr,player.currentWeapon->name);
        


        /*DrawRectangle(5, 475, 330, 100, Fade(SKYBLUE, 0.5f));
        DrawRectangleLines(5, 475, 330, 100, BLUE);*/

        ui.drawExtraHudUI(player.velocity, hitString);


        ui.drawMapUI();

        //Top box
        //DrawText("Camera controls:", 15, 15, 10, BLACK);
        //DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
        //DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
        //DrawText(TextFormat("- Velocity Len: (%06.3f)", Vector2Length(Vector2{ player.velocity.x, player.velocity.z })), 15, 60, 10, BLACK);
        //DrawText("Last Target Hit: ", 15, 75, 10, BLACK);
        //if (hitString.empty())
        //{
        //    DrawText("N/A", 100, 75, 10, BLACK);
        //}
        //else
        //{
        //
        //    DrawText(hitString.c_str(), 100, 75, 10, BLACK);
        //}


        ////AmmoBox
        //DrawText("Weapon: Rocket Launcher",5,475,20,BLACK);
        //DrawText("/4", 20, 500, 20, BLACK);
        //player.rocket.notifyIdle();
        //DrawText(player.rocket.numBullets.c_str(), 10, 500, 20, BLACK);



        /*if (FileExists("C:/Users/chido/OneDrive/Desktop/git fps/raylib-game-template-main/projects/VS2022/build/raylib_game/bin/x64/Debug/assets/textures/grass.jpg"))
        {
            std::cout << "exists" << "\n";
        }
        else
        {
            std::cout << "nah" << "\n";
        }*/

        player.gun.reset();
  
        EndDrawing();
        
        world.scanTile(player);
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

// Draw game level --> Game.h Will process data from Map Objects







