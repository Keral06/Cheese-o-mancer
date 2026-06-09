#include "Engine.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"
#include "Enemy.h"          
#include "EnemigoVolador.h"
#include <math.h>
#include "Audio.h"
#include "Input.h"
#include "Checkpoint.h"
#include "coins.h"
#include "FinalBoss.h"
#include "Protection.h"
#include "ExtraLive.h"
#include "Scene.h"
#include "Verdugo.h"
#include "Rat.h"
#include "Jailer.h"
#include "HANDMAN.h"
#include "Magician.h"
#include "Door.h"
#include "cheeseballInteractuable.h"
#include "WeakWall.h"
#include "Pickable_objects.h"
#include "Horse.h"
#include "Bee.h"
#include "KnightBoss.h"
#include "PrincessBoss.h"
#include "HighPriestess.h"
#include "Rey.h"
#include <algorithm>


bool Map::IsMoho(int gid) {
    static const std::vector<int> ids = { 881 }; // todos los GIDs de moho
    return std::find(ids.begin(), ids.end(), gid) != ids.end();
}

bool Map::IsPolvo(int gid) {
    static const std::vector<int> ids = { 882, 18967, 2135, 1636, 819, 2540,20591, 2945, 3370, 2895, 1576, 1575, 3696, 1688, 1682, 1199, 1191, 1189, 1197, 2866 }; // todos los de polvo
    return std::find(ids.begin(), ids.end(), gid) != ids.end();
}                                                                                                           //1682 en temple lvl3 tmb    2866 queen int
                                                                                                       //20591 20592 botanica
bool Map::IsHierba(int gid) {                                                                       //2945 2946 encreuada
    static const std::vector<int> ids = { 883, 20592, 2946, 3369 }; // todos los de hierba        //justes. 3369, 3370 uno polvo otro hierba
    return std::find(ids.begin(), ids.end(), gid) != ids.end();
}


Map::Map() : Module(), mapLoaded(false)
{
    name = "map";
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake()
{
    name = "map";
    LOG("Loading Map Parser");

    return true;
}

bool Map::Start() {
    killedEnemies.clear();
    return true;

	/*int checkpointfx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/checkpoint.wav");*/
}

bool Map::Update(float dt)
{
    bool ret = true;
    if (mapLoaded) {
        // actualizar temporizador animaciones
        for (auto& tileset : mapData.tilesets) {
            for (auto& animPair : tileset->animations) {
                TileAnimation& anim = animPair.second;

                // Sumamos el tiempo. NOTA: Si tu 'dt' está en segundos en vez de milisegundos, 
                // usa 'anim.timer += (dt * 1000.0f);'
                anim.timer += dt;

                // Si superamos la duración del frame actual, pasamos al siguiente
                if (anim.timer >= anim.frames[anim.currentFrame].duration) {
                    anim.timer = 0.0f; // Reiniciamos el reloj
                    anim.currentFrame = (anim.currentFrame + 1) % anim.frames.size(); // Bucle infinito
                }
            }
        }


        // L07 TODO 5: Prepare the loop to draw all tiles in a layer + DrawTexture()
        // iterate all tiles in a layer

        // 1 OBJETOS DE FONDO (Background == true)
        for (const auto& group : mapData.objectgroups) {

            auto drawProp = group->properties.GetProperty("Draw");
            auto bgProp = group->properties.GetProperty("Background");

            // Solo dibuja si tiene la propiedad Background y está activada
            if (drawProp && drawProp->value == true && bgProp != NULL && bgProp->value == true) {
                float parallax = 1.0f;
                auto parallaxProp = group->properties.GetProperty("Parallax");
                if (parallaxProp) parallax = parallaxProp->valueFloat;

                DrawObjectLayerParallax(group->name, parallax);
            }
        }

        // 2 LOS TILES (El suelo y las paredes)
        for (const auto& mapLayer : mapData.layers) {
            if (mapLayer->properties.GetProperty("Draw") != NULL && mapLayer->properties.GetProperty("Draw")->value == true) {
                DrawLayer(mapLayer->name);
            }
        }

        // 3 OBJETOS FRONTALES (Background == false o no existe)
        //for (const auto& group : mapData.objectgroups) {
        //    auto drawProp = group->properties.GetProperty("Draw");
        //    auto bgProp = group->properties.GetProperty("Background");

        //    // Solo dibuja si NO tiene la propiedad Background, o si la tiene pero es falsa
        //    if (drawProp && drawProp->value == true && (bgProp == NULL || bgProp->value == false)) {
        //        float parallax = 1.0f;
        //        auto parallaxProp = group->properties.GetProperty("Parallax");
        //        if (parallaxProp) parallax = parallaxProp->valueFloat;

        //        DrawObjectLayerParallax(group->name, parallax);
        //    }
        //}
    }

    // DRAW Y LIMPIEZA DE PARTÍCULAS
    for (auto it = mapParticles.begin(); it != mapParticles.end(); ) {
        (*it)->draw();

        // Si el emisor ya acabó su duración y no le quedan partículas vivas, lo borramos
        if (!(*it)->isActive() && (*it)->getParticleCount() == 0) {
            delete* it;
            it = mapParticles.erase(it);
        }
        else {
            ++it;
        }
    }
    return ret;
}

// L09: TODO 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    TileSet* bestFit = nullptr;
    int maxGid = -1;

    for (const auto& tileset : mapData.tilesets) {
        // Buscamos el tileset con el firstGid más alto que siga siendo menor o igual al GID del objeto
        if (gid >= tileset->firstGid && tileset->firstGid > maxGid) {
            bestFit = tileset;
            maxGid = tileset->firstGid;
        }
    }

    return bestFit;
}


// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // L06: TODO 2: Make sure you clean up any memory allocated from tilesets/map
    for (const auto& tileset : mapData.tilesets) {
        delete tileset;
    }
    mapData.tilesets.clear();

    // L07 TODO 2: clean up all layer data
    for (const auto& layer : mapData.layers)
    {
        delete layer;
    }
    mapData.layers.clear();

	// Clean up object groups
    for (const auto& group : mapData.objectgroups) {
        if (group != nullptr) {
            for (const auto& object : group->objects) {
                delete object;
            }
            group->objects.clear();
            delete group;
        }
    }
    mapData.objectgroups.clear();
    checkpoints.clear();
    for (const auto& body : mapBodies) {
        Engine::GetInstance().physics->DeletePhysBody(body);
    }
    mapBodies.clear();
    killedEnemies.clear();

    for (auto p : mapParticles) {
        delete p;
    }
    mapParticles.clear();

    return true;
}

