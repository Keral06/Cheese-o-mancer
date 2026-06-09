#include "HighPriestess.h"
#include "Engine.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Physics.h"
#include "Render.h"

HighPriestesss::HighPriestesss() : Enemy() {
    name = "HighPriestesss";
    currentWave = 1;
    enemiesAlive = 0;
    hitsTaken = 0;
    isVulnerable = false;
}

HighPriestesss::~HighPriestesss() {}

bool HighPriestesss::Start() {
    // 1. Cargar las 3 texturas por separado
    texIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/High Priestess/sprite_high_priestess_idle_01.png");
    texTurn = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/High Priestess/sprite_high_priestess_turn_01.png");
    texDeath = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/High Priestess/sprite_high_priestess_boss_01.png");

    // 2. Cargar los 3 archivos TSX independientes mapeando el alias correspondiente
    // Nota: Revisa en tus .tsx si la animación empieza en el tile 0 de cada archivo (suele ser lo habitual)
    std::unordered_map<int, std::string> aliasIdle = { {0, "idle"} };
    std::unordered_map<int, std::string> aliasTurn = { {0, "turn"} };
    std::unordered_map<int, std::string> aliasDeath = { {0, "death"} };

    animIdle.LoadFromTSX("assets/Textures/Spritesheets/High Priestess/high_priestess_idle.tsx", aliasIdle);
    animTurn.LoadFromTSX("assets/Textures/Spritesheets/High Priestess/high_priestess_turn.tsx", aliasTurn);
    animDeath.LoadFromTSX("assets/Textures/Spritesheets/High Priestess/priestess_death.tsx", aliasDeath);

    // 3. Configuración del estado inicial
    health = 3;
    state = EnemyState::IDLE;
    lastState = EnemyState::IDLE;

    currentAnimTrack = &animIdle;
    currentTexture = texIdle;
    animIdle.SetCurrent("idle");

    // 4. Inicializar la física centrándola en el tamaño de tu frame (ej: 128x128)
    pbody = Engine::GetInstance().physics->CreateRectangleFriction(position.getX(), position.getY(), 80, 128, bodyType::STATIC, 0.0f);
    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY;
    attackHitbox = nullptr;
    b2Body_SetGravityScale(pbody->body, 0.0f);
    //SpawnWave();
    return true;
}

bool HighPriestesss::Update(float dt) {
    // Si no hay enemigos y no hemos terminado, el boss se expone
    if (enemiesAlive <= 0 && currentWave <= 3 && !isVulnerable && hitsTaken < 3) {
        isVulnerable = true;
        SetState(EnemyState::WALKING); // Alias de vulnerable
    }

    // Cambiar de track de animación si ha cambiado el estado
    ChangeCurrentAnimation();

    // Actualizar los frames del track que esté activo en este frame
    if (currentAnimTrack != nullptr) {
        currentAnimTrack->Update(dt);
    }

    // Dibujar
    Draw(dt);

    return true;
}

void HighPriestesss::ChangeCurrentAnimation() {
    // Si el estado no ha cambiado, no hacemos nada (Optimización)
    //if (state == lastState) return;
    //lastState = state;

    //switch (state) {
    //case EnemyState::IDLE:
    //    currentAnimTrack = &animIdle;
    //    currentTexture = texIdle;
    //    animIdle.SetCurrent("idle");
    //    break;

    //case EnemyState::WALKING: // Vulnerable / Aturdida
    //    currentAnimTrack = &animTurn;
    //    currentTexture = texTurn;
    //    animTurn.SetCurrent("turn");
    //    break;

    //case EnemyState::DYING:
    //    currentAnimTrack = &animDeath;
    //    currentTexture = texDeath;
    //    animDeath.SetCurrent("death");
    //    break;
    //}
}

void HighPriestesss::Draw(float dt) {
    if (currentTexture == nullptr || currentAnimTrack == nullptr) return;

    // Obtener la caja de recorte (rect) del TSX activo
    const SDL_Rect& animFrame = currentAnimTrack->GetCurrentFrame();

    // Sincronizar posición con Box2D
    int x, y;
    pbody->GetPosition(x, y);
    position.setX((float)x);
    position.setY((float)y);

    // Centrar el dibujo en base al tamaño del frame actual
    int drawX = x - animFrame.w / 2;
    int drawY = y - animFrame.h / 2;

    SDL_FlipMode flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    Engine::GetInstance().render->DrawTexture(
        currentTexture, // Textura del estado actual
        drawX,
        drawY,
        &animFrame,     // Frame del estado actual
        1.0f,
        0.0,
        INT_MAX,
        INT_MAX,
        flip
    );
}

void HighPriestesss::OnCollision(PhysBody* physA, PhysBody* physB) {
    if (isVulnerable && physB->ctype == ColliderType::PLAYERATTACK) {
        hitsTaken++;
        isVulnerable = false;

        if (hitsTaken >= 3) {
            SetState(EnemyState::DYING);
            // Lógica de Die() si fuese necesario
        }
        else {
            currentWave++;
            SetState(EnemyState::IDLE);
            SpawnWave();
        }
    }
}

void HighPriestesss::SpawnWave() {
    int count = 2;
    enemiesAlive = count;
    for (int i = 0; i < count; ++i) {
        auto esbirro = Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
        if (esbirro) {
            esbirro->position.setX(position.getX() + (i == 0 ? -150.0f : 150.0f));
            esbirro->position.setY(position.getY());
            esbirro->Start();
        }
    }
}

void HighPriestesss::NotifyEnemyDeath() {
    enemiesAlive--;
}