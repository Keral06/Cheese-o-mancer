#pragma once

#include "Enemy.h"


class BossFightPrincessKnight;

enum class KnightState
{
    IDLE,

    ENTRANCE_DASH,
    POSITIONING,

    TRANSFORM,

    LUNGE_ATTACK,
    BOUNCE_ATTACK,

    RECOVER,

    DEATH
};

class KnightBoss : public Enemy
{
public:

    KnightBoss();
    virtual ~KnightBoss();

    bool Start() override;
    bool Update(float dt) override;

    void Draw(float dt);
    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;
    // =========================
    // STATE
    // =========================

    void SetKnightState(KnightState newState);

    // =========================
    // ATTACKS
    // =========================

    void StartEntrance();

    void StartTransform();

    void StartLungeAttack(float speedMultiplier = 1.0f);

    void StartBounceAttack();

    // =========================
    // HELPERS
    // =========================

    void SetFightController(BossFightPrincessKnight* controller);

    bool IsBusy() const;

    bool IsInvulnerable() const;

    bool HasFinishedAction() const;

    void ResetActionFinished();

    void ChangeCurrentAnimation() override;

    void FinishAction();

private:

    // =========================
    // INTERNAL UPDATES
    // =========================

    void UpdateEntrance(float dt);

    void UpdateLunge(float dt);

    void UpdateBounce(float dt);

    void UpdateRecover(float dt);

    // =========================
    // REFERENCES
    // =========================

    BossFightPrincessKnight* fightController;

    // =========================
    // STATES
    // =========================

    KnightState knightState;
    KnightState lastKnightState;

    // =========================
    // TIMERS
    // =========================

    float stateTimer;

    // =========================
    // MOVEMENT
    // =========================

    Vector2D targetPosition;

    Vector2D attackDirection;

    float attackSpeed;

    int bounceCount;

    bool busy;

    bool invulnerable;

    bool actionFinished;

    bool canAct = true; //Temporal

    // ===============================
    // ANIMATION
    // ===============================

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

    // ===============================
    // Bounce
    // ===============================

    Vector2D bounceDir;
    float bounceSpeed;
    int maxBounces;
};