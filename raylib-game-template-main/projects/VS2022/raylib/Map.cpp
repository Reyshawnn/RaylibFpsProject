#include <Map.h>

void Tile::setTexture(Texture2D& newTexture)
{
    texture = &newTexture;
}


void Structure::setBoundingBox()
{
    box.min = { position.x - size.x * 0.5f,
       position.y - size.y * 0.5f,
        position.z - size.z * 0.5f
    };

    box.max = { position.x + size.x * 0.5f,
    position.y + size.y * 0.5f,
    position.z + size.z * 0.5f
    };
}

void Map::setBuildings(std::vector<Structure>& list)
{
    buildings = list; //(I could move the vectors @.@)

    for (size_t index{}; index < buildings.size(); index++)
    {
        allStructures.push_back(buildings.at(index));
        allStructures.at(index).setBoundingBox();
    }
}

void Map::setWalls(std::vector<Structure>& list)
{
    walls = list; //(I could move the vectors @.@)

    for (size_t index{}; index < walls.size(); index++)
    {
        allStructures.push_back(walls.at(index));
        allStructures.at(index).setBoundingBox();
    }
}

void Map::setTiles(std::vector<Tile>& list)
{
    tiles = list;
}

void Map::addTexture(std::string& fileName)
{
    Texture2D texture{ LoadTexture(fileName.c_str()) };

    textures.push_back(texture);
}
bool Map::operator==(const Color& color1)
{
    if (((color1.r == Color{ RED }.r) && (color1.g == Color{ RED }.g) && (color1.b == Color{ RED }.b) && (color1.a == Color{ RED }.a)))
    {
        return true;
    }
    else if (((color1.r == Color{ BLUE }.r) && (color1.g == Color{ BLUE }.g) && (color1.b == Color{ BLUE }.b) && (color1.a == Color{ BLUE }.a)))
    {
        return true;
    }
    else if (((color1.r == Color{ GREEN }.r) && (color1.g == Color{ GREEN }.g) && (color1.b == Color{ GREEN }.b) && (color1.a == Color{ GREEN }.a)))
    {
        return true;
    }
    else
    {
        return false;
    }
}



void Map::sortTiles()
{
    for (auto& tile : tiles)
    {
        if (((tile.color.r == Color{ RED }.r) && (tile.color.g == Color{ RED }.g) && (tile.color.b == Color{ RED }.b) && (tile.color.a == Color{ RED }.a)))
        {
            redTiles.push_back(tile);
        }

        else if (((tile.color.r == Color{ BLUE }.r) && (tile.color.g == Color{ BLUE }.g) && (tile.color.b == Color{ BLUE }.b) && (tile.color.a == Color{ BLUE }.a)))
        {
            blueTiles.push_back(tile);
        }

        else if (((tile.color.r == Color{ GREEN }.r) && (tile.color.g == Color{ GREEN }.g) && (tile.color.b == Color{ GREEN }.b) && (tile.color.a == Color{ GREEN }.a)))
        {
            greenTiles.push_back(tile);
        }
    }
}

bool Map::scanList(Tile& tile1)
{
    for (auto& tile : lastVisited)
    {
        if ((tile1.id == tile.id))
        {
            
            return false;
        }
    }

    return true;
}

void Map::scanTile(Actor& actor)
{
    
        for (auto& tile : redTiles) //need to round off tile positions
        {
            if (lastVisited.empty() && ((int)tile.position.x == (int)actor.position.x) && ((int)tile.position.z == (int)actor.position.z))
            {
                lastVisited.push_back(tile);
                //std::cout << "Tile saved: from red vector " << "\n";
            }
               
            else if (((int)tile.position.x == (int)actor.position.x) && ((int)tile.position.z == (int)actor.position.z) && (scanList(tile)))
             {
                 lastVisited.push_back(tile);
                 //std::cout << "Tile saved:  " << (int)actor.position.x << " " << (int)actor.position.z << "Tile saved: from red vector " << "\n";
             }
        }

        for (auto& tile : blueTiles)
        {
            if (lastVisited.empty() && ((int)tile.position.x == (int)actor.position.x) && ((int)tile.position.z == (int)actor.position.z))
            {
                lastVisited.push_back(tile);
                //std::cout << "Tile saved: from blue vector " << "\n";
            }
            else if (((int)tile.position.x == (int)actor.position.x) && ((int)tile.position.z == (int)actor.position.z) && (scanList(tile)))
            {
                lastVisited.push_back(tile);
                //std::cout << "Tile saved:  " << (int)actor.position.x << " " << (int)actor.position.z << "Tile saved: from blue vector " << "\n";
            }
        }

        for (auto& tile : greenTiles)
        {
            if (lastVisited.empty() && ((int)tile.position.x == (int)actor.position.x) && ((int)tile.position.z == (int)actor.position.z))
            {
                lastVisited.push_back(tile);
                //std::cout << "Tile saved: from green vector " << "\n";
            }
            else if (((int)tile.position.x == (int)actor.position.x) && ((int)tile.position.z == (int)actor.position.z) && (scanList(tile)))
            {
                lastVisited.push_back(tile);
                //std::cout << "Tile saved:  " << (int)actor.position.x << " " << (int)actor.position.z  << "Tile saved: from green vector " << "\n";
            }

        }
    }

