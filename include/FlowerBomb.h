#pragma once

#include "Entity.h"
#include "Animation.h"

class PhysBody;
struct SDL_Texture;

class FlowerBomb : public Entity
{
public:

    FlowerBomb();
    virtual ~FlowerBomb();

    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    void Explode();

private:

    enum class State
    {
        IDLE,
        EXPLODING
    };

    void Draw(float dt);

private:

    State state;

    float timer;
    float explodeDelay;

    SDL_Texture* texture = nullptr;

    AnimationSet animIdle;
    AnimationSet animExplosion;

    AnimationSet* currentAnim = nullptr;

    PhysBody* pbody = nullptr;
};