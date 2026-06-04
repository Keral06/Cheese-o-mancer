#include "KnightBoss.h"

#include "BossFightPrincessKnight.h"

#include "Engine.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Textures.h"

// =========================
// CONSTRUCTOR
// =========================

KnightBoss::KnightBoss() : Enemy()
{
    name = "KnightBoss";
}

KnightBoss::~KnightBoss()
{
}

// =========================
// START
// =========================

bool KnightBoss::Start()
{
    texW = 128 * 2;
    texH = 128 * 5;

    type = EnemyType::MELEE;

    stateTimer = 0.0f;

    attackSpeed = 12.0f;

    bounceCount = 0;

    busy = false;

    invulnerable = true;

    std::unordered_map<int, std::string> aliasesNIdle = { {0,"idle"}};
    std::unordered_map<int, std::string> aliasesNSlide = { {0,"slide"}};
    std::unordered_map<int, std::string> aliasesCIdle = { {0,"idle"}};
    std::unordered_map<int, std::string> aliasesCAttack1 = { {0,"attack1"} };
    std::unordered_map<int, std::string> aliasesCAttack2 = { {0,"attack2"} };
    std::unordered_map<int, std::string> aliasesCDefeat = { {0,"defeat"} };
    std::unordered_map<int, std::string> aliasesCTransform = { {0,"transform"}, {45, "walk"}};

    animsNIdle.LoadFromTSX("assets/Textures/Spritesheets/Knight/Normal/kn_idle.tsx", aliasesNIdle);
    animsNSlide.LoadFromTSX("assets/Textures/Spritesheets/Knight/Normal/kn_slide.tsx", aliasesNSlide);
    animsCIdle.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/kn_idle.tsx", aliasesCIdle);
    animsCAttack1.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/kc_attack1.tsx", aliasesCAttack1);
    animsCAttack2.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/kc_attack2.tsx", aliasesCAttack2);
    animsCDefeat.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/kc_defeat.tsx", aliasesCDefeat);
    animsCTransform.LoadFromTSX("assets/Textures/Spritesheets/Knight/Cheese/kc_transform_walk.tsx", aliasesCTransform);

    textureNIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Normal/KnightN_Idle.png");
    textureNSlide = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Normal/KnightN_sliding.png");
    textureCIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Idle.png");
    textureCAttack1 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Attack1.png");
    textureCAttack2 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Attack2.png");
    textureCDefeat = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Defeat.png");
    textureCTransform = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Knight/Cheese/KnightC_Transform_And_Walk.png");
    
    texture = textureNIdle;


    LOG("Knight Pos: %f %f", position.getX(), position.getY());
    pbody = Engine::GetInstance().physics->CreateRectangleFriction(position.getX(), position.getY(), texW, texH, bodyType::DYNAMIC, 0.0f);


    pbody->listener = this;


    pbody->ctype = ColliderType::ENEMY;


    pathfinding = std::make_shared<Pathfinding>();

    Vector2D pos = GetPosition();

    Vector2D tilePos = Engine::GetInstance().map->WorldToMap((int)pos.getX(), (int)pos.getY());

    pathfinding->ResetPath(tilePos);

    knightState = KnightState::IDLE;
    lastKnightState = KnightState::IDLE;

    actionFinished = false;
    attackHitbox = nullptr;

    currentAnim = &animsNIdle;
    currentTexture = textureNIdle;

    bounceStarted = false;

    BossFightPrincessKnight* controller = Engine::GetInstance().scene->GetBossFightController();

    if (controller != nullptr)
    {
        SetFightController(controller);
        controller->knight = this; // La princesa se registra a sí misma en el controlador
        LOG("Knight enlazada automáticamente al entrar a la sala.");
    }

    return true;
}

// =========================
// UPDATE
// =========================

bool KnightBoss::Update(float dt)
{
    stateTimer += dt;

    GetPhysicsValues();

    if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_L) == KEY_DOWN)
    {
        StartLungeAttack();
    }
    if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_K) == KEY_DOWN)
    {
        StartBounceAttack();
    }

    switch (knightState)
    {
    case KnightState::IDLE:
        break;

    case KnightState::ENTRANCE_DASH:
        UpdateEntrance(dt);
        break;

    case KnightState::TRANSFORM:
    {
        velocity = { 0,0 };

        if (currentAnim && currentAnim->HasFinished())
        {
            OnTransformFinished();
            FinishAction();
        }

        break;
    }

    case KnightState::LUNGE_ATTACK:
        UpdateLunge(dt);
        break;

    case KnightState::BOUNCE_ATTACK:
        UpdateBounce(dt);
        break;

    case KnightState::RECOVER:
        UpdateRecover(dt);
        break;

    case KnightState::DEATH:
        break;
    }

    UpdateFacing();

    ApplyPhysics();

    Draw(dt);

    return true;
}

