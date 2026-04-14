#pragma once
#include "Enemy.h"

class Verdugo : public Enemy
{
public:
    Verdugo();
    ~Verdugo();

    bool Start() override;
    void Attack() override;
    bool Update(float dt) override;
protected:
    int attackTimer = 0;
    int attackCooldown = 50;
};