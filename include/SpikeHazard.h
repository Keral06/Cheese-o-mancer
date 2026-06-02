// ==========================================
// SpikeHazard.h
// ==========================================

#pragma once

#include "Entity.h"
#include "Animation.h"

class PhysBody;
struct SDL_Texture;

class SpikeHazard : public Entity
{
public:

    SpikeHazard();
    ~SpikeHazard();

    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    void Resume();
    void SetBorderSpike(bool v);
    void SetTexture(SDL_Texture* tex);


private:

    enum class State
    {
        RISING,
        ACTIVE,
        DEAD
    };

    void Draw(float dt);

private:

    State state;

    SDL_Texture* texture;

    AnimationSet anim;
    
    bool borderSpike = false;

    bool pausedAtTop = false;

    SpikeHazard* leftBorder = nullptr;
    SpikeHazard* rightBorder = nullptr;

    PhysBody* pbody;

    float spawnDelay = 0.0f;
    float timeAlive = 0.0f;
    bool activated = false;

};