// =========================
// ENTRANCE
// =========================

void KnightBoss::StartEntrance()
{
    busy = true;

    stateTimer = 0.0f;

    SetKnightState(KnightState::ENTRANCE_DASH);

    LOG("Knight Entrance");
}

void KnightBoss::UpdateEntrance(float dt)
{
    //velocity.x = -12.0f;
    //velocity.y = 0;

    //if (stateTimer >= 1.5f)
    //{
    //    velocity.x = 0;

    //    busy = false;

    //    SetKnightState(KnightState::IDLE);
    //}

    //if (stateTimer >= 1.5f)
    //{
    //    velocity.x = 0;
    //    busy = false;
    //    SetKnightState(KnightState::IDLE);
    //    FinishAction(); // Avisa al controlador para pasar a KNIGHT_TRANSFORM
    //}
    if (currentAnim && currentAnim->GetCurrentFrameIndex() == 18)
    {
        FinishAction(); 
    }
}

// =========================
// TRANSFORM
// =========================

void KnightBoss::StartTransform()
{
    busy = true;

    stateTimer = 0.0f;

    SetKnightState(KnightState::TRANSFORM);

    LOG("Knight Transform");
}

// =========================
// LUNGE
// =========================

void KnightBoss::StartLungeAttack(float speedMultiplier)
{
    if (!canAct) return;

    busy = true;

    stateTimer = 0.0f;

    attackSpeed = 15.0f * speedMultiplier;

    Vector2D player =
        Engine::GetInstance().scene->GetPlayerPosition();

    int ex, ey;
    pbody->GetPosition(ex, ey);

    attackDirection = player - Vector2D(ex, ey);

    float len =
        sqrtf(
            attackDirection.getX() * attackDirection.getX() +
            attackDirection.getY() * attackDirection.getY()
        );

    if (len > 0.0f)
    {
        attackDirection =
        {
            attackDirection.getX() / len,
            attackDirection.getY() / len
        };
    }

    SetKnightState(KnightState::LUNGE_ATTACK);

    LOG("Knight Lunge");
}

void KnightBoss::UpdateLunge(float dt)
{
    velocity.x = attackDirection.getX() * attackSpeed;
    velocity.y = attackDirection.getY() * attackSpeed;

    
    if (currentAnim && currentAnim->HasFinished())
    {
        velocity.x = 0;
        velocity.y = 0;

        busy = false;

        SetKnightState(KnightState::RECOVER);

        FinishAction();
    }
}

// =========================
// BOUNCE
// =========================

void KnightBoss::StartBounceAttack()
{
    if (!canAct) return;

    busy = true;

    stateTimer = 0.0f;
    bounceCount = 0;
    maxBounces = 6;

    SetKnightState(KnightState::BOUNCE_ATTACK);

    Vector2D player = Engine::GetInstance().scene->GetPlayerPosition();

    int x, y;
    pbody->GetPosition(x, y);

    bounceDir = Vector2D(0.5f, -1.0f);

    float len = sqrtf(
        bounceDir.getX() * bounceDir.getX() +
        bounceDir.getY() * bounceDir.getY()
    );

    if (len > 0.0f)
    {
        bounceDir = Vector2D(
            bounceDir.getX() / len,
            bounceDir.getY() / len
        );
    }

    bounceSpeed = 30.0f;

    LOG("Knight Bounce Attack");
}

void KnightBoss::UpdateBounce(float dt)
{
    int frame = currentAnim->GetCurrentFrameIndex();

    if (frame >= 10)
    {
        velocity.x = bounceDir.getX() * bounceSpeed;
        velocity.y = bounceDir.getY() * bounceSpeed;
    }
    else
    {
        velocity.x = 0;
        velocity.y = 0;
    }

    if (bounceCount >= maxBounces)
    {
        velocity.x = 0;
        velocity.y = 0;

        busy = false;

        SetKnightState(KnightState::RECOVER);

        /*FinishAction();*/
    }
}

// =========================
// RECOVER
// =========================

void KnightBoss::UpdateRecover(float dt)
{
    velocity.x = 0;
    velocity.y = 0;

    if (stateTimer >= 0.5f)
    {
        SetKnightState(KnightState::IDLE);
    }
}

// =========================
// HELPERS
// =========================

void KnightBoss::SetFightController(BossFightPrincessKnight* controller)
{
    fightController = controller;
}

