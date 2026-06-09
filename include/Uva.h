#pragma once

#include "Entity.h"
#include <string>

enum class UvaSize {
    PEQUEÑA,
    MEDIANA,
    GRANDE
};

enum class UvaType {
    METEORO,
    LINEAL
};

class Uva : public Entity
{
public:
    Uva();
    virtual ~Uva();

    bool Start() override;
    bool Update(float dt) override;
    void Draw(float dt);

    void OnCollision(PhysBody* physA, PhysBody* physB) override;

    // Configuración previa al Start
    void SetConfiguration(UvaSize size, UvaType type, float directionX = 0.0f);

private:
    UvaSize sizeConfig = UvaSize::PEQUEÑA;
    UvaType typeConfig = UvaType::METEORO;
    float dirX = 0.0f;

    SDL_Texture* texture = nullptr;
    PhysBody* pbody;

    int radius = 0;
    int texW = 0;
    int texH = 0;
    float speed = 0.0f;
};