// Load new map
bool Map::Load(std::string path, std::string fileName)//
{
    bool ret = false;

    // Assigns the name of the map file and the path
    mapFileName = fileName;
    mapPath = path;
    std::string mapPathName = mapPath + mapFileName;

   
    pugi::xml_parse_result result = mapFileXML.load_file(mapPathName.c_str());

    if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", mapPathName.c_str(), result.description());
		ret = false;
    }
    else {

        // L06: TODO 3: Implement LoadMap to load the map properties
        // retrieve the paremeters of the <map> node and store the into the mapData struct
        mapData.width = mapFileXML.child("map").attribute("width").as_int();
        mapData.height = mapFileXML.child("map").attribute("height").as_int();
        mapData.tileWidth = mapFileXML.child("map").attribute("tilewidth").as_int();
        mapData.tileHeight = mapFileXML.child("map").attribute("tileheight").as_int();

        // L06: TODO 4: Implement the LoadTileSet function to load the tileset properties
       
        //Iterate the Tileset
        for(pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode!=NULL; tilesetNode = tilesetNode.next_sibling("tileset"))
		{

            // 1. Miramos si es un TSX externo
            std::string sourcePath = tilesetNode.attribute("source").as_string();
            pugi::xml_node dataNode = tilesetNode; // Por defecto leemos los datos de aquí
            pugi::xml_document tsxDoc;

            if (sourcePath != "") {
                // Es un TSX. Lo abrimos para leer los datos reales.
                std::string fullTsxPath = mapPath + sourcePath;
                pugi::xml_parse_result result = tsxDoc.load_file(fullTsxPath.c_str());
                if (result) {
                    dataNode = tsxDoc.child("tileset"); // Cambiamos el nodo objetivo al del archivo externo
                    LOG("TSX cargado correctamente: %s", fullTsxPath.c_str());
                }
            }

            //Load Tileset attributes
            TileSet* tileSet = new TileSet();
            tileSet->firstGid = tilesetNode.attribute("firstgid").as_int();

            tileSet->name = dataNode.attribute("name").as_string();
            tileSet->tileWidth = dataNode.attribute("tilewidth").as_int();
            tileSet->tileHeight = dataNode.attribute("tileheight").as_int();
            tileSet->spacing = dataNode.attribute("spacing").as_int();
            tileSet->margin = dataNode.attribute("margin").as_int();
            tileSet->tileCount = dataNode.attribute("tilecount").as_int();
            tileSet->columns = dataNode.attribute("columns").as_int(1);

			//Load the tileset image
            std::string imgName = dataNode.child("image").attribute("source").as_string();

            if (imgName == "") {
                imgName = dataNode.child("tile").child("image").attribute("source").as_string();
            }

            // 2. SOLO cargamos si hay una imagen.
            if (imgName != "") {
                std::string fullPath = mapPath + imgName;
                tileSet->texture = Engine::GetInstance().textures->Load(fullPath.c_str());
            }
            else {
                tileSet->texture = nullptr;
            }
            LOG("Cargando imagen de tileset '%s'. Ruta completa: %s", tileSet->name.c_str(), (mapPath + imgName).c_str());

            // Sprites animados - Iteramos usando dataNode
            for (pugi::xml_node tileNode = dataNode.child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {

                // Comprobamos si este tile en concreto tiene una animación dentro
                pugi::xml_node animNode = tileNode.child("animation");

                if (animNode != NULL) {
                    TileAnimation anim; // Tu struct de animación
                    int localId = tileNode.attribute("id").as_int(); // El ID de este tile animado

                    // Iteramos por todos los fotogramas (frames) de esta animación
                    for (pugi::xml_node frameNode = animNode.child("frame"); frameNode != NULL; frameNode = frameNode.next_sibling("frame")) {
                        TileFrame frame;
                        frame.tileId = frameNode.attribute("tileid").as_int();
                        frame.duration = frameNode.attribute("duration").as_int();

                        anim.frames.push_back(frame);
                    }

                    // Guardamos la animación completa en el diccionario del tileset
                    tileSet->animations[localId] = anim;
                }
            }


            mapData.tilesets.push_back(tileSet); // Esto tiene que estar SIEMPRE al final del bucle
		}

        // L07: TODO 3: Iterate all layers in the TMX and load each of them (incluyendo grupos anidados)
        ParseLayersRecursive(mapFileXML.child("map"));

        ParseObjectGroupsRecursive(mapFileXML.child("map"));

        checkpoints.clear();
            

        // L08 TODO 3: Create colliders
        // L08 TODO 7: Assign collider type
        // Later you can create a function here to load and create the colliders from the map

        //Iterate the layer and create colliders
        for (const auto& objectGroup : mapData.objectgroups) {
            ColliderType type = ColliderType::DANGER;
            bool isSensor = false;

            if (objectGroup->name == "Floor") {
                type = ColliderType::PLATFORM;
            }
            else if (objectGroup->name == "Walls") {
                type = ColliderType::PARED;
            }
            else if (objectGroup->name == "spikes") {
                type = ColliderType::DANGER;
            }
            else if (objectGroup->name == "WeakWalls")
            {
                for (const auto& object : objectGroup->objects)
                {
                    std::shared_ptr<WeakWall> wall = std::dynamic_pointer_cast<WeakWall>(
                        Engine::GetInstance().entityManager->CreateEntity(EntityType::WEAKWALL)
                    );

                    wall->position = Vector2D((float)object->x, (float)object->y);
                    wall->width = object->width;
                    wall->height = object->height;

                    // --- SOLUCIÓN USANDO TU PROPIO SISTEMA DE PROPIEDADES ---
                    bool shouldDraw = true; // Por defecto se dibuja si no existe la propiedad

                    // Buscamos la propiedad en el listado del objeto de Tiled
                    Properties::Property* pDraw = object->properties.GetProperty("Draw");
                    if (pDraw != nullptr)
                    {
                        shouldDraw = pDraw->value; // Extrae el booleano guardado
                    }

                    wall->drawed = shouldDraw;
                    // -------------------------------------------------------
                    int targetLevel = 1; // Valor por defecto (por si no tiene la propiedad en Tiled)

                    // Buscamos la propiedad entera en el listado de Tiled
                    Properties::Property* pLevel = object->properties.GetProperty("level");
                    if (pLevel != nullptr)
                    {
                        targetLevel = pLevel->valueInt; // <--- ¡Aquí usamos tu variable valueInt!
                    }

                    wall->level = targetLevel;
                    wall->Start();
                }

                continue;
            }
            else if (objectGroup->name == "Checkpoints") {
                for (const auto& object : objectGroup->objects) {
                    if (object->name == "Player") {
                        continue;
                    }
                    std::shared_ptr<Checkpoint> checkpoint = std::dynamic_pointer_cast<Checkpoint>(
                        Engine::GetInstance().entityManager->CreateEntity(EntityType::CHECKPOINT)
                    );
                    checkpoint->position = Vector2D((float)object->x, (float)object->y);
                    checkpoint->xInicial = (int)object->x;
                    checkpoint->yInicial = (int)object->y;
                    checkpoint->name = object->name;
                    checkpoint->Start();
                    checkpoints.push_back(checkpoint);
                }
                continue;
            }
            else if (objectGroup->name == "Coins") {
                for (const auto& object : objectGroup->objects) {
                    if (object->name == "Coin" || object->name == "coin") {
                        std::shared_ptr<Coins> coin = std::dynamic_pointer_cast<Coins>(
                            Engine::GetInstance().entityManager->CreateEntity(EntityType::COIN)
                        );
                        coin->position = Vector2D((float)object->x, (float)object->y);
                        coin->xInicial = (int)object->x;
                        coin->yInicial = (int)object->y;
                        coin->Start();
                    }
                }
                continue;
              
            }
            else if (objectGroup->name == "Items") {
                for (const auto& object : objectGroup->objects) {
                    if (object->name == "Protection") {
                        std::shared_ptr<Protection> protection = std::dynamic_pointer_cast<Protection>(
                            Engine::GetInstance().entityManager->CreateEntity(EntityType::PROTECTION)
                        );
                        protection->position = Vector2D((float)object->x, (float)object->y);
                        protection->xInicial = (int)object->x;
                        protection->yInicial = (int)object->y;
                        protection->Start();
                    }
                    if (object->name == "Live") {
                        std::shared_ptr<ExtraLive> protection = std::dynamic_pointer_cast<ExtraLive>(
                            Engine::GetInstance().entityManager->CreateEntity(EntityType::EXTRALIVE)
                        );
                        protection->position = Vector2D((float)object->x, (float)object->y);
                        protection->xInicial = (int)object->x;
                        protection->yInicial = (int)object->y;
                        protection->Start();
                    }
                }
                continue;
            }
            else if (objectGroup->name == "Entities") {
                for (const auto& object : objectGroup->objects) {
                    if (object->name == "miniBoss") {
                        int w = object->width > 0 ? object->width : 50;
                        int h = object->height > 0 ? object->height : 50;
                        PhysBody* triggerBody = Engine::GetInstance().physics->CreateRectangleSensor(
                            object->x + w / 2, object->y + h / 2, w, h, STATIC
                        );
                        triggerBody->ctype = ColliderType::ARENA_TRIGGER;
                        mapBodies.push_back(triggerBody);
                    }
                }
                continue;
            }

            else {
                continue;
            }
            for (const auto& object : objectGroup->objects) {

                int x = object->x;
                int y = object->y;
                int w = object->width;
                int h = object->height;

                if (type == ColliderType::SAVE && (w == 0 || h == 0)) {
                    w = mapData.tileWidth;
                    h = mapData.tileHeight;
                }

                int centerX = x + w / 2;
                int centerY = y + h / 2;

                PhysBody* body = nullptr;

                if (isSensor) {
                    body = Engine::GetInstance().physics.get()->CreateRectangleSensor(centerX, centerY, w, h, STATIC);
                }
                else {
                    body = Engine::GetInstance().physics.get()->CreateRectangle(centerX, centerY, w, h, STATIC);
                }

                if (body != nullptr) {
                    body->ctype = type;
                    body->objectName = object->name;
                    mapBodies.push_back(body);
                }
            }
        }

        ret = true;

        // L06: TODO 5: LOG all the data loaded iterate all tilesetsand LOG everything
        if (ret == true)
        {
            LOG("Successfully parsed map XML file :%s", fileName.c_str());
            LOG("width : %d height : %d", mapData.width, mapData.height);
            LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);
            LOG("Tilesets----");

            //iterate the tilesets
            for (const auto& tileset : mapData.tilesets) {
                LOG("name : %s firstgid : %d", tileset->name.c_str(), tileset->firstGid);
                LOG("tile width : %d tile height : %d", tileset->tileWidth, tileset->tileHeight);
                LOG("spacing : %d margin : %d", tileset->spacing, tileset->margin);
            }
            			
            LOG("Layers----");

            for (const auto& layer : mapData.layers) {
                LOG("id : %d name : %s", layer->id, layer->name.c_str());
				LOG("Layer width : %d Layer height : %d", layer->width, layer->height);
            }   
        }
        else {
            LOG("Error while parsing map file: %s", mapPathName.c_str());
        }

      

    }

    mapLoaded = ret;
    return ret;
}

