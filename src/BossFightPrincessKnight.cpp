// ===============================
// BossFightPrincessKnight.cpp
// ===============================

#include "BossFightPrincessKnight.h"

#include "Engine.h"
#include "Input.h"
#include "Scene.h"
#include "Log.h"
#include "EntityManager.h"
#include "PrincessBoss.h"
#include "KnightBoss.h"

// ===============================
// CONSTRUCTOR
// ===============================

BossFightPrincessKnight::BossFightPrincessKnight()
{
    princess = nullptr;
    knight = nullptr;

    fightState = BossFightState::IDLE;
    lastFightState = BossFightState::IDLE;

    currentTurn = BossTurn::NONE;

    currentPhase = 1;

    stateTimer = 0.0f;
    turnDelayTimer = 0.0f;

    fightStarted = false;
    introFinished = false;
    waitingNextTurn = false;

    debugStartFight = true;

    introStarted = false;
}

BossFightPrincessKnight::~BossFightPrincessKnight()
{
}

// ===============================
// START
// ===============================

bool BossFightPrincessKnight::Start()
{
    LOG("BossFightPrincessKnight START");

    // Limpiamos los punteros por seguridad. Se rellenarán solos cuando los bosses spawneen.
    princess = nullptr;
    knight = nullptr;

    return true;
}

// ===============================
// UPDATE MAIN FSM
// ===============================
bool BossFightPrincessKnight::Update(float dt)
{
    // Si no ha empezado la intro, medimos la distancia entre el Jugador y la Princesa
    if (fightState == BossFightState::IDLE && !introTriggered)
    {
        Player* player = Engine::GetInstance().scene->GetPlayer();

        // Nos aseguramos de que el jugador y la princesa ya estén enlazados en memoria
        if (player != nullptr && princess != nullptr)
        {
            Vector2D playerPos = player->position; // O el método que uses para su posición
            Vector2D princessPos = princess->GetPosition();

            /*LOG("DEBUG CONTROLLER | Player Ptr: %p | Pos X: %f | Princess X: %f",
                (void*)player,
                player->position.getX(),
                princess->GetPosition().getX());*/

            float distance = abs(princessPos.getX() - playerPos.getX());
            
            // Si el jugador se acerca a 10 píxeles (o unidades de tu mapa)
            if (distance <= 1350.0f)
            {
                LOG("Jugador cerca de la Princesa. Arrancando Intro...");
                introTriggered = true;

                // 1. Hacemos Zoom a la cámara (Ajusta el método según tu módulo de Render/Camera)
                Engine::GetInstance().render->SetZoomSmooth(0.5f, 800);
                LOG("CAMARA: Aplicando ZOOM a la escena.");

                // 2. Cambiamos al estado INTRO (esperando el diálogo)
                SetFightState(BossFightState::INTRO);
            }
        }
    }

    // ===============================
    // MAQUINA DE ESTADOS PRINCIPAL
    // ===============================
    switch (fightState)
    {
    case BossFightState::IDLE:
        break;

    case BossFightState::INTRO:
    case BossFightState::KNIGHT_ENTRANCE:
    case BossFightState::KNIGHT_DIAL_BEFORE_TRANSFORM:
    case BossFightState::KNIGHT_TRANSFORM:
    case BossFightState::PRINCESS_TRANSFORM:
        UpdateIntro(dt);
        break;

    case BossFightState::PHASE_1:
    case BossFightState::PHASE_2:
    case BossFightState::PHASE_3:
        UpdatePhase(dt);
        break;

    case BossFightState::DEATH:
        UpdateDeath(dt);
        break;
    }

    return true;
}

// ===============================
// CLEANUP
// ===============================

bool BossFightPrincessKnight::CleanUp()
{
    LOG("BossFightPrincessKnight CLEANUP");

    return true;
}

// ===============================
// START FIGHT
// ===============================

void BossFightPrincessKnight::StartFight()
{
    if (fightStarted)
        return;

    LOG("BOSS FIGHT START");

    knightBasePos = knight->GetPosition();
    princessBasePos = princess->GetPosition();

    fightStarted = true;

    currentPhase = 1;

    currentTurn = BossTurn::NONE;

    stateTimer = 0.0f;

    SetFightState(BossFightState::INTRO);
}

