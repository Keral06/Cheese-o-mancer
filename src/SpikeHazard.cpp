// ==========================================
// SpikeHazard.cpp
// ==========================================

#include "SpikeHazard.h"

#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Physics.h"
#include "Log.h"

SpikeHazard::SpikeHazard()
{
    name = "SpikeHazard";

    type = EntityType::SPIKEHAZARD;
}

SpikeHazard::~SpikeHazard()
{
}

bool SpikeHazard::Start()
{
    timer = 0.0f;

    warningDuration = 0.8f;
    activeDuration = 1.0f;

    state = State::WARNING;

    std::unordered_map<int, std::string> aliasesWarning =
    {
        {0, "warning"}
    };

    std::unordered_map<int, std::string> aliasesActive =
    {
        {0, "active"}
    };

    animWarning.LoadFromTSX(
        "assets/Textures/Spritesheets/Princess/spike_warning.tsx",
        aliasesWarning
    );

    animActive.LoadFromTSX(
        "assets/Textures/Spritesheets/Princess/spike_active.tsx",
        aliasesActive
    );

    texture = Engine::GetInstance().textures->Load(
        "assets/Textures/Spritesheets/Princess/SpikeHazard.png"
    );

    currentAnim = &animWarning;

    pbody = Engine::GetInstance().physics->CreateRectangle(
        position.getX(),
        position.getY(),
        64,
        128,
        bodyType::STATIC
    );

    if (pbody)
    {
        pbody->listener = this;
        pbody->ctype = ColliderType::DANGER;
    }

    return true;
}

bool SpikeHazard::Update(float dt)
{
    timer += dt;

    if (state == State::WARNING)
    {
        if (timer >= warningDuration)
        {
            Activate();
        }
    }
    else if (state == State::ACTIVE)
    {
        if (timer >= activeDuration)
        {
            toDelete = true;
        }
    }

    Draw(dt);

    return true;
}

void SpikeHazard::Activate()
{
    LOG("Spike activate");

    state = State::ACTIVE;

    timer = 0.0f;

    currentAnim = &animActive;
    currentAnim->Resets();

    if (pbody)
    {
        pbody->ctype = ColliderType::DANGER;
    }
}

void SpikeHazard::Draw(float dt)
{
    if (currentAnim == nullptr)
        return;

    currentAnim->Update(dt);

    const SDL_Rect& frame =
        currentAnim->GetCurrentFrame();

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
        y - frame.h,
        &frame
    );
}

bool SpikeHazard::CleanUp()
{
    return true;
}