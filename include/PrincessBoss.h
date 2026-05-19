// ===============================
// PrincessBoss.h
// ===============================

#pragma once

#include "Enemy.h"

class BossFightPrincessKnight;

enum class PrincessState
{
    IDLE,

    TRANSFORM,

    FLOWER_ATTACK,
    SPIKE_ATTACK,

    CASTING,

    DEATH
};

class PrincessBoss : public Enemy
{
public:

    PrincessBoss();
    virtual ~PrincessBoss();

    bool Start() override;
    bool Update(float dt) override;

    void Draw(float dt);

    // ===============================
    // STATES
    // ===============================

    void SetPrincessState(PrincessState newState);

    // ===============================
    // ATTACKS
    // ===============================

    void StartTransform();

    void StartFlowerAttack(int amount);

    void StartSpikeAttack();

    // ===============================
    // HELPERS
    // ===============================

    void SetFightController(BossFightPrincessKnight* controller);

    bool IsBusy() const;

    bool HasFinishedAction() const;

    void ResetActionFinished();

    void ChangeCurrentAnimation() override;

private:

    // ===============================
    // INTERNAL UPDATES
    // ===============================

    void UpdateFlowerAttack(float dt);

    void UpdateSpikeAttack(float dt);

    // ===============================
    // REFERENCES
    // ===============================

    BossFightPrincessKnight* fightController;

    // ===============================
    // STATES
    // ===============================

    PrincessState princessState;
    PrincessState lastPrincessState;

    // ===============================
    // TIMERS
    // ===============================

    float stateTimer;

    // ===============================
    // ATTACK DATA
    // ===============================

    int flowersToSpawn;

    bool busy;

    bool actionFinished;

    AnimationSet animsNIdle;
    AnimationSet animsNSlide;
    AnimationSet animsCAttack1;
    AnimationSet animsCAttack2;
    AnimationSet animsCDefeat;
    AnimationSet animsCIdle;
    AnimationSet animsCTransform;

    SDL_Texture* textureNIdle = nullptr;
    SDL_Texture* textureNSlide = nullptr;
    SDL_Texture* textureCAttack1 = nullptr;
    SDL_Texture* textureCAttack2 = nullptr;
    SDL_Texture* textureCDefeat = nullptr;
    SDL_Texture* textureCIdle = nullptr;
    SDL_Texture* textureCTransform = nullptr;

    AnimationSet* currentAnim;
    SDL_Texture* currentTexture;
};