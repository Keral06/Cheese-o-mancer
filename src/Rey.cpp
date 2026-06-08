#include "Rey.h"
#include "Engine.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Physics.h"

Rey::Rey() : Enemy()
{
    name = "Verdugo";
}
Rey::~Rey() {}

bool Rey::Start() { 
    
    texW = 128;
    texH = 128;

    std::unordered_map<int, std::string> aliasesAnim = { {0,"idle"},{10,"dead"} };
    anims.LoadFromTSX("assets/Textures/Spritesheets/King/king.tsx", aliasesAnim);
    texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/King/sprite_king_01.png");

    
    //Add physics to the enemy - initialize physics body
    pbody = Engine::GetInstance().physics->CreateRectangleSensor(position.getX(), position.getY(), texW, texH, bodyType::STATIC);
    //Assign enemy class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
    pbody->listener = this;

    //ssign collider type
    pbody->ctype = ColliderType::ENEMY;
    return true; 


}

void Rey::ChangeCurrentAnimation() {

    if (stateR == lastStateR) return;

    lastStateR = stateR;

    switch (stateR)
    {
    case IDLER:
        anims.SetCurrent("idle");
        offsetY = 0.0f;
        break;

    case DEADR:
        anims.SetCurrent("dead");
        offsetY = 0.0f;
        break;

    default:
        break;
    }

}

bool Rey::Update(float dt)
{

    Draw(dt);
    return false;
}

void Rey::Draw(float dt)
{
    anims.Update(dt);
    const SDL_Rect& animFrame = anims.GetCurrentFrame();

    //// Update render position using your PhysBody helper
    int x, y;
    pbody->GetPosition(x, y);
    position.setX((float)x);
    position.setY((float)y);



    SDL_FlipMode flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    int drawX = x - animFrame.w / 2 - offsetX;
    int drawY = y - animFrame.h / 2 - offsetY;

    Engine::GetInstance().render->DrawTexture(
        texture,
        drawX,
        drawY,
        &animFrame,
        1.0f,
        0.0,
        INT_MAX,
        INT_MAX,
        flip
    );
}

void Rey::Die() {
    // 1. Aquí va tu lógica de animación de muerte

    // 2. Spawneamos al Mago directamente desde aquí
    // Usamos el EntityManager para crear la nueva entidad
    auto newMago = Engine::GetInstance().entityManager->CreateEntity(EntityType::MAGOBOSS);

    // Posicionamos al mago donde estaba el rey o en un punto predefinido
    if (newMago) {
        newMago->position.setX(this->position.getX());
        newMago->position.setY(this->position.getY());
        newMago->Start(); // Importante llamar a Start() para que inicialice su física
    }
}