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

    CIDLE,

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

    void SpawnFlower(Vector2D pos);

    void SpawnSpike(Vector2D pos);

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

    bool spawnedFlower = false;
    bool spawnedSpike = false;

    AnimationSet animsNIdle;
    AnimationSet animsCIdle;
    AnimationSet animsDeath;
    AnimationSet animsDefeat;
    AnimationSet animsMagic;
    AnimationSet animsMove;

    SDL_Texture* textureNIdle = nullptr;
    SDL_Texture* textureDeath = nullptr;
    SDL_Texture* textureDefeat = nullptr;
    SDL_Texture* textureMagic = nullptr;
    SDL_Texture* textureCIdle = nullptr;
    SDL_Texture* textureMove = nullptr;

    AnimationSet* currentAnim;
    SDL_Texture* currentTexture;


};