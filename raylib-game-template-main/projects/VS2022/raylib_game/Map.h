#pragma once
#include "raylib.h"
#include<vector>


struct Tile
{
    Vector3 position;
    Vector2 size;
    Color color;
};

struct Structure //may have to designate between wall or tower later but for now 
{
    Vector3 position;
    Vector3 size;
    Color color;
    BoundingBox box;

};


struct Map
{
    std::vector<Tile> tiles;
    std::vector<Structure> buildings; //buildings
    std::vector<Structure> walls; // walls

};