bool KnightBoss::IsBusy() const
{
    return busy;
}

bool KnightBoss::IsInvulnerable() const
{
    return invulnerable;
}

// =========================
// STATE
// =========================

void KnightBoss::SetKnightState(KnightState newState)
{
    if (knightState == newState)
        return;

    lastKnightState = knightState;

    knightState = newState;

    stateTimer = 0.0f;

    ChangeCurrentAnimation();

    LOG("Knight State: %d", (int)knightState);
}

// =========================
// DRAW
// =========================

void KnightBoss::Draw(float dt)
{
    if (currentAnim == nullptr)
        return;


    double rotation = 0.0;

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
        SDL_FLIP_HORIZONTAL :SDL_FLIP_NONE ;

    if (knightState == KnightState::BOUNCE_ATTACK)
    {
        if (bounceDir.getY() > 0.0f)
        {
            rotation = 90.0f;
        }
    }

    if (knightState == KnightState::LUNGE_ATTACK)
    {
        if (velocity.x > 0) {
            flip = SDL_FLIP_NONE;
        }
    }

    Engine::GetInstance().render->DrawTexture(
        currentTexture,
        x - animFrame.w / 2,
        y - animFrame.h / 2,
        &animFrame,
        1.0f,
        rotation,
        INT_MAX,
        INT_MAX,
        flip
    );
}

void KnightBoss::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (knightState != KnightState::BOUNCE_ATTACK)
        return;

   
    bounceDir.setY(-bounceDir.getY());

    
    velocity.x = bounceDir.getX() * bounceSpeed;
    velocity.y = bounceDir.getY() * bounceSpeed;

    bounceCount++;
}

void KnightBoss::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
}

bool KnightBoss::HasFinishedAction() const
{
    return actionFinished;
}

void KnightBoss::ResetActionFinished()
{
    actionFinished = false;
}

void KnightBoss::ChangeCurrentAnimation()
{
    switch (knightState)
    {
    case KnightState::IDLE:
    {
        currentAnim = &animsNIdle;
        currentTexture = textureNIdle;

       
        break;
    }

    case KnightState::ENTRANCE_DASH:
    {
        currentAnim = &animsNSlide;
        currentTexture = textureNSlide;
        
        
        break;
    }

    case KnightState::TRANSFORM:
    {
        currentAnim = &animsCTransform;
        currentTexture = textureCTransform;

        
        break;
    }

    case KnightState::LUNGE_ATTACK:
    {
        currentAnim = &animsCAttack1;
        currentTexture = textureCAttack1;

        
        break;
    }

    case KnightState::BOUNCE_ATTACK:
    {
        currentAnim = &animsCAttack2;
        currentTexture = textureCAttack2;

        
        break;
    }

    case KnightState::RECOVER:
    {
        currentAnim = &animsCIdle;
        currentTexture = textureCIdle;

        
        break;
    }

    case KnightState::DEATH:
    {
        currentAnim = &animsCDefeat;
        currentTexture = textureCDefeat;

        
        break;
    }
    }

    if (currentAnim != nullptr)
    {
        currentAnim->Resets();
    }
}

void KnightBoss::UpdateFacing()
{
    Vector2D player = Engine::GetInstance().scene->GetPlayerPosition();

    int x, y;
    pbody->GetPosition(x, y);

    facingLeft = (player.getX() < x);
}

void KnightBoss::StartFight()
{
    busy = true;

    phase = PHASE_NONE;

    SetKnightState(KnightState::TRANSFORM);

    LOG("Boss fight start -> transforming");
}

void KnightBoss::OnTransformFinished()
{
    LOG("Transform finished -> Phase 1 start");

    SetKnightState(KnightState::IDLE);

    phase = PHASE_1;

    busy = false;

    canAct = true;
}

void KnightBoss::ReturnToBase(Vector2D pos)
{
    int x = (int)pos.getX();
    int y = (int)pos.getY();

    pbody->SetPosition(x, y);

    velocity = { 0,0 };

    busy = false;
}

void KnightBoss::StartIntroEntrance(float speedMultiplier)
{
    busy = true;
    actionFinished = false;
    stateTimer = 0.0f;

    // Forzamos el estado de la entrada deslizante
    SetKnightState(KnightState::ENTRANCE_DASH);

    // Ajustamos la velocidad de entrada basándonos en tu UpdateEntrance original multiplicada por el float
    velocity.x = -12.0f * speedMultiplier;
    velocity.y = 0.0f;

    LOG("Knight iniciando deslizamiento de la intro con animsNSlide");
}

void KnightBoss::FinishAction()
{
    actionFinished = true;
    busy = false;

}