// ===============================
// INTRO UPDATE
// ===============================
void BossFightPrincessKnight::UpdateIntro(float dt)
{
    switch (fightState)
    {
        // ==========================================
        // PASO 1: ESPERANDO AL PRIMER DIÁLOGO
        // ==========================================
    case BossFightState::INTRO:
    {
        if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
        {
            LOG("Primer diálogo completado. ¡El Caballero SPAWNEA en medio!");

            if (knight != nullptr && princess != nullptr)
            {
                princessBasePos = princess->GetPosition();

                // Calculamos el medio entre la Princesa y el Jugador
                Player* player = Engine::GetInstance().scene->GetPlayer();
                float playerX = player ? player->position.getX() : princessBasePos.getX() - 300.0f;
                knightTargetX = (princessBasePos.getX() + playerX) * 0.5f;

                // Teletransportamos al Caballero directamente al centro exacto
                knightBasePos = Vector2D(knightTargetX, knight->GetPosition().getY());
                knight->ReturnToBase(knightBasePos);

                // Activamos la animación del Slide/Lunge inicial
                knight->SetKnightState(KnightState::ENTRANCE_DASH);
                knight->ResetActionFinished();

                // Quitamos el Zoom de la cámara suavemente
                Engine::GetInstance().render->SetZoomSmooth(0.3f, 800);

                // CAMBIO AQUÍ: En vez de ir a la transformación, pasamos al nuevo diálogo
                SetFightState(BossFightState::KNIGHT_ENTRANCE);
                LOG("BossFight State: Esperando al segundo diálogo pre-transformación...");
            }
        }
        break;
    }

    // ========================================================
    // PASO 2: EL CABALLERO LLEGA AL FINAL DEL SLIDE
    // ========================================================
    case BossFightState::KNIGHT_ENTRANCE:
    {
        // En cuanto el caballero llega al frame final del slide...
        if (knight->HasFinishedAction())
        {
            LOG("El Caballero ha frenado en el centro. Se queda estático en el slide.");

            knight->ResetActionFinished();

            
            
            SetFightState(BossFightState::KNIGHT_DIAL_BEFORE_TRANSFORM);
        }
        break;
    }

    // ========================================================
    // PASO 3: SEGUNDO DIÁLOGO (ESPERANDO EN LA POSE DEL SLIDE)
    // ========================================================
    case BossFightState::KNIGHT_DIAL_BEFORE_TRANSFORM:
    {
        
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
            {
                LOG("Segundo diálogo completado. ¡Empieza la transformación!");

                // Al pasar a KNIGHT_TRANSFORM, tu código ya llamará a knight->StartTransform()
                // lo que sacará al caballero del slide y cargará los sprites de mutación.
                SetFightState(BossFightState::KNIGHT_TRANSFORM);
            }
        
        break;
    }
    case BossFightState::KNIGHT_TRANSFORM:
    {
        if (knight->GetState() != KnightState::TRANSFORM)
        {
            knight->ResetActionFinished();
            knight->StartTransform();
            LOG("Intro: Caballero empieza a transformarse de manera segura.");
        }

        if (knight->HasFinishedAction())
        {
            LOG("Controller: Caballero transformado correctamente. Pasamos a la Princesa.");
            knight->ResetActionFinished();
            SetFightState(BossFightState::PRINCESS_TRANSFORM);
        }
        break;
    }
    // 4. TRANSFORMACIÓN DE LA PRINCESA Y COMIENZO DE LA PELEA
    case BossFightState::PRINCESS_TRANSFORM:
    {
        if (!princess->IsBusy())
        {
            princess->ResetActionFinished();
            princess->StartTransform();
            LOG("Intro: Princesa empieza a transformarse.");
        }

        if (princess->HasFinishedAction())
        {
            princess->ResetActionFinished();
            introFinished = true;

            LOG("--- INTRO FINALIZADA: EMPIEZA EL COMBATE REAL (FASE 1) ---");

            currentPhase = 1;
            SetFightState(BossFightState::PHASE_1);

            // El Caballero rompe el hielo y hace el primer turno de la pelea
            StartKnightTurn();
        }
        break;
    }
    }
}

