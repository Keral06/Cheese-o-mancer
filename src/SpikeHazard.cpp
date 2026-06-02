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
    state = State::RISING;

    std::unordered_map<int, std::string> aliases =
    {
        {0, "flowerspike1"}, {34, "flowerspike2"}
    };

    anim.LoadFromTSX(
        "assets/Textures/Spritesheets/Princess/ataques planta/pt_flowerspike.tsx",
        aliases
    );

    pbody = Engine::GetInstance().physics->CreateRectangle(
        position.getX(),
        position.getY() + 128 * 3,
        128,
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
    

    // =========================
    // BORDER SPIKE PAUSE
    // =========================

    if (borderSpike && !pausedAtTop)
    {
        int frame = anim.GetCurrentFrameIndex();

        // frame pico
        if (frame >= 5)
        {
            pausedAtTop = true;
        }
    }

    // SOLO actualiza si NO está pausado
    if (!pausedAtTop)
    {
        anim.Update(dt);
    }

    // delete normal
    if (anim.HasFinished())
    {
        toDelete = true;
    }

    Draw(dt);

    return true;
}

void SpikeHazard::SetBorderSpike(bool v)
{
    borderSpike = v;
}

void SpikeHazard::Draw(float dt)
{
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
        y - frame.h,
        &frame
    );
}

bool SpikeHazard::CleanUp()
{
    if (pbody != nullptr) {
        pbody->listener = nullptr;
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }

    return true;
}

void SpikeHazard::Resume()
{
    pausedAtTop = false;
}

void SpikeHazard::SetTexture(SDL_Texture* tex)
{
    texture = tex;

    if (borderSpike) {
        anim.SetCurrent("flowerspike2");
    }
}
