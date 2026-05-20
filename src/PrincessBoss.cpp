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
#include "EntityManager.h"

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
    texH = 128*4;

    type = EnemyType::RANGED;

    std::unordered_map<int, std::string> aliasesNIdle = { {0,"idle"}, {18,"prepared"}};
    std::unordered_map<int, std::string> aliasesCIdle = { {0,"idle"} };
    std::unordered_map<int, std::string> aliasesDeath = { {0,"death"} };
    std::unordered_map<int, std::string> aliasesDefeat = { {0,"defeat"} };
    std::unordered_map<int, std::string> aliasesMagic = { {0,"magic"} };
    std::unordered_map<int, std::string> aliasesMove = { {0,"move"} };

    animsNIdle.LoadFromTSX("assets/Textures/Spritesheets/Princess/pn_idle_prepared.tsx", aliasesNIdle);
    animsCIdle.LoadFromTSX("assets/Textures/Spritesheets/Princess/pt_idle.tsx", aliasesCIdle);
    animsDeath.LoadFromTSX("assets/Textures/Spritesheets/Princess/pt_death.tsx", aliasesDeath);
    animsDefeat.LoadFromTSX("assets/Textures/Spritesheets/Princess/pt_defeat.tsx", aliasesDefeat);
    animsMagic.LoadFromTSX("assets/Textures/Spritesheets/Princess/pt_magic.tsx", aliasesMagic);
    animsMove.LoadFromTSX("assets/Textures/Spritesheets/Princess/pt_move.tsx", aliasesMove);
    /*animsCTransform.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/j_sp_5x5.tsx", aliasesCTransform);*/

    textureNIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Princess/princessN_Idle+Prepared.png");
    textureCIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Princess/sprite_princessT_01_idle.png");
    textureDeath = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Princess/sprite_princessT_06_death.png");
    textureDefeat = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Princess/sprite_princessT_05_defeat.png");
    textureMagic = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Princess/sprite_princessT_02_magic.png");
    textureMove = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Princess/sprite_princessT_03_move.png");

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
    stateTimer += dt;

    if (stateTimer > 0.5f && !spawnedFlower)
    {
        spawnedFlower = true;

        Vector2D pos = GetPosition();
        pos.setX(pos.getX() + 50);

        SpawnFlower(pos);
    }

    if (stateTimer >= 2.0f)
    {
        busy = false;

        SetPrincessState(PrincessState::IDLE);

        actionFinished = true;

        if (fightController)
            fightController->EndCurrentTurn();
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
    stateTimer += dt;

    if (stateTimer > 0.5f && !spawnedSpike)
    {
        spawnedSpike = true;

        Vector2D pos = GetPosition();
        pos.setY(pos.getY() + 50);

        SpawnSpike(pos);
    }

    if (stateTimer >= 2.5f)
    {
        busy = false;

        SetPrincessState(PrincessState::IDLE);

        actionFinished = true;

        if (fightController)
            fightController->EndCurrentTurn();
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

    ChangeCurrentAnimation();
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
        x - animFrame.w / 2,
        y - animFrame.h / 2,
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
    switch (princessState)
    {
    case PrincessState::IDLE:
        currentAnim = &animsNIdle;
        currentTexture = textureNIdle;
        break;

    case PrincessState::CIDLE:
        currentAnim = &animsCIdle;
        currentTexture = textureCIdle;
        break;

    case PrincessState::FLOWER_ATTACK:
        currentAnim = &animsMagic; 
        currentTexture = textureMagic;
        break;

    case PrincessState::SPIKE_ATTACK:
        currentAnim = &animsMove;
        currentTexture = textureMove;
        break;

    case PrincessState::DEATH:
        currentAnim = &animsDeath;
        currentTexture = textureDeath;
        break;

    default:
        break;
    }

    if (currentAnim)
        currentAnim->Resets();
}

void PrincessBoss::SpawnFlower(Vector2D pos)
{
    auto flower = Engine::GetInstance()
        .entityManager
        ->CreateEntity(EntityType::FLOWERBOMB);

    flower->position = pos;
}

void PrincessBoss::SpawnSpike(Vector2D pos)
{
    auto spike = Engine::GetInstance()
        .entityManager
        ->CreateEntity(EntityType::SPIKEHAZARD);

    spike->position = pos;
}