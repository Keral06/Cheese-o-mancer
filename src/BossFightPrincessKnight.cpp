// ===============================
// BossFightPrincessKnight.cpp
// ===============================

#include "BossFightPrincessKnight.h"

#include "Engine.h"
#include "Input.h"
#include "Scene.h"
#include "Textures.h"
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

    Dialogue Before("assets/Dialogues/The_Lovers_Bossfight/Lovers_Before_Bossfight1_Dialogues.txt", "assets/Dialogues/The_Lovers_Bossfight/Lovers_Before_Bossfight1_Names.txt");
    this->Before = Before;
    Dialogue Before2("assets/Dialogues/The_Lovers_Bossfight/Lovers_Before_Bossfight2_Dialogues.txt", "assets/Dialogues/The_Lovers_Bossfight/Lovers_Before_Bossfight2_Names.txt");
    this->Before2 = Before2;

    Dialogue Defeat("assets/Dialogues/The_Lovers_Bossfight/Lovers_Death_Dialogues.txt", "assets/Dialogues/The_Lovers_Bossfight/Lovers_Death_Names.txt");
    this->Defeat = Defeat;
    Dialogue DefeatWell("assets/Dialogues/The_Lovers_Bossfight/Lovers_Choice_Dialogues.txt", "assets/Dialogues/The_Lovers_Bossfight/Lovers_Choice_Names.txt");
    if (Engine::GetInstance().scene->cards.tieneObjeto("Sun") == false) {

        Engine::GetInstance().scene->cards.push("Sun", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Sun.png"), Engine::GetInstance().textures->Load("assets/UI/Tarot/Inverted/UI_TarotCard_Sun_inverted.png"));


    }

    if (Engine::GetInstance().scene->cards.tieneObjeto("Moon") == false) {

        Engine::GetInstance().scene->cards.push("Moon", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Moon.png"), Engine::GetInstance().textures->Load("assets/UI/Tarot/Inverted/UI_TarotCard_Moon_inverted.png"));


    }
    this->DefeatWell = DefeatWell;
    /*Dialogue AfterFightSpare("assets/Dialogues/The_Lovers_Bossfight/Lovers_Before_Bossfight1_Dialogues", "assets/Dialogues/The_Lovers_Bossfight/Lovers_Before_Bossfight1_Names");
    this->AfterFightSpare = AfterFightSpare;
    Dialogue AfterFightKill("assets/Dialogues/The_Lovers_Bossfight/Lovers_Before_Bossfight1_Dialogues", "assets/Dialogues/The_Lovers_Bossfight/Lovers_Before_Bossfight1_Names");
    this->AfterFightKill = AfterFightKill;*/
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
                Engine::GetInstance().render->SetZoomSmooth(0.5f, 800);
                LOG("CAMARA: Aplicando ZOOM a la escena.");
                LOG("Jugador cerca de la Princesa. Arrancando Intro...");
                if (introTriggered == false) {
                    if (Before.hasStarted == false) { Before.AvanzarDialogo(dt, nameNPC); return true; }
                    if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

                        if (Before.AvanzarDialogo(dt, nameNPC)) {

                            introTriggered = true;
                            SetFightState(BossFightState::INTRO);
                            return true;
                        }



                    }
                    if (Before.hasStarted && !Before.hasEnded) {

                        Before.Draw(dt);
                        return true;


                    }

                }
                // 1. Hacemos Zoom a la cámara (Ajusta el método según tu módulo de Render/Camera)


                // 2. Cambiamos al estado INTRO (esperando el diálogo)

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
        if (Before.hasEnded)
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


                // Activamos la animación del Slide/Lunge inicial


                // Quitamos el Zoom de la cámara suavemente
                Engine::GetInstance().render->SetZoomSmooth(0.3f, 800);

                // CAMBIO AQUÍ: En vez de ir a la transformación, pasamos al nuevo diálogo
                if (Before2.hasStarted == false) {
                    Before2.AvanzarDialogo(dt, nameNPC);  knightBasePos = Vector2D(knightTargetX, knight->GetPosition().getY());
                    knight->ReturnToBase(knightBasePos); knight->SetKnightState(KnightState::ENTRANCE_DASH);
                    knight->ResetActionFinished(); return;
                }

                if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

                    if (Before2.AvanzarDialogo(dt, nameNPC)) {
                        SetFightState(BossFightState::KNIGHT_ENTRANCE);
                    }

                }

                if (Before2.hasStarted && !Before2.hasEnded) {
                    Before2.Draw(dt);
                    return;

                }

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


        LOG("Segundo diálogo completado. ¡Empieza la transformación!");

        // Al pasar a KNIGHT_TRANSFORM, tu código ya llamará a knight->StartTransform()
        // lo que sacará al caballero del slide y cargará los sprites de mutación.
        SetFightState(BossFightState::KNIGHT_TRANSFORM);


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
        //LOG("%d", princess->IsBusy());

        if (!princessTranformed)
        {
            princess->ResetActionFinished();
            princess->StartTransform();
            LOG("Intro: Princesa empieza a transformarse.");
            princessTranformed = true;
        }

        if (princess->HasFinishedAction())
        {
            princess->currentAnim->Resets();
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
        //princess->ReturnToBase(princessBasePos);
        EndCurrentTurn();
    }

}

