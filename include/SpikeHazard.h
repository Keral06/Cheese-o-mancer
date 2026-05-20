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

    void Activate();

private:

    enum class State
    {
        WARNING,
        ACTIVE
    };

    void Draw(float dt);

private:

    State state;

    float timer;

    float warningDuration;
    float activeDuration;

    SDL_Texture* texture;

    AnimationSet animWarning;
    AnimationSet animActive;

    AnimationSet* currentAnim;

    PhysBody* pbody;
};


