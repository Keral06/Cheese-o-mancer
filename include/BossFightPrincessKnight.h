// ===============================
// BossFightPrincessKnight.h
// ===============================

#pragma once

#include "Module.h"
#include "Dialogue.h"
#include <Vector2D.h>

class PrincessBoss;
class KnightBoss;

enum class BossFightState
{
    IDLE,

    INTRO,
    KNIGHT_ENTRANCE,
    KNIGHT_DIAL_BEFORE_TRANSFORM,
    KNIGHT_TRANSFORM,
    PRINCESS_TRANSFORM,

    PHASE_1,
    PHASE_2,
    PHASE_3,

    DEATH,
    FINISHED
};

enum class BossTurn
{
    NONE,
    KNIGHT,
    PRINCESS
};

class BossFightPrincessKnight : public Module
{
public:

    BossFightPrincessKnight();
    virtual ~BossFightPrincessKnight();

    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    // ===============================
    // FIGHT FLOW
    // ===============================

    void StartFight();

    void StartKnightTurn();
    void StartPrincessTurn();

    void EndCurrentTurn();

    void NextPhase();

    void SetFightState(BossFightState newState);

    bool IsFightActive() const;


    // ===============================
    // REFERENCES
    // ===============================

    PrincessBoss* princess;
    KnightBoss* knight;

public:

    // ===============================
    // INTERNAL UPDATE
    // ===============================

    void UpdateIntro(float dt);
    void UpdatePhase(float dt);
    void UpdateDeath(float dt);


    

    // ===============================
    // STATES
    // ===============================

    BossFightState fightState;
    BossFightState lastFightState;

    BossTurn currentTurn;

    // ===============================
    // PHASES
    // ===============================

    int currentPhase;

    // ===============================
    // TIMERS
    // ===============================

    float stateTimer;
    float turnDelayTimer;

    // ===============================
    // FLAGS
    // ===============================

    bool fightStarted;
    bool introFinished;
    bool waitingNextTurn;

    // ===============================
    // DEBUG
    // ===============================

    bool debugStartFight;
    bool introStarted;

    Vector2D knightBasePos;
    Vector2D princessBasePos;

    bool introTriggered = false;
    float knightTargetX = 1800.0f;
    bool princessTranformed = false;

    bool waitingDecisionFinished = false;
    bool playerChoiceSpare = false;

    //DIALOGS

    Dialogue Before;
    Dialogue Before2;
    Dialogue Defeat;
    Dialogue DefeatWell;
    Dialogue AfterFightSpare;
    Dialogue AfterFightKill;
};