// ===============================
// DEATH UPDATE
// ===============================

void BossFightPrincessKnight::UpdateDeath(float dt)
{
    // Detenemos a los jefes si estaban haciendo algo en Box2D
    if (knight != nullptr)  knight->toDelete = true;
    if (princess != nullptr) princess->ResetActionFinished();

    // Dependiendo de tu input, usaremos teclas provisionales para la decisión:
    // [Teclas ejemplo: 'Y' para Spare, 'X' para Kill]

    // 1. ESPERANDO ELECCIÓN DEL JUGADOR
    if (!waitingDecisionFinished)
    {
        //Primero mira las condiciones para ver el spare o no

        if (Engine::GetInstance().scene->hasShownPoemToWell == true) {

            if (DefeatWell.hasStarted == false) { DefeatWell.AvanzarDialogo(dt, nameNPC); return; }

            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


                if (DefeatWell.AvanzarDialogo(dt, nameNPC)) {



                    desicion = DefeatWell.WhatChoice();
                    waitingDecisionFinished = true;



                }



            }
            if (DefeatWell.hasStarted && !DefeatWell.hasEnded) {
                DefeatWell.Draw(dt);
                return;

            }




        }
        else {



            if (Defeat.hasStarted == false) { Defeat.AvanzarDialogo(dt, nameNPC); return; }

            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {


                if (Defeat.AvanzarDialogo(dt, nameNPC)) {



                    desicion = false;
                    waitingDecisionFinished = true;



                }



            }

            if (Defeat.hasStarted && !Defeat.hasEnded) {
                Defeat.Draw(dt);
                return;

            }


        }



        if (desicion)
        {
            LOG("Elección: ¡Has decidido PERDONAR a los Lovers!");
            waitingDecisionFinished = true;
            playerChoiceSpare = true;
            if (!Engine::GetInstance().scene->cards.tieneObjeto("Lovers")) {


                Engine::GetInstance().scene->cards.push("Lovers", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Lovers.png"), nullptr);

            }
            Engine::GetInstance().scene->hasSparedPrincessAndKnight = true;
            // Los jefes se quedan en DEFEAT de forma permanente
            // Aquí puedes disparar tu diálogo "AfterFightSpare"
        }

        // OPCIÓN B: MATAR (KILL)
        if (!desicion)
        {
            LOG("Elección: ¡Has decidido ASESINAR a los Lovers!");
            Engine::GetInstance().scene->cards.GirarCarta("Moon");
            Engine::GetInstance().scene->cards.GirarCarta("Sun");

            waitingDecisionFinished = true;
            playerChoiceSpare = false;

            // Cambiamos a la Princesa a su animación real de muerte definitiva
            if (princess != nullptr)
            {
                princess->SetPrincessState(PrincessState::DEATH);
            }


            // Aquí puedes disparar tu diálogo "AfterFightKill"
        }
    }
    else
    {
        // 2. LA DECISIÓN YA FUE TOMADA
        if (playerChoiceSpare)
        {
            // Lógica final tras perdonar (ej: abrir puertas, dar recompensa pacífica)
            // Se quedan en animación DEFEAT permanentemente.
            Engine::GetInstance().scene->hasSparedPrincessAndKnight = true;
        }
        else
        {
            Engine::GetInstance().scene->hasSparedPrincessAndKnight = false;
            // Lógica final tras matar (esperar a que la animación de DEATH termine)
            if (princess != nullptr && princess->currentAnim->HasFinished())
            {
                LOG("Animación de muerte completada. Finalizando combate.");
                SetFightState(BossFightState::FINISHED);

            }
        }
    }
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




