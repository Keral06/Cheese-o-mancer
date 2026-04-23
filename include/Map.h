#pragma once

#include "Module.h"
#include <list>
#include <vector>
#include "Player.h"
#include "Checkpoint.h"
#include <memory>

class Enemy;
class Player;

// L09: TODO 5: Add attributes to the property structure
struct Properties
{
    struct Property
    {
        std::string name;
        bool value; //We assume that we are going to work only with bool for the moment
        int valueInt;
        float valueFloat;
        std::string valueString;
       
    };

    std::list<Property*> propertyList;

    ~Properties()
    {
        for (const auto& property : propertyList)
        {
            delete property;
        }

        propertyList.clear();
    }

    // L09: DONE 7: Method to ask for the value of a custom property
    Property* GetProperty(std::string name)
    {
        for (const auto& property : propertyList) {
            if (property->name == name) {
                return property;
            }
        }

        return nullptr;
    }

};

struct MapLayer
{
    // L07: TODO 1: Add the info to the MapLayer Struct
    int id;
    std::string name;
    int width;
    int height;
    std::vector<int> tiles;
    Properties properties;
    // L07: TODO 6: Short function to get the gid value of i,j
    unsigned int Get(int i, int j) const
    {
        return tiles[(j * width) + i];
    }
};



struct ObjectGroup
{
    // L07: TODO 1: Add the info to the MapLayer Struct
    int id;
    std::string name;
    struct Object
    {
        // L07: TODO 1: Add the info to the MapLayer Struct
        int id;
        std::string name;
        int width;
        int height;
        int x;
        int y;
        int gid = 0;
        Properties properties;
    };

    
    std::vector<Object*> objects;

};

// L06: TODO 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!

struct TileSet
{
    int firstGid;
    std::string name;
    int tileWidth;
    int tileHeight;
    int spacing;
    int margin;
    int tileCount;
    int columns;
    SDL_Texture* texture;

    // L07: TODO 7: Implement the method that receives the gid and returns a Rect
    SDL_Rect GetRect(unsigned int gid) {
        SDL_Rect rect = { 0 };

        int relativeIndex = gid - firstGid;
        rect.w = tileWidth;
        rect.h = tileHeight;
        rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
        rect.y = margin + (tileHeight + spacing) * (relativeIndex / columns);

        return rect;
    }

};

// L06: TODO 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int height;
	int tileWidth;
	int tileHeight;
    std::list<TileSet*> tilesets;

    // L07: TODO 2: Add the info to the MapLayer Struct
    std::list<MapLayer*> layers;
    std::list<ObjectGroup*> objectgroups;
};

class Map : public Module
{
public:

    Map();

    // Destructor
    virtual ~Map();

    // Called before render is available
    bool Awake();

    // Called before the first frame
    bool Start();

    // Called each loop iteration
    bool Update(float dt);

    // Called before quitting
    bool CleanUp();

    // Load new map
    bool Load(std::string path, std::string mapFileName);

    // L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
    Vector2D MapToWorld(int i, int j) const;

    // L09: TODO 2: Implement function to the Tileset based on a tile id
    TileSet* GetTilesetFromTileId(int gid) const;

    // L09: TODO 6: Load a group of properties 
    bool LoadProperties(pugi::xml_node& node, Properties& properties);

    // L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
    Vector2D GetStartPoint(std::string layerName, std::string objectName);
   
    Vector2D WorldToMap(int x, int y);

    // L09: TODO 2: Implement function to the Tileset based on a tile id


    // L09: TODO 6: Load a group of properties 
    MapLayer* GetNavigationLayer();
    Vector2D Map::GetMapSizeInTiles();
    Vector2D GetMapSizeInPixels();
    void DrawLayer(const char* layerName);
    void DrawObjectLayerParallax(std::string layerName, float parallaxSpeed);
    int GetTileWidth() {
    return mapData.tileWidth;
}

    int GetTileHeight() {
    return mapData.tileHeight;
}
    //L15 TODO 2: Define a method to load entities from the map XML
    void LoadEntities(std::shared_ptr<Player>& player, std::vector<std::shared_ptr<Enemy>>& enemies);
    std::vector<std::shared_ptr<Checkpoint>> checkpoints;
    //L15 TODO 4: Define a method to save entities to the map XML
    void SaveEntities(std::shared_ptr<Player> player);
    // L19 TODO 1: Calculate Camera position in Tiles
    Vector2D GetCameraPositionInTiles();

    // L19 TODO 2: Calculate Camera limits in Tiles
    Vector2D GetCameraLimitsInTiles(Vector2D camPosTile);
    ObjectGroup::Object* GetObject(std::string layerName, std::string objectName);
    ObjectGroup::Object* GetObjectByProperty(std::string layerName, std::string propName, std::string value);
    std::vector<int> killedEnemies;
    std::vector<PhysBody*> mapBodies;
public: 
    std::string mapFileName;
    std::string mapPath;
private:
    bool mapLoaded;
    // L06: DONE 1: Declare a variable data of the struct MapData
    MapData mapData;
    //L15 TODO 2: make the mapFileXML an attribute of the Map class
    pugi::xml_document mapFileXML;
};
