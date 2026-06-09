#include "Rey.h"
#include "Engine.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Physics.h"
#include "MagoBoss.h"
#include "Log.h"

Rey::Rey() : Enemy()
{
    name = "Rey";

    // INICIALIZAMOS LAS VARIABLES PARA EVITAR VALORES BASURA DE MEMORIA
    stateR = IDLER;
    lastStateR = DEATH_STATICR; // Lo ponemos distinto a IDLER para forzar que cargue la animación en el primer frame

    offsetX = 0.0f;
    offsetY = 0.0f;

    playerInRange = false;
    magoSpawned = false;
}

Rey::~Rey() {}

bool Rey::Start() { 
    
    texW = 650;
    texH = 650;

    std::unordered_map<int, std::string> aliasesAnim = {
            {0,"idle"},
            {1, "talking"},
            {28,"death_start"},
            {29, "death_static"}
    };

    anims.LoadFromTSX("assets/Textures/Spritesheets/King/king.tsx", aliasesAnim);
    texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/King/sprite_king_01.png");

    textureIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/Cmage_Idles_spritesheet.png");
    textureBall = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/Cmage_ball attack_spritesheet.png");
    textureTransformation = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/Cmage_transformation_spritesheet.png");

    pbody = Engine::GetInstance().physics->CreateRectangleSensor(position.getX(), position.getY(), texW, texH, bodyType::STATIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::REY;
    
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

    case TALKINGR:
        anims.SetCurrent("talking");
        offsetY = 0.0f;
        break;

    case DEADR:
        anims.SetCurrent("death_start");
        offsetY = 0.0f;
        break;

    case DEATH_STATICR:
        anims.SetCurrent("death_static");
        offsetY = 0.0f;
        break;

    default:
        break;
    }

}

bool Rey::Update(float dt)
{

    bool eKeyCurrent = (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN);

    // Si pulsamos la E, el jugador está cerca y NO la habíamos pulsado en el frame anterior
    if (playerInRange && eKeyCurrent && !eKeyWasPressed) {
        LOG("Has interactuado con el Rey (Tecla E)");

        if (stateR == IDLER) {
            stateR = TALKINGR; // Pasa a hablar
        }
        else if (stateR == TALKINGR) {
            Engine::GetInstance().render->SetZoomSmooth(0.5f, 800.0f);
            Die();
        }
    }

    eKeyWasPressed = eKeyCurrent;

    // Cuando termina la animación de muerte
    if (stateR == DEADR && anims.HasFinished() && !magoSpawned) {
        stateR = DEATH_STATICR;
        magoSpawned = true;

        // 1. Guardas lo que devuelve tu mánager (asumo que se llama así el tipo)
        std::shared_ptr<Entity> entityPtr = Engine::GetInstance().entityManager->CreateEntity(EntityType::MAGOBOSS);

        if (entityPtr) {
            // 2. Usamos .get() para sacar el puntero crudo y le hacemos el static_cast
            MagoBoss* newMago = static_cast<MagoBoss*>(entityPtr.get());

            newMago->position.setX(this->position.getX() + 2000);
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
        Engine::GetInstance().render->SetZoomSmooth(0.3f, 800.0f);
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

    if (animFrame.w == 0 || animFrame.h == 0) {
        LOG("CUIDADO: El fotograma de la animacion del Rey mide 0x0. Revisa el TSX y los IDs.");
    }

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
    // 2. SOLUCIÓN A LA INTERACCIÓN: Detectamos también los pies del jugador
    if (physB->ctype == ColliderType::PLAYER || physB->ctype == ColliderType::PLAYERFEET) {
        playerInRange = true;
        LOG("JUGADOR EN RANGO - Puedes pulsar la tecla E");
    }
}

void Rey::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
    // Detectamos cuando el jugador se aleja
    if (physB->ctype == ColliderType::PLAYER) {
        playerInRange = false;
    }
}