#pragma once
#include "DestructibleEntity.h"
#include "Verdugo.h"

class WeakWall : public DestructibleEntity
{
public:
    WeakWall();

    bool Start() override;
    bool Destroy() override;

    Verdugo* owner = nullptr;
    int width = 0;
    int height = 0;
};