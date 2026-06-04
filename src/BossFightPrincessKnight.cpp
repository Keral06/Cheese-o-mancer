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
// UPDATE
// ===============================

bool BossFightPrincessKnight::Update(float dt)
{
    // ===============================
    // DEBUG START
    // ===============================

    if (!fightStarted && debugStartFight)
    {
        /*if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_J) == KEY_DOWN)
        {
            StartFight();
        }*/
    }

    // ===============================
    // MAIN FSM
    // ===============================

    switch (fightState)
    {
    case BossFightState::IDLE:
        break;

    case BossFightState::INTRO:
    case BossFightState::KNIGHT_ENTRANCE:
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

    case BossFightState::FINISHED:
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
        // =========================
        // INTRO
        // =========================

    case BossFightState::INTRO:
    {
        if (!introStarted)
        {
            introStarted = true;

            knight->ResetActionFinished();

            knight->StartEntrance();

            LOG("Knight Entrance Started");
        }

        if (knight->HasFinishedAction())
        {
            knight->ResetActionFinished();

            SetFightState(BossFightState::KNIGHT_TRANSFORM);
        }

        break;
    }

    // =========================
    // KNIGHT TRANSFORM
    // =========================

    case BossFightState::KNIGHT_TRANSFORM:
    {
        if (!knight->IsBusy())
        {
            knight->ResetActionFinished();

            knight->StartTransform();

            LOG("Knight Transform Started");
        }

        if (knight->HasFinishedAction())
        {
            knight->ResetActionFinished();

            SetFightState(BossFightState::PRINCESS_TRANSFORM);
        }

        break;
    }

    // =========================
    // PRINCESS TRANSFORM
    // =========================

    case BossFightState::PRINCESS_TRANSFORM:
    {
        if (!princess->IsBusy())
        {
            princess->ResetActionFinished();

            princess->StartTransform();

            LOG("Princess Transform Started");
        }

        if (princess->HasFinishedAction())
        {
            princess->ResetActionFinished();

            introFinished = true;

            LOG("INTRO FINISHED");

            SetFightState(BossFightState::PHASE_1);

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



