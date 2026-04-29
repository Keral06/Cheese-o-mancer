#include "CheeseBall.h"
#include "Engine.h"
#include "Physics.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Log.h"
#include "DestructibleEntity.h"

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
    texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Jester/Cheese_wheel/Cheese_wheel.png");

    pbody = Engine::GetInstance().physics->CreateCircle(position.getX(), position.getY(), radius, bodyType::DYNAMIC);

    pbody->listener = this;
    pbody->ctype = ColliderType::CHEESEBALL;

    LOG("CheeseBall created");

    return true;
}

bool CheeseBall::Update(float dt)
{
    if (toDelete) return true;

    int x, y;
    pbody->GetPosition(x, y);

    position.setX((float)x);
    position.setY((float)y);
    float angle = pbody->GetRotation();
    float angleDeg = angle * 180.0f / 3.14f;

    Engine::GetInstance().render->DrawTexture(
        texture,
        x - radius,
        y - radius,
        nullptr,
        1.0f,
        angle,  
        INT_MAX,
        INT_MAX,
        SDL_FLIP_NONE
    );

    return true;
}

void CheeseBall::OnCollision(PhysBody* physA, PhysBody* physB)
{
    if (physB->ctype == ColliderType::WEAKWALL)
    {
        if (physB->listener != nullptr)
        {
            DestructibleEntity* destructible = dynamic_cast<DestructibleEntity*>(physB->listener);

            if (destructible)
            {
                destructible->TakeDamage(1);
                if (!ismounted) {
                    toDelete = true;
                }
            }
        }
    }

    else if ((physB->ctype == ColliderType::PLATFORM || physB->ctype == ColliderType::PARED) && !ismounted)
    {
        LOG("CheeseBall touched platform/wall");
        toDelete = true;
    }
}

void CheeseBall::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
}

bool CheeseBall::CleanUp()
{
    LOG("Cleaning CheeseBall");

    if (pbody)
    {
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }

    Engine::GetInstance().textures->UnLoad(texture);

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