// L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
Vector2D Map::MapToWorld(int i, int j) const
{
    Vector2D ret;
    ret.setX((float)(i * mapData.tileWidth));
    ret.setY((float)(j * mapData.tileHeight));

    return ret;
}

// L09: TODO 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    for (pugi::xml_node propertieNode = node.child("properties").child("property");
        propertieNode;
        propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();

        p->name = propertieNode.attribute("name").as_string();

        const char* value = propertieNode.attribute("value").as_string();

        p->valueString = value;
        p->valueInt = atoi(value);
        p->valueFloat = (float)atof(value);

        // bool robusto
        std::string v = value;
        p->value = (v == "true" || v == "1");

        properties.propertyList.push_back(p);
    }

    return true;
}

Vector2D Map::GetMapSizeInPixels()
{
    Vector2D sizeInPixels;
    sizeInPixels.setX((float)(mapData.width * mapData.tileWidth));
    sizeInPixels.setY((float)(mapData.height * mapData.tileHeight));
    return sizeInPixels;
}


// L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions



// L09: TODO 6: Load a group of properties from a node and fill a list with it
Vector2D Map::WorldToMap(int x, int y) {

    Vector2D ret(0, 0);
    ret.setX((float)(x / mapData.tileWidth));
    ret.setY((float)(y / mapData.tileHeight));

    return ret;
}

// L10: TODO 7: Create a method to get the map size in pixels


Vector2D Map::GetMapSizeInTiles()
{
    return Vector2D((float)mapData.width, (float)mapData.height);
}

// Method to get the navigation layer from the map
MapLayer* Map::GetNavigationLayer() {
    for (const auto& layer : mapData.layers) {
        if (layer->properties.GetProperty("Navigation") != NULL &&
            layer->properties.GetProperty("Navigation")->value) {
            return layer;
        }
    }

    return nullptr;
}


