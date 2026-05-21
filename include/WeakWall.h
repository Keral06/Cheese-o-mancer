#pragma once
#include "DestructibleEntity.h"
#include "Verdugo.h"

class WeakWall : public DestructibleEntity
{
public:
    WeakWall();

    bool Start() override;
    bool Destroy() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    bool drawed = true;
    SDL_Texture* texture = nullptr;
    Verdugo* owner = nullptr;
    int width = 0;
    int height = 0;
};