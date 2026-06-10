#pragma once

#include "Enemy.h"
#include "Animation.h"

class HighPriestesss : public Enemy {
public:
    HighPriestesss();
    virtual ~HighPriestesss();

    bool Start() override;
    bool Update(float dt) override;
    void Draw(float dt);

    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void SpawnWave();
    void NotifyEnemyDeath();
    void ChangeCurrentAnimation();
    static HighPriestesss* instance;
private:
    // Control de oleadas y vulnerabilidad
    int currentWave;
    int enemiesAlive;
    int hitsTaken;
    bool isVulnerable;

    // --- 3 Animaciones y 3 Texturas Independientes ---
    AnimationSet animIdle;
    AnimationSet animTurn;
    AnimationSet animDeath;

    SDL_Texture* texIdle = nullptr;
    SDL_Texture* texTurn = nullptr;
    SDL_Texture* texDeath = nullptr;

    // Punteros de control para saber qué pintar en el frame actual
    AnimationSet* currentAnimTrack = nullptr;
    SDL_Texture* currentTexture = nullptr;
    EnemyState lastState; // Para optimizar el cambio de animación


    bool isImmobilized = false;
    bool waitingForChoice = false;
    std::string currentAnimName = "none";
     

    Dialogue Choosing;

    bool choiceMade = false;       
};