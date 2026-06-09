#include "CheeseBall.h"
#include "Engine.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Log.h"
#include "DestructibleEntity.h"
#include "Enemy.h"

CheeseBall::CheeseBall()
    : Entity(EntityType::CHEESEBALL)
{
    name = "CheeseBall";
}

CheeseBall::~CheeseBall()
{
}

bool CheeseBall::Awake()
{
    return true;
}

bool CheeseBall::Start()
{
    // Carga de texturas
    texIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/Cheese_wheel/Cheese_wheel.png");
    texBreak = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/Cheese_wheel/spritesheet.png");
    texExplode = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/Cheese_wheel/Cheese_break_explosion_spritesheet.png");

    // Configuración de animaciones (asumiendo que tus .tsx definen un solo clip llamado igual)
    std::unordered_map<int, std::string> aliasBreak = { {0, "break"} };
    std::unordered_map<int, std::string> aliasExplode = { {0, "explode"} };

    animBreak.LoadFromTSX("assets/Textures/Spritesheets/Jester/Cheese_wheel/cb_break.tsx", aliasBreak);
    animExplode.LoadFromTSX("assets/Textures/Spritesheets/Jester/Cheese_wheel/cb_explode.tsx", aliasExplode);

    // Estado inicial
    currentTexture = texIdle;
    currentAnimation = nullptr;

    pbody = Engine::GetInstance().physics->CreateCircle(position.getX(), position.getY(), radius, bodyType::DYNAMIC);

    pbody->listener = this;
    pbody->ctype = ColliderType::CHEESEBALL;

    LOG("CheeseBall created");

    return true;
}

bool CheeseBall::Update(float dt)
{
    if (toDelete) return true;

    // --- LÓGICA DE ESTADOS ---
    if (currentState == BallState::IDLE && !ismounted)
    {
        lifeTimer += dt;
        if (lifeTimer >= maxLifeTime)
        {
            currentState = BallState::BREAKING; // Se acabó el tiempo, rompemos
        }
    }
    else if (currentState == BallState::BREAKING)
    {
        // Si la animación terminó, eliminamos
        if (currentAnimation->HasFinished())
        {
            toDelete = true;
            return true;
        }
    }
    else if (currentState == BallState::EXPLODING)
    {
        // Si la animación terminó, eliminamos
        if (currentAnimation->HasFinished())
        {
            toDelete = true;
            return true;
        }
    }

    int x, y;
    pbody->GetPosition(x, y);

    position.setX((float)x);
    position.setY((float)y);
    float angle = pbody->GetRotation();
    float angleDeg = angle * 180.0f / 3.14f;

    if (currentState == BallState::BREAKING && currentAnimation != &animBreak) {
        currentAnimation = &animBreak;
        currentTexture = texBreak;
        currentAnimation->Resets(); // ¡Importante!
    }
    else if (currentState == BallState::EXPLODING && currentAnimation != &animExplode) {
        currentAnimation = &animExplode;
        currentTexture = texExplode;
        currentAnimation->Resets();
    }

    // Renderizado
    if (currentAnimation != nullptr && currentState == BallState::BREAKING) {
        // Obtenemos el rectángulo del frame actual de la animación
        SDL_Rect rect = currentAnimation->GetCurrentFrame();
        Engine::GetInstance().render->DrawTexture(
            currentTexture,
            x - radius,
            y - radius,
            &rect,
            1.0f,
            angle,
            INT_MAX,
            INT_MAX,
            SDL_FLIP_NONE
        );
        currentAnimation->Update(dt); // Avanza el frame
    }
    else if (currentAnimation != nullptr && currentState == BallState::EXPLODING) {
        SDL_Rect rect = currentAnimation->GetCurrentFrame();
        Engine::GetInstance().render->DrawTexture(
            currentTexture,
            x - radius,
            y - radius,
            &rect,
            1.0f,
            0.0f,
            INT_MAX,
            INT_MAX,
            SDL_FLIP_NONE
        );
        currentAnimation->Update(dt); // Avanza el frame
    
    }
    else {
        // Renderizamos la textura estática si estamos en IDLE
        Engine::GetInstance().render->DrawTexture(
            texIdle,
            x - radius,
            y - radius,
            nullptr,
            1.0f,
            angle,
            INT_MAX,
            INT_MAX,
            SDL_FLIP_NONE
        );
    }

   

    return true;
}

void CheeseBall::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype == ColliderType::MOHOWALL) {
        isOnMoho = true;
    }

    if (physB->ctype == ColliderType::WEAKWALL)
    {
        if (physB->listener != nullptr)
        {
            DestructibleEntity* destructible = dynamic_cast<DestructibleEntity*>(physB->listener);

            if (destructible && ismounted)
            {
                destructible->TakeDamage(1);
                if (!ismounted) {
                    toDelete = true;
                }
            }
        }
        return;
    }

    if (currentState != BallState::IDLE) return;

    
    if (physB->ctype == ColliderType::PLATFORM && ismounted)
    {
       
        firstjump = true;
        return;
    }
    if (physB->ctype == ColliderType::ENEMY)
    {
        Enemy* enemy = dynamic_cast<Enemy*>(physB->listener);

        if (enemy)
        {
            if (!enemy->isboss) {
                // Bola en smash
                if (canSmash)
                {
                    enemy->toDelete = true;
                    return;
                }

                // Bola lanzada
                if (!ismounted)
                {
                    currentState = BallState::EXPLODING;
                    currentAnimation = &animExplode;
                    currentTexture = texExplode;
                    currentAnimation->Resets();
                    SetVelocityy({0.0f,0.0f});
                    enemy->DecreaseHealth(50);
                    
                    return;
                }

                // Bola montada
                LOG("CheeseBall touched enemy");
                launch = true;
            }
        }

        return;
    }
}

void CheeseBall::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype == ColliderType::MOHOWALL) {
        isOnMoho = false;
    }
}

bool CheeseBall::CleanUp()
{
    LOG("Cleaning CheeseBall");

    if (pbody)
    {
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }

    Engine::GetInstance().textures->UnLoad(texIdle);
    Engine::GetInstance().textures->UnLoad(texBreak);
    Engine::GetInstance().textures->UnLoad(texExplode);

    return true;
}

void CheeseBall::SetPosition(const Vector2D& pos)
{
    position = pos;

    if (pbody)
    {
        pbody->SetPosition(pos.getX(), pos.getY());
    }
}

void CheeseBall::SetVelocityy(b2Vec2 vel)
{
    Engine::GetInstance().physics->SetLinearVelocity(pbody, vel);
}

void CheeseBall::StartLifespan()
{
    // Solo iniciamos si estamos en IDLE
    if (currentState == BallState::IDLE)
    {
        lifeTimer = 0.0f;
        // Aquí podrías aplicar una velocidad inicial si fuera necesario
    }
}


