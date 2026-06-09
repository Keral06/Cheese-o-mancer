#include "HighPriestess.h"
#include "Engine.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Physics.h"
#include "Render.h"
#include "Player.h"
#include "Scene.h"

HighPriestesss* HighPriestesss::instance = nullptr;

HighPriestesss::HighPriestesss() : Enemy() {
    name = "HighPriestesss";
    currentWave = 1;
    enemiesAlive = 0;
    hitsTaken = 0;
    isVulnerable = false;
}

HighPriestesss::~HighPriestesss() {}

bool HighPriestesss::Start() {

    instance = this;
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

    // 1. Cargar el spritesheet
    texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/High Priestess/sprite_high_priestess_boss_01.png");

    // 2. Definir las animaciones
    std::unordered_map<int, std::string> aliases = {
        {0, "inmobilization_start"},
        {10, "inmobilization_idle"},
        {30, "spare"},
        {40, "death"},
        {41, "death_static"}
    };

    anims.LoadFromTSX("assets/Textures/Spritesheets/High Priestess/high_priestess_boss.tsx", aliases);

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
    SpawnWave();
    return true;
}

bool HighPriestesss::Update(float dt) {
    // Si no hay enemigos vivos, la jefa no es vulnerable todavía y no ha muerto...
    if (enemiesAlive <= 0 && !isVulnerable && state != EnemyState::DYING) {
        // Solo se expone si realmente la oleada actual se ha limpiado de verdad
        isVulnerable = true;
        SetState(EnemyState::WALKING); // Pasa a estado Vulnerable (Animación Turn)
    }

    // --- TRANSICIONES DE FASE FINAL (Tu lógica de cinemática/elección) ---
    if (currentAnimName == "inmobilization_start" && anims.HasFinished()) {
        anims.SetCurrent("inmobilization_idle");
        currentAnimName = "inmobilization_idle";
        waitingForChoice = true;
    }
    else if (currentAnimName == "spare" && anims.HasFinished()) {
        Player* player = Engine::GetInstance().scene->GetPlayer();
        if (player != nullptr) player->isDeadDefinitive = true;
    }
    else if (currentAnimName == "death" && anims.HasFinished()) {
        anims.SetCurrent("death_static");
        currentAnimName = "death_static";
        if (instance == this) {
            instance = nullptr;
        }
    }

    // Actualizar máquinas de estado y tracks
    ChangeCurrentAnimation();

    if (currentAnimTrack != nullptr) {
        currentAnimTrack->Update(dt);
    }

    Draw(dt);

    return true;
}

void HighPriestesss::ChangeCurrentAnimation() {
    if (state == lastState) return;
    lastState = state;

    switch (state) {
    case EnemyState::IDLE:
        currentAnimTrack = &animIdle;
        currentTexture = texIdle;
        animIdle.SetCurrent("idle");
        break;

    case EnemyState::WALKING: // Vulnerable / Aturdida
        currentAnimTrack = &animTurn;
        currentTexture = texTurn;
        animTurn.SetCurrent("turn");
        break;

    case EnemyState::DYING:
        currentAnimTrack = &animDeath;
        currentTexture = texDeath;
        animDeath.SetCurrent("death");
        break;
    }
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
    // IMPORTANTE: Comprobamos que sea vulnerable para procesar el golpe
    if (isVulnerable && physB->ctype == ColliderType::PLAYERATTACK) {

        isVulnerable = false; // Desactivar vulnerabilidad INSTANTÁNEAMENTE para evitar doble golpe
        hitsTaken++;

        if (hitsTaken >= 3) {
            SetState(EnemyState::DYING);
            // Aquí puedes iniciar tu animación de "inmobilization_start" si es lo que deseas
            currentAnimName = "inmobilization_start";
        }
        else {
            currentWave++; // Cambiamos a la siguiente oleada (Wave 2 o 3)
            SetState(EnemyState::IDLE);

            // Dejar un pequeño margen o llamar directamente al spawner
            SpawnWave();
        }
    }
}

void HighPriestesss::SpawnWave() {
    // Definimos cuántos enemigos por oleada (según tu especificación, siempre son 3)
    int count = 3;
    enemiesAlive = count;
    
    // Determinamos el tipo de enemigo según la oleada actual
    EntityType typeToSpawn;
    switch (currentWave) {
    case 1:
        typeToSpawn = EntityType::RAT; // Reemplaza por tu enum real de Rata
        break;
    case 2:
        typeToSpawn = EntityType::JAILER; // Reemplaza por tu enum real de Jailer
        break;
    case 3:
        typeToSpawn = EntityType::HORSE; // Reemplaza por tu enum real de Abeja
        break;
    default:
        typeToSpawn = EntityType::ENEMY; // Por si acaso
        break;
    }
    printf("Spawneando oleada: %d, Tipo de entidad: %d\n", currentWave, (int)typeToSpawn);
    // Spawneamos los 3 enemigos en posiciones distribuidas
    for (int i = 0; i < count; ++i) {
        auto esbirro = Engine::GetInstance().entityManager->CreateEntity(typeToSpawn);
        if (esbirro != nullptr) {
            // Posicionamiento: uno a la izquierda (-200), uno en el centro (0), uno a la derecha (+200)
            float offsetX = (i - 1) * 200.0f;
            esbirro->position.setX(position.getX() + offsetX);
            esbirro->position.setY(position.getY() + 2000);

            // ¡IMPORTANTE! Si tu EntityManager ya llama a Start() internamente al crear la entidad,
            // llamar aquí a esbirro->Start() de forma manual puede causar un doble Start() y romper el juego.
            // Si ves que vuelven los crashes, prueba a comentar la línea de abajo:
            esbirro->Start();
        }
    }
}

void HighPriestesss::NotifyEnemyDeath() {
    enemiesAlive--;
}