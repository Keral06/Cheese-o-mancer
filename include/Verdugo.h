#pragma once
#include "Enemy.h"
#include "Physics.h"

enum VerdugoState {
    IDLEV,
    WALKV,
    ATAQUEP1,
    ATAQUE1,
    ATAQUE2,
    ATAQUE3START,
    ATAQUE3A,
    ATAQUE3B,
    ATAQUE3C,
    ATAQUE4A,
    ATAQUE4B,
    MUERTO
};

enum AttackType {
    ATTACK_1,
    ATTACK_2,
    ATTACK_3,
    ATTACK_4
};

enum BossPhase
{
    PHASE_INTROV,
    PHASE_1V,
    PHASE_TRANSFORMV,
    PHASE_2V,
    PHASE_DEADV,
    PHASE_ENDCHOICEV
};

class Verdugo : public Enemy
{
public:
    Verdugo();
    ~Verdugo();

    bool Start() override;
    void Attack() override;
    bool Update(float dt) override;
    void UpdateIntro(float dt);
    void UpdatePhase1(float dt);
    void ExecutePhase1Attack();

    void UpdatePhase1Attack();

    void StartTransformation();
    void UpdateTransformation(float dt);
    void UpdatePhase2(float dt);
    void EnterEndState();
    void UpdateEndChoice(float dt);
    void Draw(float dt) override;
    void UpdateAttack();
    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;
    void ChangeCurrentAnimation();
    void DebugChangeState();
    AttackType ChooseRandomAttack();

    void ExecuteAttack();

    void UpdateAttackLogic();
    void Die();
    void SpawnWeakWall();

    void DebugChangePhase();


    bool MoveToAttackRange(float targetRange);
    void OnWallDestroyed();
   
protected:
    int attackTimer = 0;
    int attackCooldown = 50;
    float attackDuration = 20.0f;

    float hitboxStart = 5.0f;
    float hitboxEnd = 15.0f;
    bool hasHit = false;
    bool hitboxActive = false;
    bool coinDropped = false;
    bool isAttacking = false;

    bool playerInHitbox = true;

    AnimationSet animsAtaqueFase1;
    AnimationSet animsIdle;

    SDL_Texture* textureAF1 = NULL;
    SDL_Texture* textureIdleWalk = NULL;

 

    AnimationSet animsAtaque1;
    AnimationSet animsAtaque2;
    AnimationSet animsAtaque3a;
    AnimationSet animsAtaque3b;
    AnimationSet animsAtaque3c;
    AnimationSet animsAtaque4a;
    AnimationSet animsAtaque4b;
    AnimationSet animsDeath;

    AnimationSet* currentAnimSet = nullptr;

    VerdugoState state;
    VerdugoState lastState;

    SDL_Texture* textureA1 = NULL;
    SDL_Texture* textureA2 = NULL;
    SDL_Texture* textureA3a = NULL;
    SDL_Texture* textureA3b = NULL;
    SDL_Texture* textureA3c = NULL;
    SDL_Texture* textureA4a = NULL;
    SDL_Texture* textureA4b = NULL;
    SDL_Texture* textureDeath = NULL;
    SDL_Texture* textureT = NULL;
    
    float offsetY = 0.0f;
    float offsetX = 0.0f;

    AttackType currentAttack = ATTACK_1;
    bool attackInProgress = false;
    bool attackFinished = false;

    float attackRange1 = 80.0f;
    float attackRange2 = 200.0f;
    float attackRange3 = 120.0f;

    bool facingLeft = false;
    bool bolazo = false;

    bool wallSpawned = false;
    bool wallDestroyed = false;

    int limitRight = 10100;
    int limitLeft = 900;

    BossPhase phase = PHASE_INTROV;

    bool introFinished = false;
    bool transformationFinished = false;
    bool phase1Defeated = false;
    bool phase2Defeated = false;

    bool introTriggered = false;
    bool introCinematicDone = false;
    float introTimer = 0.0f;

    int bodyW;
    int bodyH;
    bodyType type;
};