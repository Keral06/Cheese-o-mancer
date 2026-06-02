#pragma once

#include "Entity.h"
#include "scene.h"
#include "Physics.h"
#include "Engine.h"

class Door : public Entity
{
public:

    Door();
    virtual ~Door();

    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    void OnCollision(PhysBody* physA, PhysBody* physB) override;

    // Setup desde el mapa
    void SetDoorData(
        const std::string& targetMap,
        const std::string& targetDoor,
        int offsetX,
        int offsetY,
        int width,
        int height
    );

public:

    std::string targetMap;
    std::string targetDoor;

    int offsetX = 0;
    int offsetY = 0;

    Vector2D position;
    int width = 0;
    int height = 0;

private:

    PhysBody* pbody = nullptr;
};