void Map::LoadEntities(std::shared_ptr<Player>& player, std::vector<std::shared_ptr<Enemy>>& enemies) {
    std::list<std::shared_ptr<Entity>> toDestroy;
    for (auto& entity : Engine::GetInstance().entityManager->entities) {
        if (entity->type == EntityType::ENEMY || entity->type == EntityType::ENEMYFLYING) {
            toDestroy.push_back(entity);
        }
    }
    for (auto& entity : toDestroy) {
        Engine::GetInstance().entityManager->DestroyEntity(entity);
    }

    enemies.clear();

    for (auto p : mapParticles) {
        delete p;
    }
    mapParticles.clear();

    for (pugi::xml_node objectGroupNode = mapFileXML.child("map").child("objectgroup"); objectGroupNode != NULL; objectGroupNode = objectGroupNode.next_sibling("objectgroup")) {
        if (objectGroupNode.attribute("name").as_string() == std::string("Doors")) {
            for (pugi::xml_node objectNode = objectGroupNode.child("object"); objectNode != NULL; objectNode = objectNode.next_sibling("object")) {
                std::string entityType = objectNode.attribute("type").as_string();
                if (entityType == "") {
                    entityType = objectNode.attribute("class").as_string(); // Añadimos el parche que ya usabas en las partículas
                }
                float x = objectNode.attribute("x").as_float();
                float y = objectNode.attribute("y").as_float();

                if (entityType == "Door") {
                    auto door = std::dynamic_pointer_cast<Door>(Engine::GetInstance().entityManager->CreateEntity(EntityType::DOOR));
                    door->position = Vector2D(x, y);
                    Properties tempProperties;
                    LoadProperties(objectNode, tempProperties);

                    auto mapProp = tempProperties.GetProperty("targetMap");
                    auto spawnProp = tempProperties.GetProperty("targetSpawn");
                    auto offsetX = tempProperties.GetProperty("offsetX");
                    auto offsetY = tempProperties.GetProperty("offsetY");
                    int width = objectNode.attribute("width").as_int();
                    int height = objectNode.attribute("height").as_int();
                    auto interactionProp = tempProperties.GetProperty("requiresInteraction");

                    door->SetDoorData(
                        mapProp ? mapProp->valueString : "",
                        spawnProp ? spawnProp->valueString : "",
                        offsetX ? offsetX->valueInt : 0,
                        offsetY ? offsetY->valueInt : 0,
                        width,
                        height,
                        interactionProp ? interactionProp->value : false
                    );
                    door->Start();
                }
            }
        }
    }

    for (pugi::xml_node objectGroupNode = mapFileXML.child("map").child("objectgroup"); objectGroupNode != NULL; objectGroupNode = objectGroupNode.next_sibling("objectgroup")) {
        if (objectGroupNode.attribute("name").as_string() == std::string("Entities") || objectGroupNode.attribute("name").as_string() == std::string("FinalBoss") || objectGroupNode.attribute("name").as_string() == std::string("Doors")) {
            for (pugi::xml_node objectNode = objectGroupNode.child("object"); objectNode != NULL; objectNode = objectNode.next_sibling("object")) {
                int id = objectNode.attribute("id").as_int();

                bool isDead = false;
                for (int killedId : killedEnemies) {
                    if (killedId == id) {
                        isDead = true;
                        break;
                    }
                }

                if (isDead) continue;

                std::string entityType = objectNode.attribute("type").as_string();
                float x = objectNode.attribute("x").as_float();
                float y = objectNode.attribute("y").as_float();

                if (entityType == "Player") {
                    if (Engine::GetInstance().scene->continueGame == true) {
                        if (objectNode.attribute("cpX")) x = objectNode.attribute("cpX").as_float();
                        if (objectNode.attribute("cpY")) y = objectNode.attribute("cpY").as_float();
                    }
                    else {
                        killedEnemies.clear();
                    }

                    if (player == nullptr) {
                        player = std::dynamic_pointer_cast<Player>(Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER));
                        player->position = Vector2D(x, y);
                        player->Start();
                    }

                    if (/*Engine::GetInstance().scene->GetLastScene() != SceneID::INTRO_SCREEN &&
                        Engine::GetInstance().scene->GetLastScene() != SceneID::MAIN_MENU &&*/
                        Engine::GetInstance().scene->firstDoor == false) {

                        auto obj = Engine::GetInstance().map->GetObjectByProperty("Doors", "name", Engine::GetInstance().scene->nextSpawnPoint);
                        if (obj != nullptr) {
                            int offsetX = obj->properties.GetProperty("offsetX") ? obj->properties.GetProperty("offsetX")->valueInt : 0;
                            int offsetY = obj->properties.GetProperty("offsetY") ? obj->properties.GetProperty("offsetY")->valueInt : 0;
                            Vector2D spawn(obj->x + offsetX, obj->y + offsetY);
                            player->SetPosition(spawn);
                        }
                    }

                    /*if (objectNode.attribute("score")) {
                        Engine::GetInstance().scene->score = objectNode.attribute("score").as_int();
                        LOG("Score cargado desde XML: %d", Engine::GetInstance().scene->score);
                    }
                    if (objectNode.attribute("timer")) {
                        Engine::GetInstance().scene->levelTimer = objectNode.attribute("timer").as_float();
                        LOG("Timer cargado desde XML: %f", Engine::GetInstance().scene->levelTimer);
                    }*/
                }
                else if (entityType == "Enemy") {
                    std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY));
                    enemy->position = Vector2D(x, y);
                    enemy->Start();
                    enemy->mapID = id;
                }
                else if (entityType == "Verdugo") {
                    std::shared_ptr<Verdugo> verdugo = std::dynamic_pointer_cast<Verdugo>(Engine::GetInstance().entityManager->CreateEntity(EntityType::VERDUGO));
                    verdugo->position = Vector2D(x, y);
                    verdugo->Start();
                    verdugo->mapID = id;
                }
                else if (entityType == "Rat") {
                    std::shared_ptr<Rat> rat = std::dynamic_pointer_cast<Rat>(Engine::GetInstance().entityManager->CreateEntity(EntityType::RAT));
                    rat->position = Vector2D(x, y);

                    Properties tempProps;
                    LoadProperties(objectNode, tempProps);
                    auto propEntered = tempProps.GetProperty("hasEntered");

                    if (propEntered && propEntered->value == false) {
                        rat->hasEntered = false;
                        rat->isArenaRat = true;
                    }

                    rat->Start();
                    rat->mapID = id;
                }
                else if (entityType == "Horse") {
                    std::shared_ptr<Horse> horse = std::dynamic_pointer_cast<Horse>(Engine::GetInstance().entityManager->CreateEntity(EntityType::HORSE));
                    horse->position = Vector2D(x, y);
                    horse->Start();
                    horse->mapID = id;
                }
                else if (entityType == "KnightBoss") {
                    std::shared_ptr<KnightBoss> knightBoss = std::dynamic_pointer_cast<KnightBoss>(Engine::GetInstance().entityManager->CreateEntity(EntityType::KNIGHT));
                    knightBoss->position = Vector2D(x, y);
                    knightBoss->Start();
                    knightBoss->mapID = id;
                }
                else if (entityType == "Princess") {
                    std::shared_ptr<PrincessBoss> princess = std::dynamic_pointer_cast<PrincessBoss>(Engine::GetInstance().entityManager->CreateEntity(EntityType::PRINCESS));
                    princess->position = Vector2D(x, y);
                    princess->Start();
                    princess->mapID = id;
                }
                else if (entityType == "Rey") {
                    std::shared_ptr<Rey> rey = std::dynamic_pointer_cast<Rey>(Engine::GetInstance().entityManager->CreateEntity(EntityType::REY));
                    rey->position = Vector2D(x, y);
                    rey->Start();
                    rey->mapID = id;
                }
                else if (entityType == "HighPriestesss") {
                    std::shared_ptr<HighPriestesss> highPriestess = std::dynamic_pointer_cast<HighPriestesss>(Engine::GetInstance().entityManager->CreateEntity(EntityType::HIGHPRIESTESSS));
                    highPriestess->position = Vector2D(x, y);
                    highPriestess->Start();
                    highPriestess->mapID = id;
                }
                else if (entityType == "Jailer") {
                    std::shared_ptr<Jailer> jailer = std::dynamic_pointer_cast<Jailer>(Engine::GetInstance().entityManager->CreateEntity(EntityType::JAILER));
                    jailer->position = Vector2D(x, y);
                    jailer->Start();
                    jailer->mapID = id;
                }
                else if (entityType == "Handman") {
                    std::shared_ptr<HANDMAN> Handman = std::dynamic_pointer_cast<HANDMAN>(Engine::GetInstance().entityManager->CreateEntity(EntityType::HANDMAN));
                    Properties handman;
                    LoadProperties(objectNode, handman);
                    auto storeIdProp = handman.GetProperty("storeID");
                    Handman->storeID = storeIdProp ? storeIdProp->valueInt : 1;
                    Handman->position = Vector2D(x, y);
                    Handman->xInicial = (int)x;
                    Handman->yInicial = (int)y;
                    Handman->Start();
                    Handman->mapID = id;
                }
                else if (entityType == "NPC") {
                    std::shared_ptr<NPC> Npc = std::dynamic_pointer_cast<NPC>(Engine::GetInstance().entityManager->CreateEntity(EntityType::NPC));
                    Npc->position = Vector2D(x, y);
                    Npc->Start();
                    Npc->mapID = id;
                }
                else if (entityType == "Bee") {
                    std::shared_ptr<Bee> bee = std::dynamic_pointer_cast<Bee>(Engine::GetInstance().entityManager->CreateEntity(EntityType::BEE));
                    bee->position = Vector2D(x, y);
                    bee->Start();
                    bee->mapID = id;
                }
                else if (entityType == "FinalBoss") {
                    std::shared_ptr<FinalBoss> boss = std::dynamic_pointer_cast<FinalBoss>(Engine::GetInstance().entityManager->CreateEntity(EntityType::FINALBOSS));
                    boss->position = Vector2D(x, y);
                    boss->xInicial = (int)x;
                    boss->yInicial = (int)y;
                    boss->Start();
                    boss->mapID = id;
                }
                else if (entityType == "Magician") {
                    std::shared_ptr<Magician> magician = std::dynamic_pointer_cast<Magician>(Engine::GetInstance().entityManager->CreateEntity(EntityType::MAGICIAN));
                    magician->position = Vector2D(x, y);
                    magician->xInicial = (int)x;
                    magician->yInicial = (int)y;
                    magician->Start();
                    magician->mapID = id;
                }
                else if (entityType == "HiddenScrapOfPaper") {
                    std::shared_ptr<HiddenScrapOfPaper> hiddenScrapOfPaper = std::dynamic_pointer_cast<HiddenScrapOfPaper>(Engine::GetInstance().entityManager->CreateEntity(EntityType::HIDDENSCRAPOFPAPER));
                    hiddenScrapOfPaper->position = Vector2D(x, y);
                    hiddenScrapOfPaper->xInicial = (int)x;
                    hiddenScrapOfPaper->yInicial = (int)y;
                    hiddenScrapOfPaper->Start();
                    hiddenScrapOfPaper->mapID = id;
                }
                else if (entityType == "DiscardedScroll") {
                    std::shared_ptr<DiscardedScroll> discardedScroll = std::dynamic_pointer_cast<DiscardedScroll>(Engine::GetInstance().entityManager->CreateEntity(EntityType::DISCARDEDSCROLL));
                    discardedScroll->position = Vector2D(x, y);
                    discardedScroll->xInicial = (int)x;
                    discardedScroll->yInicial = (int)y;
                    discardedScroll->Start();
                    discardedScroll->mapID = id;
                }
                else if (entityType == "Sketches") {
                    std::shared_ptr<Sketches> sketches = std::dynamic_pointer_cast<Sketches>(Engine::GetInstance().entityManager->CreateEntity(EntityType::SKETCHES));
                    sketches->position = Vector2D(x, y);
                    sketches->xInicial = (int)x;
                    sketches->yInicial = (int)y;
                    sketches->Start();
                    sketches->mapID = id;
                }
                else if (entityType == "LockedDoor") {
                    std::shared_ptr<LockedDoor> lockedDoor = std::dynamic_pointer_cast<LockedDoor>(Engine::GetInstance().entityManager->CreateEntity(EntityType::LOCKEDDOOR));
                    lockedDoor->position = Vector2D(x, y);
                    lockedDoor->xInicial = (int)x;
                    lockedDoor->yInicial = (int)y;
                    lockedDoor->Start();
                    lockedDoor->mapID = id;
                }
                else if (entityType == "DestructDoor") {
                    std::shared_ptr<DestructDoor> destructDoor = std::dynamic_pointer_cast<DestructDoor>(Engine::GetInstance().entityManager->CreateEntity(EntityType::DESTRUCTDOOR));
                    destructDoor->position = Vector2D(x, y);
                    destructDoor->xInicial = (int)x;
                    destructDoor->yInicial = (int)y;
                    destructDoor->Start();
                    destructDoor->mapID = id;
                }
                else if (entityType == "Notice from the Royal Halls") {
                    std::shared_ptr<NoteRoyalHalls> destructDoor = std::dynamic_pointer_cast<NoteRoyalHalls>(Engine::GetInstance().entityManager->CreateEntity(EntityType::NOTEROYALHALLS));
                    destructDoor->position = Vector2D(x, y);
                    destructDoor->xInicial = (int)x;
                    destructDoor->yInicial = (int)y;
                    destructDoor->Start();
                    destructDoor->mapID = id;
                }
                else if (entityType == "NormalFlag") {
                    std::shared_ptr<NormalFlag> normalFlag = std::dynamic_pointer_cast<NormalFlag>(Engine::GetInstance().entityManager->CreateEntity(EntityType::NORMALFLAG));
                    normalFlag->position = Vector2D(x, y);
                    normalFlag->xInicial = (int)x;
                    normalFlag->yInicial = (int)y;
                    normalFlag->Start();
                    normalFlag->mapID = id;
                }
                else if (entityType == "CheeseFlag") {
                    std::shared_ptr<CheeseFlag> cheeseFlag = std::dynamic_pointer_cast<CheeseFlag>(Engine::GetInstance().entityManager->CreateEntity(EntityType::CHEESEFLAG));
                    cheeseFlag->position = Vector2D(x, y);
                    cheeseFlag->xInicial = (int)x;
                    cheeseFlag->yInicial = (int)y;
                    cheeseFlag->Start();
                    cheeseFlag->mapID = id;
                }
                else if (entityType == "RatKing") {
                    Properties ratProps;
                    LoadProperties(objectNode, ratProps);
                    auto levelProp = ratProps.GetProperty("Level");
                    int ratLevel = levelProp ? levelProp->valueInt : 1;
                    LOG("RATRKING FOUND! ratLevel=%d, whereIsRat=%d", ratLevel, Engine::GetInstance().scene->whereIsRat);
                    if (ratLevel == Engine::GetInstance().scene->whereIsRat) {
                        LOG("CREATING RATKING!");
                        std::shared_ptr<RatKing> ratking = std::dynamic_pointer_cast<RatKing>(Engine::GetInstance().entityManager->CreateEntity(EntityType::TELEPORT));
                        ratking->position = Vector2D(x, y);
                        ratking->xInicial = (int)x;
                        ratking->yInicial = (int)y;
                        ratking->Start();
                        ratking->mapID = id;
                        ratking->level = ratLevel;
                    }
                }
                else if (entityType == "interactball") {
                    std::shared_ptr<CheeseBallInteract> cheeseBallInteract = std::dynamic_pointer_cast<CheeseBallInteract>(Engine::GetInstance().entityManager->CreateEntity(EntityType::CHEESEBALLINTERACT));
                    cheeseBallInteract->position = Vector2D(x, y);
                    cheeseBallInteract->xInicial = (int)x;
                    cheeseBallInteract->yInicial = (int)y;
                    cheeseBallInteract->Start();
                    cheeseBallInteract->mapID = id;
                }
                else if (entityType == "Portrait") {
                    std::shared_ptr<Portrait> portrait = std::dynamic_pointer_cast<Portrait>(Engine::GetInstance().entityManager->CreateEntity(EntityType::PORTRAIT));
                    portrait->position = Vector2D(x, y);
                    portrait->xInicial = (int)x;
                    portrait->yInicial = (int)y;
                    portrait->Start();
                    portrait->mapID = id;
                }
                else if (entityType == "UnfinishedPortrait") {
                    std::shared_ptr<UnfinishedPortrait> unfinishedPortrait = std::dynamic_pointer_cast<UnfinishedPortrait>(Engine::GetInstance().entityManager->CreateEntity(EntityType::UNFINISHEDPORTRAIT));
                    unfinishedPortrait->position = Vector2D(x, y);
                    unfinishedPortrait->xInicial = (int)x;
                    unfinishedPortrait->yInicial = (int)y;
                    unfinishedPortrait->Start();
                    unfinishedPortrait->mapID = id;
                }
                else if (entityType == "HungSword") {
                    std::shared_ptr<HungSword> hungSword = std::dynamic_pointer_cast<HungSword>(Engine::GetInstance().entityManager->CreateEntity(EntityType::HUNGSWORD));
                    hungSword->position = Vector2D(x, y);
                    hungSword->xInicial = (int)x;
                    hungSword->yInicial = (int)y;
                    hungSword->Start();
                    hungSword->mapID = id;
                }
                else if (entityType == "CowWeb") {
                    std::shared_ptr<CowWeb> cowWeb = std::dynamic_pointer_cast<CowWeb>(Engine::GetInstance().entityManager->CreateEntity(EntityType::COWWEB));
                    cowWeb->position = Vector2D(x, y);
                    cowWeb->xInicial = (int)x;
                    cowWeb->yInicial = (int)y;
                    cowWeb->Start();
                    cowWeb->mapID = id;
                }
                else if (entityType == "Monument") {
                    std::shared_ptr<CommemorativeMonument> monument = std::dynamic_pointer_cast<CommemorativeMonument>(Engine::GetInstance().entityManager->CreateEntity(EntityType::MONUMENT));
                    monument->position = Vector2D(x, y);
                    monument->xInicial = (int)x;
                    monument->yInicial = (int)y;
                    monument->Start();
                    monument->mapID = id;
                }
                else if (entityType == "Nohuely") {
                    std::shared_ptr<Nohuely> nohuely = std::dynamic_pointer_cast<Nohuely>(Engine::GetInstance().entityManager->CreateEntity(EntityType::NOHUELY));
                    nohuely->position = Vector2D(x, y);
                    nohuely->xInicial = (int)x;
                    nohuely->yInicial = (int)y;
                    nohuely->Start();
                    nohuely->mapID = id;
                }
                else if (entityType == "death") {
                    std::shared_ptr<death> deathObj = std::dynamic_pointer_cast<death>(Engine::GetInstance().entityManager->CreateEntity(EntityType::DEATH));
                    deathObj->position = Vector2D(x, y);
                    deathObj->xInicial = (int)x;
                    deathObj->yInicial = (int)y;
                    deathObj->Start();
                    deathObj->mapID = id;
                }
                else if (entityType == "TowerGuard") {
                    std::shared_ptr<TowGuard> guard = std::dynamic_pointer_cast<TowGuard>(Engine::GetInstance().entityManager->CreateEntity(EntityType::GUARDTOWER));
                    guard->position = Vector2D(x, y);
                    guard->xInicial = (int)x;
                    guard->yInicial = (int)y;
                    guard->Start();
                    guard->mapID = id;
                }
                else if (entityType == "milkmaid") {
                    std::shared_ptr<milkmaid> MILKY = std::dynamic_pointer_cast<milkmaid>(Engine::GetInstance().entityManager->CreateEntity(EntityType::MILKMAID));
                    MILKY->position = Vector2D(x, y);
                    MILKY->xInicial = (int)x;
                    MILKY->yInicial = (int)y;
                    MILKY->Start();
                    MILKY->mapID = id;
                }
                else if (entityType == "Hermit") {
                    std::shared_ptr<Hermit> hermit = std::dynamic_pointer_cast<Hermit>(Engine::GetInstance().entityManager->CreateEntity(EntityType::HERMIT));
                    hermit->position = Vector2D(x, y);
                    hermit->xInicial = (int)x;
                    hermit->yInicial = (int)y;
                    hermit->Start();
                    hermit->mapID = id;
                }
                else if (entityType == "Timmy") {
                   
                        auto pics = std::dynamic_pointer_cast<Timmy>(
                           
                            Engine::GetInstance().entityManager->CreateEntity(EntityType::TIMMY)
                           
                            
                        );
                   


                        
                        pics->position = Vector2D(x, y);
                   
                        pics->xInicial = (int)x;
                    
                        pics->yInicial = (int)y;
                   
                        pics->mapID = id;
                

                        // Load Tiled custom properties
                       
                        Properties tempProperties;
                    
                        LoadProperties(objectNode, tempProperties);
                  


                        auto where = tempProperties.GetProperty("Where");
                
                        if (where)
                          
                        {
                           
                                pics->hidingPlace = where->valueInt;
                           
                                LOG("WhoIs = %i", pics->hidingPlace);
                            
                                pics->WhatPositionIsIt(pics->hidingPlace);
                           
                        }
                   


                        pics->Start();
                    
      
                        }

                else if (entityType == "Hierophant") {
                    std::shared_ptr<Hierophant> hierophant = std::dynamic_pointer_cast<Hierophant>(Engine::GetInstance().entityManager->CreateEntity(EntityType::HIEROPHANT));
                    hierophant->position = Vector2D(x, y);
                    hierophant->xInicial = (int)x;
                    hierophant->yInicial = (int)y;
                    hierophant->Start();
                    hierophant->mapID = id;
                }
                else if (entityType == "Well") {
                    std::shared_ptr<Well> well = std::dynamic_pointer_cast<Well>(Engine::GetInstance().entityManager->CreateEntity(EntityType::WELL));
                    well->position = Vector2D(x, y);
                    well->xInicial = (int)x;
                    well->yInicial = (int)y;
                    well->Start();
                    well->mapID = id;
                }
                else if (entityType == "Empress") {
                    std::shared_ptr<Empress> well = std::dynamic_pointer_cast<Empress>(Engine::GetInstance().entityManager->CreateEntity(EntityType::EMPRESS));
                    well->position = Vector2D(x, y);
                    well->xInicial = (int)x;
                    well->yInicial = (int)y;
                    well->Start();
                    well->mapID = id;
                    }
                else if (entityType == "Sculptor") {
                        std::shared_ptr<Sculptor> sculptor = std::dynamic_pointer_cast<Sculptor>(Engine::GetInstance().entityManager->CreateEntity(EntityType::SCULPTOR));
                        sculptor->position = Vector2D(x, y);
                        sculptor->xInicial = (int)x;
                        sculptor->yInicial = (int)y;
                        sculptor->Start();
                        sculptor->mapID = id;
                        }
                else if (entityType == "RetiredKnight") {
                            std::shared_ptr<RetiredKnight> retired = std::dynamic_pointer_cast<RetiredKnight>(Engine::GetInstance().entityManager->CreateEntity(EntityType::RETIREDKNIGHT));
                            retired->position = Vector2D(x, y);
                            retired->xInicial = (int)x;
                            retired->yInicial = (int)y;
                            retired->Start();
                            retired->mapID = id;
                            }
                
                else if (entityType == "Pics") {
                    auto pics = std::dynamic_pointer_cast<Pics>(Engine::GetInstance().entityManager->CreateEntity(EntityType::PICS));
                    pics->position = Vector2D(x, y);
                    pics->xInicial = (int)x;
                    pics->yInicial = (int)y;
                    pics->mapID = id;
                    Properties tempProperties;
                    LoadProperties(objectNode, tempProperties);
                    auto whoIsProp = tempProperties.GetProperty("WhoIs");
                    if (whoIsProp) {
                        pics->name = whoIsProp->valueString;
                        pics->ChooseWhoIs();
                    }
                    pics->Start();
                }
            }
        }
    }

    // --- LECTOR DE PARTICULAS ---
    for (pugi::xml_node objectGroupNode = mapFileXML.child("map").child("objectgroup"); objectGroupNode != NULL; objectGroupNode = objectGroupNode.next_sibling("objectgroup")) {
        if (objectGroupNode.attribute("name").as_string() == std::string("Particles")) {
            for (pugi::xml_node objectNode = objectGroupNode.child("object"); objectNode != NULL; objectNode = objectNode.next_sibling("object")) {
                std::string pType = objectNode.attribute("type").as_string();
                if (pType == "") {
                    pType = objectNode.attribute("class").as_string();
                }
                float x = objectNode.attribute("x").as_float();
                float y = objectNode.attribute("y").as_float();
                float w = objectNode.attribute("width").as_float();
                float h = objectNode.attribute("height").as_float();

                ParticleExample* p = new ParticleExample();
                p->setRenderer(Engine::GetInstance().render->renderer);
                p->setPosition((int)(x + (w / 2.0f)), (int)(y + (h / 2.0f)));

                if (pType == "Fire" || pType == "FIRE") p->setStyle(ParticleExample::FIRE);
                else if (pType == "Soup" || pType == "SOUP") p->setStyle(ParticleExample::SOUP);
                else if (pType == "Moho" || pType == "MOHO") p->setStyle(ParticleExample::MOHO);
                else if (pType == "Smoke" || pType == "SMOKE") p->setStyle(ParticleExample::SMOKE);
                else if (pType == "Rain" || pType == "RAIN") p->setStyle(ParticleExample::RAIN);
                else if (pType == "Meteor" || pType == "METEOR") p->setStyle(ParticleExample::METEOR);
                else if (pType == "Explosion" || pType == "EXPLOSION") p->setStyle(ParticleExample::EXPLOSION);
                else if (pType == "Snow" || pType == "SNOW") p->setStyle(ParticleExample::SNOW);

                p->setPosVar(Vec2(w / 2.0f, h / 2.0f));
                mapParticles.push_back(p);
            }
        }
    }
}

   






    //L15 TODO 4: Define a method to save entities to the map XML
