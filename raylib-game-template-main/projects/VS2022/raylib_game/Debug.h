#pragma once
#include "raylib.h"
#include "iostream"
#include "Bullet.h"
class Debug
{
};

std::ostream& operator<<(std::ostream& out, Vector3 vec);
std::ostream& operator<<(std::ostream& out, Color color);


//
//std::cout << player.dir.x << " " << player.dir.y << " " << player.dir.z << "\n";
//std::cout << "Player Pos: " << player.position.x << " " << player.position.y << " " << player.position.z << "\n";
//std::cout << towers[0].box.min.x << " " << towers[0].box.min.y << " " << towers[0].box.min.z << "\n";
//std::cout << "Tower X COLL: " << hitTowerX << " Tower Z COLL: " << hitTowerZ << "\n";
//std::cout << "State of position:  " << player.rocket.ammoList[0].position.x << " " << player.rocket.ammoList[0].position.y << " " << player.rocket.ammoList[0].position.z << " " << (int)player.rocket.ammoList[0].state << "\n";
//std::cout << "Hit Tower X: " << hitTowerX << " " << "Hit Tower Z: " << hitTowerZ;
//std::cout << " " << "Hit Wall X: " << hitWallX << " " << "Hit Wall Z: " << hitWallZ << "\n";
//std::cout << std::boolalpha << "Player Grounded flag: " << player.isGrounded << " Player jump pressed: " << player.jumpPressed << "\n";