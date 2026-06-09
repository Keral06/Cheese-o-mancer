#include "Rey.h"
#include "Engine.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Physics.h"
#include "MagoBoss.h"
Rey::Rey() : Enemy()
{
    name = "Rey";
}
Rey::~Rey() {}

bool Rey::Start() { 
    
    texW = 640;
    texH = 640;

    std::unordered_map<int, std::string> aliasesAnim = { {0,"idle"},{28,"death"} };
    anims.LoadFromTSX("assets/Textures/Spritesheets/King/kg_idle.tsx", aliasesAnim);
    texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/King/sprite_king_01.png");
    textureIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/Cmage_Idles_spritesheet.png");
    textureBall = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/Cmage_ball attack_spritesheet.png");
    textureTransformation = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/Cmage_transformation_spritesheet.png");
    
    //Add physics to the enemy - initialize physics body
    pbody = Engine::GetInstance().physics->CreateRectangleSensor(position.getX(), position.getY(), texW, texH, bodyType::STATIC);
    //Assign enemy class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
    pbody->listener = this;

    //ssign collider type
    pbody->ctype = ColliderType::REY;

    anims.SetCurrent("idle");
    attackHitbox = nullptr;
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
        anims.SetCurrent("death");
        offsetY = 0.0f;
        break;

    default:
        break;
    }

}

bool Rey::Update(float dt)
{
    // 1. Si el jugador está en rango y pulsa la tecla E
    if (playerInRange && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
        Engine::GetInstance().render->SetZoomSmooth(0.5f, 800.0f);
        Die(); // Recuerda que aquí dentro pones stateR = DEADR;
    }

    // Añadimos "!magoSpawned" a la condición
    if (stateR == DEADR && anims.HasFinished() && !magoSpawned) {
        magoSpawned = true;

        Engine::GetInstance().render->SetZoomSmooth(0.3f, 800.0f);

        // 1. Guardas lo que devuelve tu mánager (asumo que se llama así el tipo)
        std::shared_ptr<Entity> entityPtr = Engine::GetInstance().entityManager->CreateEntity(EntityType::MAGOBOSS);

        if (entityPtr) {
            // 2. Usamos .get() para sacar el puntero crudo y le hacemos el static_cast
            MagoBoss* newMago = static_cast<MagoBoss*>(entityPtr.get());

            newMago->position.setX(this->position.getX() + 400);
            newMago->position.setY(this->position.getY());

            // 3. Le pasamos las texturas
            newMago->SetTextures(
                this->textureIdle,
                this->textureBall,
                this->textureTransformation
            );

            // 4. Inicializamos
            newMago->Start();
        }

        //this->pendingToDelete = true;
    }

    // 2. Actualizar máquina de estados
    ChangeCurrentAnimation();

    // 3. Dibujar
    Draw(dt);

    return true;
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
    stateR = DEADR; // Cambia el estado para que dibuje la animación de muerte

   
    // Opcional: Marcar al Rey para ser destruido por el EntityManager si ya no se va a dibujar
    // this->pendingToDelete = true;
}

void Rey::OnCollision(PhysBody* physA, PhysBody* physB) {
    // Detectamos si el cuerpo que entra es el jugador
    if (physB->ctype == ColliderType::PLAYER) {
        playerInRange = true;
    }
}

void Rey::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
    // Detectamos cuando el jugador se aleja
    if (physB->ctype == ColliderType::PLAYER) {
        playerInRange = false;
    }
}