void Map::SaveEntities(std::shared_ptr<Player> player) {
    for (pugi::xml_node objectGroupNode = mapFileXML.child("map").child("objectgroup"); objectGroupNode != NULL; objectGroupNode = objectGroupNode.next_sibling("objectgroup")) {
        if (objectGroupNode.attribute("name").as_string() == std::string("Entities")) {
            for (pugi::xml_node objectNode = objectGroupNode.child("object"); objectNode != NULL; objectNode = objectNode.next_sibling("object")) {
                std::string entityType = objectNode.attribute("type").as_string();
                if (entityType == "Player") {
                    Vector2D playerPos = player->GetPosition();
                    pugi::xml_attribute cpXAttr = objectNode.attribute("cpX");
                    if (!cpXAttr) cpXAttr = objectNode.append_attribute("cpX");
                    cpXAttr.set_value(playerPos.getX());

                    pugi::xml_attribute cpYAttr = objectNode.attribute("cpY");
                    if (!cpYAttr) cpYAttr = objectNode.append_attribute("cpY");
                    cpYAttr.set_value(playerPos.getY());

                    pugi::xml_attribute scoreAttr = objectNode.attribute("score");
                    if (!scoreAttr) scoreAttr = objectNode.append_attribute("score");
                    scoreAttr.set_value(Engine::GetInstance().scene->score);

                    pugi::xml_attribute timerAttr = objectNode.attribute("timer");
                    if (!timerAttr) timerAttr = objectNode.append_attribute("timer");
                    timerAttr.set_value(Engine::GetInstance().scene->levelTimer);
                }
                else {
                    int id = objectNode.attribute("id").as_int();
                    bool isDead = false;
                    for (int kId : killedEnemies) {
                        if (kId == id) { isDead = true; break; }
                    }
                    pugi::xml_attribute deadAttr = objectNode.attribute("isDead");
                    if (!deadAttr) deadAttr = objectNode.append_attribute("isDead");
                    deadAttr.set_value(isDead);
                }
            }
        }
    }
    std::string mapPathName = mapPath + mapFileName;
    mapFileXML.save_file(mapPathName.c_str());
}
    Vector2D Map::GetStartPoint(std::string layerName, std::string objectName)
    {
        for (const auto& group : mapData.objectgroups) {
            if (group->name == layerName) {
                for (const auto& object : group->objects) {
                    if (object->name == objectName) {
                        return Vector2D((float)object->x, (float)object->y);
                    }
                }
            }
        }
        return Vector2D(0, 0);
    }
    void Map::DrawLayer(std::string layerName) {
        if (!mapLoaded) return;

        for (const auto& mapLayer : mapData.layers) {
            if (mapLayer->name != layerName) continue;

            for (int i = 0; i < mapData.width; i++) {
                for (int j = 0; j < mapData.height; j++) {

                    // Evitar bugs si voltean tiles
                    unsigned int rawGid = mapLayer->Get(i, j);
                    int gid = rawGid & 0x1FFFFFFF;

                    if (gid != 0) {
                        TileSet* tileSet = GetTilesetFromTileId(gid);
                        if (tileSet != nullptr && tileSet->texture != nullptr) {

                            int relativeId = gid - tileSet->firstGid;

                            // Comprobamos si este tile tiene una animación guardada
                            if (tileSet->animations.find(relativeId) != tileSet->animations.end()) {
                                TileAnimation& anim = tileSet->animations[relativeId];
                                int frameLocalId = anim.frames[anim.currentFrame].tileId;

                                // Sobrescribimos el gid original por el gid del fotograma actual
                                gid = tileSet->firstGid + frameLocalId;
                            }

                            SDL_Rect tileRect = tileSet->GetRect(gid);
                            Vector2D mapCoord = MapToWorld(i, j);

                            // Calculamos la posición base en el mundo
                            int drawX = (int)mapCoord.getX();
                            int drawY = (int)mapCoord.getY() + mapData.tileHeight - tileSet->tileHeight;

                            // ====================================================
                            // --- NUEVA LÓGICA DE PARALLAX PARA CAPAS DE TILES ---
                            // ====================================================
                            // Como DrawTexture suma de forma automática la cámara para pintar en pantalla,
                            // restamos o sumamos la proporción de la velocidad multiplicada por (parallax - 1.0f).
                            float camX = Engine::GetInstance().render->camera.x;
                            float camY = Engine::GetInstance().render->camera.y;

                            drawX += (int)(camX * (mapLayer->parallaxX - 1.0f));
                            drawY += (int)(camY * (mapLayer->parallaxY - 1.0f));
                            // ====================================================

                            // Lógica de inversión (Flips)
                            bool flipHorizontal = (rawGid & 0x80000000) != 0;
                            bool flipVertical = (rawGid & 0x40000000) != 0;

                            SDL_FlipMode flip = SDL_FLIP_NONE;
                            if (flipHorizontal && flipVertical) flip = (SDL_FlipMode)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
                            else if (flipHorizontal) flip = SDL_FLIP_HORIZONTAL;
                            else if (flipVertical) flip = SDL_FLIP_VERTICAL;

                            // IMPORTANTE: En el 5º parámetro pasamos '1.0f' fijo para la escala visual del tamaño,
                            // ya que el parallaxX de Tiled controla la velocidad de scroll, no el tamaño de los bloques.
                            Engine::GetInstance().render->DrawTexture(
                                tileSet->texture,
                                drawX,
                                drawY,
                                &tileRect,
                                1.0f,                // Escala de tamaño (fijo a 1.0f)
                                0.0,                 // Ángulo
                                INT_MAX, INT_MAX,    // Pivotes
                                flip                 // Modo flip
                            );
                        }
                    }
                }
            }
        }
    }

    // L19 TODO 1: Calculate Camera position in Tiles
    Vector2D Map::GetCameraPositionInTiles() {

        // Gets the camera position in world space. Moving the camera right means drawing the world shifted left. 
        // Multiplying by -1 converts render offset actual world - space camera position
        Vector2D camPos = Vector2D(Engine::GetInstance().render->camera.x * -1, Engine::GetInstance().render->camera.y * -1);
        if (camPos.getX() < 0) camPos.setX(0);
        if (camPos.getY() < 0) camPos.setY(0);

        // Converts the camera position to map tile coordinates
        Vector2D camPosTile = WorldToMap(camPos.getX(), camPos.getY());

        return camPosTile;
    }

    // L19 TODO 2: Calculate Camera limits in Tiles
    Vector2D Map::GetCameraLimitsInTiles(Vector2D camPosTile) {

        // Gets the camera size in world space and converts it to map tile coordinates
        Vector2D camSize = Vector2D(Engine::GetInstance().render->camera.w, Engine::GetInstance().render->camera.h);
        Vector2D camSizeTile = WorldToMap(camSize.getX(), camSize.getY());

        // Computes the tile range to draw
        Vector2D limits = Vector2D(camPosTile.getX() + camSizeTile.getX(), camPosTile.getY() + camSizeTile.getY());
        if (limits.getX() > mapData.width) limits.setX(mapData.width);
        if (limits.getY() > mapData.height) limits.setY(mapData.height);

        return limits;
    }

    ObjectGroup::Object* Map::GetObject(std::string layerName, std::string objectName)
    {
        for (const auto& group : mapData.objectgroups) {
            if (group->name == layerName) {
                for (const auto& object : group->objects) {
                    if (object->name == objectName) {
                        return object;
                    }
                }
            }
        }
        return nullptr;
    }
    ObjectGroup::Object* Map::GetObjectByProperty(std::string layerName, std::string propName, std::string value)
    {
        LOG("Buscando spawn: %s", Engine::GetInstance().scene->nextSpawnPoint.c_str());
        for (const auto& group : mapData.objectgroups) {
            if (group->name == layerName) {
                for (const auto& object : group->objects) {

                    auto prop = object->properties.GetProperty(propName);

                    if (prop) {
                        LOG("Comparando: [%s] con [%s]", prop->valueString.c_str(), value.c_str());
                    }

                    if (prop && prop->valueString == value) {
                        LOG("MATCH!");
                        return object;
                    }
                }
            }
        }
        return nullptr;
    }

    void Map::DrawObjectLayerParallax(std::string layerName, float parallaxSpeed)
    {
        if (!mapLoaded) return;

        bool layerFound = false;

        for (const auto& group : mapData.objectgroups) {
            if (group->name == layerName) {
                layerFound = true;
                //LOG("PARALLAX DEBUG: Capa '%s' encontrada. Tiene %d objetos.", layerName.c_str(), group->objects.size());

                for (const auto& object : group->objects) {

                    unsigned int rawGid = object->gid; // <--- Asegurar unsigned
                    int cleanGid = rawGid & 0x1FFFFFFF;

                    if (cleanGid != 0) {
                        TileSet* tileSet = GetTilesetFromTileId(cleanGid);

                        if (tileSet != nullptr && tileSet->texture != nullptr) {
                            SDL_Rect tileRect = tileSet->GetRect(cleanGid);

                            // --- PARCHE DE TAMAÑO ---
                            int finalWidth = object->width;
                            int finalHeight = object->height;
                            if (finalWidth == 0 || finalHeight == 0) {
                                finalWidth = tileRect.w;
                                finalHeight = tileRect.h;
                            }

                            // --- NUEVA LÓGICA DE FLIP ---
                            bool flipHorizontal = (rawGid & 0x80000000) != 0;
                            bool flipVertical = (rawGid & 0x40000000) != 0;

                            SDL_FlipMode flip = SDL_FLIP_NONE;
                            if (flipHorizontal && flipVertical) flip = (SDL_FlipMode)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
                            else if (flipHorizontal) flip = SDL_FLIP_HORIZONTAL;
                            else if (flipVertical) flip = SDL_FLIP_VERTICAL;

                            Engine::GetInstance().render->DrawParallax(
                                tileSet->texture,
                                object->x,
                                object->y - finalHeight,
                                finalWidth,
                                finalHeight,
                                &tileRect,
                                parallaxSpeed,
                                0, 0, 0,
                                flip // <--- APLICAMOS EL FLIP EN VEZ DE SDL_FLIP_NONE
                            );
                        }
                    }
                }
            }
        }

        if (!layerFound) {
            //LOG("PARALLAX DEBUG: ERROR FATAL - La capa '%s' no existe en el mapa.", layerName.c_str());
        }
    }

    void Map::ParseLayersRecursive(pugi::xml_node parentNode)
    {
        for (pugi::xml_node node = parentNode.first_child(); node; node = node.next_sibling()) {
            std::string nodeName = node.name();

            if (nodeName == "layer") {
                MapLayer* mapLayer = new MapLayer();
                mapLayer->id = node.attribute("id").as_int();
                mapLayer->name = node.attribute("name").as_string();
                mapLayer->width = node.attribute("width").as_int();
                mapLayer->height = node.attribute("height").as_int();

                mapLayer->parallaxX = node.attribute("parallaxx").as_float(1.0f);
                mapLayer->parallaxY = node.attribute("parallaxy").as_float(1.0f);

                LoadProperties(node, mapLayer->properties);

                for (pugi::xml_node tileNode = node.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
                    mapLayer->tiles.push_back(tileNode.attribute("gid").as_uint());
                }

                mapData.layers.push_back(mapLayer);
            }
            else if (nodeName == "group") {
                ParseLayersRecursive(node);
            }
        }
    }

