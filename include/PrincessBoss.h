// ===============================
// PrincessBoss.h
// ===============================

#pragma once

#include "Enemy.h"
#include <vector>
#include "SpikeHazard.h"

struct SpawnedPos
{
    Vector2D pos;
};



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

    std::shared_ptr<SpikeHazard> PrincessBoss::SpawnSpike(Vector2D pos);

    bool IsFarEnough(Vector2D pos);

    void SpawnFlowerGrid();

    void SpawnSpikeWave(float centerX);

    void ReturnToBase(Vector2D pos);

    void FinishAction();

    void AdjustHitboxY(int offsetPxl);

public:

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
    AnimationSet animsCTransform;
    

    SDL_Texture* textureNIdle = nullptr;
    SDL_Texture* textureDeath = nullptr;
    SDL_Texture* textureDefeat = nullptr;
    SDL_Texture* textureMagic = nullptr;
    SDL_Texture* textureCIdle = nullptr;
    SDL_Texture* textureMove = nullptr;
    SDL_Texture* textureTransform = nullptr;

    SDL_Texture* spikeTexture = nullptr;

    AnimationSet* currentAnim;
    SDL_Texture* currentTexture;

    float offsetX = 0.0f;
    float offsetY = 0.0f;

    std::vector<Vector2D> spawnedFlowers;
    float flowerSpawnTimer = 0.0f;
    int flowersSpawned = 0;

    int gridRows = 4;
    int gridCols = 6;

    bool gridUsed[4][6] = { false };

    std::shared_ptr<SpikeHazard> leftBorderSpike = nullptr;
    std::shared_ptr<SpikeHazard> rightBorderSpike = nullptr;

    float waveSpawnTimer = 0.0f;

    float waveX = 0.0f;

    bool bordersSpawned = false;
    bool waveStarted = false;

    bool isTransforming = false;
    bool isTransformed = false;
};