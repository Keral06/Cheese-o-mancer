// ===============================
// PrincessBoss.cpp
// ===============================

#include "PrincessBoss.h"

#include "BossFightPrincessKnight.h"

#include "Engine.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Textures.h"

// ===============================
// CONSTRUCTOR
// ===============================

PrincessBoss::PrincessBoss() : Enemy()
{
    name = "PrincessBoss";
}

PrincessBoss::~PrincessBoss()
{
}

// ===============================
// START
// ===============================

bool PrincessBoss::Start()
{
    texW = 128*2;
    texH = 128*3;

    type = EnemyType::RANGED;

    std::unordered_map<int, std::string> aliasesNIdle = { {0,"idle"} };
    std::unordered_map<int, std::string> aliasesNSlide = { {0,"slide"} };
    std::unordered_map<int, std::string> aliasesCIdle = { {0,"idle"} };
    std::unordered_map<int, std::string> aliasesCAttack1 = { {0,"attack1"} };
    std::unordered_map<int, std::string> aliasesCAttack2 = { {0,"attack2"} };
    std::unordered_map<int, std::string> aliasesCDefeat = { {0,"defeat"} };
    std::unordered_map<int, std::string> aliasesCTransform = { {0,"transform"}, {21, "death"} };

    animsNIdle.LoadFromTSX("assets/Textures/Spritesheets/Knight/Normal/kn_idle.tsx", aliasesNIdle);
    animsNSlide.LoadFromTSX("assets/Textures/Spritesheets/Knight/Normal/kn_slide.tsx", aliasesNSlide);
    animsCIdle.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/j_sp_3x4.tsx", aliasesCIdle);
    animsCAttack1.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/kc_attack1.tsx", aliasesCAttack1);
    animsCAttack2.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/j_sp_5x5.tsx", aliasesCAttack2);
    animsCDefeat.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/j_sp_5x5.tsx", aliasesCDefeat);
    animsCTransform.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/j_sp_5x5.tsx", aliasesCTransform);

    textureNIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Normal/KnightN_Idle.png");
    textureNSlide = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Normal/KnightN_sliding.png");
    textureCIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Idle.png");
    textureCAttack1 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Attack1.png");
    textureCAttack2 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Attack2.png");
    textureCDefeat = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Defeat.png");
    textureCTransform = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Transform_And_Walk.png");

    texture = textureNIdle;

    pbody = Engine::GetInstance().physics->CreateRectangleFriction(position.getX(), position.getY(), texW, texH, bodyType::DYNAMIC, 0.0f);


    pbody->listener = this;


    pbody->ctype = ColliderType::ENEMY;


    pathfinding = std::make_shared<Pathfinding>();

    Vector2D pos = GetPosition();

    Vector2D tilePos = Engine::GetInstance().map->WorldToMap((int)pos.getX(), (int)pos.getY());

    pathfinding->ResetPath(tilePos);

    actionFinished = false;
    attackHitbox = nullptr;

    currentAnim = &animsNIdle;
    currentTexture = textureNIdle;

    princessState = PrincessState::IDLE;
    lastPrincessState = PrincessState::IDLE;

    stateTimer = 0.0f;

    flowersToSpawn = 0;

    busy = false;

    return true;
}

// ===============================
// UPDATE
// ===============================

bool PrincessBoss::Update(float dt)
{
    stateTimer += dt;

    GetPhysicsValues();

    switch (princessState)
    {
    case PrincessState::IDLE:
        break;

    case PrincessState::TRANSFORM:
    {
        if (stateTimer >= 2.0f)
        {
            busy = false;

            SetPrincessState(PrincessState::IDLE);
        }

        break;
    }

    case PrincessState::FLOWER_ATTACK:
        UpdateFlowerAttack(dt);
        break;

    case PrincessState::SPIKE_ATTACK:
        UpdateSpikeAttack(dt);
        break;

    case PrincessState::CASTING:
        break;

    case PrincessState::DEATH:
        break;
    }

    Draw(dt);

    return true;
}

// ===============================
// TRANSFORM
// ===============================