void Map::ParseObjectGroupsRecursive(pugi::xml_node parentNode)
{
    for (pugi::xml_node node = parentNode.first_child(); node; node = node.next_sibling()) {
        std::string nodeName = node.name();

        if (nodeName == "objectgroup") {
            ObjectGroup* objectGroup = new ObjectGroup();
            objectGroup->id = node.attribute("id").as_int();
            objectGroup->name = node.attribute("name").as_string();
            LoadProperties(node, objectGroup->properties);

            for (pugi::xml_node objectNode = node.child("object"); objectNode != NULL; objectNode = objectNode.next_sibling("object")) {
                ObjectGroup::Object* object = new ObjectGroup::Object();
                object->id = objectNode.attribute("id").as_int();
                object->name = objectNode.attribute("name").as_string();
                object->x = objectNode.attribute("x").as_int();
                object->y = objectNode.attribute("y").as_int();
                object->width = objectNode.attribute("width").as_int();
                object->height = objectNode.attribute("height").as_int();
                object->gid = objectNode.attribute("gid").as_uint(0);
                LoadProperties(objectNode, object->properties);

                objectGroup->objects.push_back(object);
            }

            mapData.objectgroups.push_back(objectGroup);
        }
        else if (nodeName == "group") {
            ParseObjectGroupsRecursive(node);
        }
    
    }
}


