#pragma once
#include "DestructibleEntity.h"

class WeakWall : public DestructibleEntity
{
public:
    WeakWall();

    bool Start() override;
    bool Destroy() override;

    int width = 0;
    int height = 0;
};