#pragma once
#include "raylib.h"
#include<vector>
#include "string"
#include <Actor.h>

/*
 Ideally, I will make map files with tiles and structures and then send process them into a Map object that will be drawn by Game
*/

struct Tile
{
    Vector3 position;
    Vector2 size;
    Color color;
    int id;
    Texture2D* texture;

    void setTexture(Texture2D& newTexture);
};

struct Structure //may have to designate between wall or tower later but for now 
{
    Vector3 position;
    Vector3 size;
    Color color;
    BoundingBox box;

    void setBoundingBox();

};


struct Map
{
    std::vector<Tile> tiles;
    std::vector<Structure> buildings; //buildings
    std::vector<Structure> walls; // walls
    std::vector<Structure> allStructures;

    std::vector<Texture2D> textures;

    std::vector<Tile> redTiles;
    std::vector<Tile> blueTiles;
    std::vector<Tile> greenTiles;
    std::vector<Tile> lastVisited;


    void setBuildings(std::vector<Structure>& list);
    void setWalls(std::vector<Structure>& list);
    void setTiles(std::vector<Tile>& list);
    void addTexture(std::string& fileName);
    void sortTiles();
    void scanTile(Actor& actor);
    bool scanList(Tile& tile);

    bool operator==(const Color& color1);

    bool operator==(const Tile& tile1);

};