int Map::GetTileFromLayer(std::string layerName, int worldX, int worldY)
{
    if (!mapLoaded) return 0;

    // Convertimos la posición del mundo a casillas de Tiled (i, j)
    Vector2D tileCoords = WorldToMap(worldX, worldY);
    int i = (int)tileCoords.getX();
    int j = (int)tileCoords.getY();

    // Seguridad por si miramos fuera del mapa
    if (i < 0 || i >= mapData.width || j < 0 || j >= mapData.height) return 0;

    // Buscamos la capa
    for (const auto& layer : mapData.layers) {
        if (layer->name == layerName) {
            // Devolvemos el GID limpio
            return layer->Get(i, j) & 0x1FFFFFFF;
        }
    }
    return 0; // Si no encuentra la capa o está vacío
}

void Map::SpawnParticle(ParticleExample::PatticleStyle style, int x, int y, float duration)
{
    ParticleExample* p = new ParticleExample();
    p->setRenderer(Engine::GetInstance().render->renderer);
    p->setPosition(x, y);
    p->setStyle(style);

    // Le decimos que solo viva X segundos
    p->setDuration(duration);

    mapParticles.push_back(p);
}

void Map::DestroyBodyByName(std::string name)
{
    for (auto it = mapBodies.begin(); it != mapBodies.end(); ) {
        if ((*it) != nullptr && (*it)->objectName == name) {
            Engine::GetInstance().physics->DeletePhysBody(*it);
            it = mapBodies.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Map::DrawForeground() {
    if (!mapLoaded) return;

    // 1. DIBUJAR CAPAS DE TILES MARCADAS COMO FOREGROUND
    for (const auto& mapLayer : mapData.layers) {
        auto isForeground = mapLayer->properties.GetProperty("Foreground");
        bool drawForeground = (isForeground != nullptr && isForeground->value == true);

        if (drawForeground) {
            if (mapLayer->properties.GetProperty("Draw") != NULL && mapLayer->properties.GetProperty("Draw")->value == true) {
                DrawLayer(mapLayer->name);
            }
        }
    }

    // 2. DIBUJAR OBJETOS PARALLAX FRONTALES (Lo que antes era el bloque 3 del Update)
    for (const auto& group : mapData.objectgroups) {
        auto drawProp = group->properties.GetProperty("Draw");
        auto bgProp = group->properties.GetProperty("Background");

        if (drawProp && drawProp->value == true && (bgProp == NULL || bgProp->value == false)) {
            float parallax = 1.0f;
            auto parallaxProp = group->properties.GetProperty("Parallax");
            if (parallaxProp) parallax = parallaxProp->valueFloat;

            DrawObjectLayerParallax(group->name, parallax);
        }
    }
}