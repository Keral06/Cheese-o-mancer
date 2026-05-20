// ==========================================
// FlowerBomb.cpp
// ==========================================

#include "FlowerBomb.h"

#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Physics.h"
#include "Log.h"

FlowerBomb::FlowerBomb()
{
    name = "FlowerBomb";

    type = EntityType::FLOWERBOMB;
}

FlowerBomb::~FlowerBomb()
{
}

bool FlowerBomb::Start()
{
    timer = 0.0f;

    explodeDelay = 2.5f;

    state = State::IDLE;

    std::unordered_map<int, std::string> aliasesIdle =
    {
        {0, "idle"}
    };

    std::unordered_map<int, std::string> aliasesExplosion =
    {
        {0, "explode"}
    };

    anim.LoadFromTSX(
        "assets/Textures/Spritesheets/Princess/ataques planta/pt_flowerbomb.tsx",
        aliasesIdle
    );


    texture = Engine::GetInstance().textures->Load(
        "assets/Textures/Spritesheets/Princess/ataques planta/Flowe bomb.png"
    );

   

    pbody = Engine::GetInstance().physics->CreateCircle(
        position.getX(),
        position.getY(),
        32,
        bodyType::STATIC
    );

    if (pbody)
    {
        pbody->listener = this;
        pbody->ctype = ColliderType::DANGER;
    }

    return true;
}

bool FlowerBomb::Update(float dt)
{
    timer += dt;

    if (state == State::IDLE)
    {
        if (timer >= explodeDelay)
        {
            Explode();
        }
    }
    else if (state == State::EXPLODING)
    {
        if (anim.HasFinished())
        {
            toDelete = true;
        }
    }

    Draw(dt);

    return true;
}

void FlowerBomb::Explode()
{
    LOG("FlowerBomb explode");

    state = State::EXPLODING;

    timer = 0.0f;

    
    anim.Resets();

    // TODO:
    // Spawn explosion collider
    // Damage player
    // Particles
    // Screenshake
}

void FlowerBomb::Draw(float dt)
{

    anim.Update(dt);

    const SDL_Rect& frame =
        anim.GetCurrentFrame();

    int x = (int)position.getX();
    int y = (int)position.getY();

    if (pbody)
    {
        pbody->GetPosition(x, y);

        position.setX((float)x);
        position.setY((float)y);
    }

    Engine::GetInstance().render->DrawTexture(
        texture,
        x - frame.w / 2,
        y - frame.h / 2,
        &frame
    );
}

bool FlowerBomb::CleanUp()
{
    if (pbody != nullptr) {
        pbody->listener = nullptr;
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }

    Engine::GetInstance().textures->UnLoad(texture);

    return true;
}