// ===============================
// PHASE UPDATE (Comprobación de término de acción)
// ===============================
void BossFightPrincessKnight::UpdatePhase(float dt)
{
    // 1. Si estamos esperando el delay entre turnos
    if (waitingNextTurn)
    {
        turnDelayTimer -= dt;
        if (turnDelayTimer <= 0.0f)
        {
            waitingNextTurn = false;

            // Alternancia estricta
            if (currentTurn == BossTurn::KNIGHT) StartPrincessTurn();
            else StartKnightTurn();
        }
        return;
    }

    // 2. Monitorear si el boss actual ha terminado su ataque
    if (currentTurn == BossTurn::KNIGHT && knight->HasFinishedAction())
    {
        knight->ResetActionFinished();
        knight->ReturnToBase(knightBasePos);
        EndCurrentTurn(); // Activa el delay seguro antes de pasar al siguiente
    }
    else if (currentTurn == BossTurn::PRINCESS && princess->HasFinishedAction())
    {
        princess->ResetActionFinished();
        princess->ReturnToBase(princessBasePos);
        EndCurrentTurn();
    }

    // TODO: Aquí añadirías la lógica para cambiar de fase cuando la vida baje:
    // if (vida <= 0 && currentPhase < 3) { NextPhase(); }
    // else if (vida <= 0 && currentPhase == 3) { SetFightState(BossFightState::DEATH); }
}

// ===============================
// DEATH UPDATE
// ===============================

void BossFightPrincessKnight::UpdateDeath(float dt)
{
    // TODO:
    // Death cinematic
}

// ===============================
// START KNIGHT TURN
// ===============================
void BossFightPrincessKnight::StartKnightTurn()
{
    currentTurn = BossTurn::KNIGHT;
    knight->ResetActionFinished();

    // Filtro de ataques por fase
    if (currentPhase == 1 || currentPhase == 2)
    {
        // Fase 1 y 2: Solo Lunge. Pasamos multiplicador de velocidad (Fase 2 es más rápida)
        float speedMult = (currentPhase == 2) ? 1.5f : 1.0f;
        knight->StartLungeAttack(speedMult);
    }
    else if (currentPhase == 3)
    {
        // Fase 3: Todos los ataques aleatorios (Lunge a velocidad normal o Bounce)
        int r = rand() % 2;
        if (r == 0) knight->StartLungeAttack(1.0f);
        else knight->StartBounceAttack();
    }
}

// ===============================
// START PRINCESS TURN
// ===============================
void BossFightPrincessKnight::StartPrincessTurn()
{
    currentTurn = BossTurn::PRINCESS;
    princess->ResetActionFinished();

    if (currentPhase == 1 || currentPhase == 2)
    {
        // Fase 1 y 2: Solo Spike Attack
        princess->StartSpikeAttack();
    }
    else if (currentPhase == 3)
    {
        // Fase 3: Todos los ataques aleatorios
        int r = rand() % 2;
        if (r == 0) princess->StartSpikeAttack();
        else princess->StartFlowerAttack(20);
    }
}



// ===============================
// END CURRENT TURN
// ===============================

void BossFightPrincessKnight::EndCurrentTurn()
{
    waitingNextTurn = true;

    turnDelayTimer = 1.0f;

    LOG("TURN FINISHED");
}

// ===============================
// NEXT PHASE
// ===============================

void BossFightPrincessKnight::NextPhase()
{
    currentPhase++;

    LOG("NEXT PHASE: %d", currentPhase);

    switch (currentPhase)
    {
    case 2:
        SetFightState(BossFightState::PHASE_2);
        break;

    case 3:
        SetFightState(BossFightState::PHASE_3);
        break;

    default:
        SetFightState(BossFightState::DEATH);
        break;
    }
}

// ===============================
// SET STATE
// ===============================

void BossFightPrincessKnight::SetFightState(BossFightState newState)
{
    if (fightState == newState)
        return;

    lastFightState = fightState;
    fightState = newState;

    LOG("BossFight State: %d", (int)fightState);
}

// ===============================
// HELPERS
// ===============================

bool BossFightPrincessKnight::IsFightActive() const
{
    return fightStarted &&
        fightState != BossFightState::FINISHED;
}