void PrincessBoss::StartTransform()
{
    busy = true;

    stateTimer = 0.0f;

    SetPrincessState(PrincessState::TRANSFORM);

    LOG("Princess Transform");
}

// ===============================
// FLOWER ATTACK
// ===============================

void PrincessBoss::StartFlowerAttack(int amount)
{
    busy = true;

    flowersToSpawn = amount;

    stateTimer = 0.0f;

    SetPrincessState(PrincessState::FLOWER_ATTACK);

    LOG("Princess Flower Attack");
}

void PrincessBoss::UpdateFlowerAttack(float dt)
{
    // ===============================
    // TODO:
    // Spawn flower bombs
    // ===============================

    // Ejemplo futuro:
    //
    // for(int i = 0; i < flowersToSpawn; ++i)
    // {
    //      SpawnFlower();
    // }

    if (stateTimer >= 2.0f)
    {
        busy = false;

        SetPrincessState(PrincessState::IDLE);

        if (fightController)
            fightController->EndCurrentTurn();
            actionFinished = true;
    }
}

// ===============================
// SPIKE ATTACK
// ===============================

void PrincessBoss::StartSpikeAttack()
{
    busy = true;

    stateTimer = 0.0f;

    SetPrincessState(PrincessState::SPIKE_ATTACK);

    LOG("Princess Spike Attack");
}

void PrincessBoss::UpdateSpikeAttack(float dt)
{
    // ===============================
    // TODO:
    // Spawn spikes
    // ===============================

    if (stateTimer >= 2.5f)
    {
        busy = false;

        SetPrincessState(PrincessState::IDLE);

        if (fightController)
            fightController->EndCurrentTurn();
            actionFinished = true;
    }
}

// ===============================
// HELPERS
// ===============================

void PrincessBoss::SetFightController(BossFightPrincessKnight* controller)
{
    fightController = controller;
}

bool PrincessBoss::IsBusy() const
{
    return busy;
}

// ===============================
// STATE
// ===============================

void PrincessBoss::SetPrincessState(PrincessState newState)
{
    if (princessState == newState)
        return;

    lastPrincessState = princessState;
    princessState = newState;

    stateTimer = 0.0f;

    LOG("Princess State: %d", (int)princessState);
}

// ===============================
// DRAW
// ===============================

void PrincessBoss::Draw(float dt)
{
    if (currentAnim == nullptr)
        return;

    currentAnim->Update(dt);

    const SDL_Rect& animFrame =
        currentAnim->GetCurrentFrame();

    int x, y;

    if (pbody != nullptr)
    {
        pbody->GetPosition(x, y);

        position.setX((float)x);
        position.setY((float)y);
    }
    else
    {
        x = (int)deathPosition.getX();
        y = (int)deathPosition.getY();
    }

    SDL_FlipMode flip =
        facingLeft ?
        SDL_FLIP_NONE :
        SDL_FLIP_HORIZONTAL;

    Engine::GetInstance().render->DrawTexture(
        currentTexture,
        x - texW / 2,
        y - texH / 2,
        &animFrame,
        1.0f,
        0.0,
        INT_MAX,
        INT_MAX,
        flip
    );
}

bool PrincessBoss::HasFinishedAction() const
{
    return actionFinished;
}

void PrincessBoss::ResetActionFinished()
{
    actionFinished = false;
}

void PrincessBoss::ChangeCurrentAnimation()
{
    /*switch (princessState)
    {
    case PrincessState::IDLE:
        currentAnim = &animsIdle;
        currentTexture = texIdle;
        break;

    case PrincessState::TRANSFORM:
        currentAnim = &animsTransform;
        currentTexture = texTransform;
        break;

    case PrincessState::FLOWER_ATTACK:
        currentAnim = &animsFlower;
        currentTexture = texFlower;
        break;

    case PrincessState::SPIKE_ATTACK:
        currentAnim = &animsSpike;
        currentTexture = texSpike;
        break;

    case PrincessState::DEATH:
        currentAnim = &animsDeath;
        currentTexture = texDeath;
        break;
    }

    if (currentAnim)
        currentAnim->Resets();*/
}