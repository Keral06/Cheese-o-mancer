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

    

    spikeTexture = Engine::GetInstance().textures->Load(
        "assets/Textures/Spritesheets/Princess/ataques planta/Flower spike.png"
    );

    

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

    if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_N) == KEY_DOWN)
    {
        StartSpikeAttack();
    }
    if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
    {
        StartFlowerAttack(20);
    }

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
    stateTimer = 0.0f;

    SetPrincessState(PrincessState::FLOWER_ATTACK);

    LOG("Princess Flower Grid Attack");

    SpawnFlowerGrid();
}

void PrincessBoss::UpdateFlowerAttack(float dt)
{

    stateTimer += dt;

    float minY = -200.0f; 
    float maxY = 80.0f;

    Vector2D base = GetPosition();

    for (int i = flowersSpawned; i < flowersToSpawn; i++)
    {
        Vector2D pos;
        bool valid = false;

        
        for (int attempt = 0; attempt < 10; attempt++)
        {
            float offsetX = (rand() % 300) - 150;
            float offsetY = minY + (rand() / (float)RAND_MAX) * (maxY - minY);

            pos = Vector2D(base.getX() + offsetX, base.getY() + offsetY);

            if (IsFarEnough(pos))
            {
                valid = true;
                break;
            }
        }

        
        if (!valid)
        {
            float offsetX = (rand() % 3000) - 150;
            float offsetY = 80 - (rand() % 200);

            pos = Vector2D(base.getX() + offsetX, base.getY() + offsetY);
        }

        SpawnFlower(pos);
        spawnedFlowers.push_back(pos);
        flowersSpawned++;
    }

    // FIN ATAQUE
    if (stateTimer >= 2.0f)
    {
        busy = false;
        SetPrincessState(PrincessState::IDLE);

        actionFinished = true;

        flowersSpawned = 0;
        spawnedFlowers.clear();
    }
    

    
}

// ===============================
// SPIKE ATTACK
// ===============================

void PrincessBoss::StartSpikeAttack()
{
    busy = true;

    stateTimer = 0.0f;

    waveSpawnTimer = 0.0f;

    waveX = 0.0f;

    bordersSpawned = false;

    waveStarted = false;

    SetPrincessState(PrincessState::SPIKE_ATTACK);

    LOG("Princess Spike Attack");
}

void PrincessBoss::UpdateSpikeAttack(float dt)
{
    stateTimer += dt;

    Vector2D base = GetPosition();

    // =========================
    // 1. SPAWN BORDERS
    // =========================

    if (!bordersSpawned)
    {
        Vector2D leftPos(
            base.getX() + 100.0f,
            base.getY() + 128
        );

        Vector2D rightPos(
            base.getX() + 3000.0f,
            base.getY() + 128
        );

        leftBorderSpike = SpawnSpike(leftPos);
        rightBorderSpike = SpawnSpike(rightPos);

        if (leftBorderSpike)
            leftBorderSpike->SetBorderSpike(true);

        if (rightBorderSpike)
            rightBorderSpike->SetBorderSpike(true);

        waveX = leftPos.getX();

        bordersSpawned = true;
    }

    // =========================
    // 2. WAIT FOR RISING
    // =========================

    if (stateTimer < 0.8f)
        return;

    // =========================
    // 3. SPAWN WAVE
    // =========================

    waveSpawnTimer += dt;

    if (waveSpawnTimer >= 200.0f)
    {
        waveSpawnTimer = 0.0f;

        SpawnSpikeWave(waveX);

        waveX += 120.0f;
    }

    // =========================
    // 4. END CONDITION
    // =========================

    if (rightBorderSpike && waveX >= rightBorderSpike->position.getX())
    {
        if (leftBorderSpike)
            leftBorderSpike->Resume();

        if (rightBorderSpike)
            rightBorderSpike->Resume();

        busy = false;

        actionFinished = true;

        fightController->OnBossFinishedAttack(BossTurn::PRINCESS);

        SetPrincessState(PrincessState::IDLE);
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
    flower->Start();
}

std::shared_ptr<SpikeHazard> PrincessBoss::SpawnSpike(Vector2D pos)
{
    auto entity =
        Engine::GetInstance()
        .entityManager
        ->CreateEntity(EntityType::SPIKEHAZARD);

    auto spike =
        std::dynamic_pointer_cast<SpikeHazard>(entity);

    if (!spike)
    {
        LOG("ERROR: SpikeHazard cast failed");
        return nullptr;
    }

    spike->position = pos;
    spike->SetTexture(spikeTexture);
    
    spike->Start();

    return spike;
}

bool PrincessBoss::IsFarEnough(Vector2D pos)
{
    float minDist = 200.0f;

    for (auto& p : spawnedFlowers)
    {
        float dx = pos.getX() - p.getX();
        float dy = pos.getY() - p.getY();

        float distSq = dx * dx + dy * dy;

        if (distSq < minDist * minDist)
            return false;
    }

    return true;
}

void PrincessBoss::SpawnFlowerGrid()
{
    Vector2D base = GetPosition();

    float width = 3000.0f;   // ancho total del grid
    float height = 1500.0f;  // alto total del grid

    float cellW = width / (gridCols - 1);
    float cellH = height / (gridRows - 1);

    for (int r = 0; r < gridRows; r++)
    {
        for (int c = 0; c < gridCols; c++)
        {
            // patrón tipo ajedrez
            if ((r + c) % 2 == 0)
            {
                float x = base.getX() + 1000 - width * 0.5f + c * cellW;
                float y = base.getY() - height * 0.5f + r * cellH;

                SpawnFlower(Vector2D(x, y));
            }
        }
    }
}

void PrincessBoss::SpawnSpikeWave(float centerX)
{
    Vector2D base = GetPosition();

    std::vector<float> offsets =
    {
        120.0f,
        40.0f,
        120.0f
    };

    float spacing = 160.0f;

    for (int i = 0; i < (int)offsets.size(); i++)
    {
        float x = centerX + (i - 2) * spacing;
        float y = base.getY() + offsets[i];

        auto spike = SpawnSpike(Vector2D(x, y));

        // IMPORTANT: por si quieres diferenciar comportamiento futuro
        if (spike)
        {
            spike->SetBorderSpike(false);
        }
    }
}

void PrincessBoss::ReturnToBase(Vector2D pos)
{
    int x = (int)pos.getX();
    int y = (int)pos.getY();

    pbody->SetPosition(x, y);

    busy = false;
}

void PrincessBoss::FinishAction()
{
    actionFinished = true;
    busy = false;

    if (fightController)
        fightController->OnBossFinishedAttack(BossTurn::PRINCESS);
}