#pragma once

#include "Enemy.h" // Asumo que hereda de Enemy igual que Rey
#include "Animation.h" // Estructura de animaciones que usas
#include <unordered_map>
#include <string>

// Fases generales del combate
enum class BossPhaseM {
    INTROM,
    TRANSFORMATIONM,
    BOSSFIGHTM
};

// Estados específicos de acción y animación del Mago
enum MagoState {
    // Animaciones de Transformación (Fases 1 y 2)
    MAGO_NORMAL_IDLE,
    MAGO_WEAK_START,
    MAGO_WEAK_LOOP,
    MAGO_TRANSFORMATION_FX,

    // Animaciones de Combate / Vuelo (Fase 3 - Reposo y Magia)
    MAGO_FLY_IDLE,
    MAGO_FLY_WEAK,
    MAGO_FLY_MAGIC,

    // Animaciones del Ataque de la Bola (Fase 3 - Ataque 2)
    MAGO_BALL_START,
    MAGO_BALL_LOOP,
    MAGO_BALL_END,

    MAGO_VIAJANDO_AL_CENTRO
};

class MagoBoss : public Enemy
{
public:
    MagoBoss();
    virtual ~MagoBoss();

    bool Start() override;
    bool Update(float dt) override;
    void Draw(float dt);

    // Detección de colisiones para cuando el jugador le pegue o choque
    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void SetTextures(SDL_Texture* idle, SDL_Texture* ball, SDL_Texture* trans);
    //void OnCollisionEnd(PhysBody* physA, PhysBody* physB); // Si tu motor lo usa

private:
    // Métodos para separar la lógica de cada fase en el Update
    void UpdateIntroPhase(float dt);
    void UpdateTransformationPhase(float dt);
    void UpdateBossfightPhase(float dt);

    // Métodos de los ataques individuales de la Fase 3
    void ExecuteMeteorAttack(float dt);
    void ExecuteBouncingBallAttack(float dt);

    // Control de animaciones idéntico al del Rey
    void ChangeCurrentAnimation();

private:
    // Fases y estados
    BossPhaseM currentPhase = BossPhaseM::INTROM;
    MagoState stateM = MAGO_NORMAL_IDLE;
    MagoState lastStateM = MAGO_BALL_END; // Para forzar el primer cambio de animación

    // Temporizadores útiles para las fases y ataques
    float phaseTimer = 0.0f;
    float attackTimer = 0.0f;

    // Variables de control para la IA de combate
    int currentAttack = 0;      // 1 para meteoros, 2 para la bola rebotadora
    bool isAttacking = false;
    bool hasBeenHit = false;    // Bandera para detectar el primer golpe del player

    // Variables para las físicas del ataque de la bola (Ataque 2)
    float speedX = 0.0f;
    float speedY = 0.0f;

    AnimationSet* currentAnimTrack;
    SDL_Texture* currentTexture;

    AnimationSet animsIdle;
    AnimationSet animsBall;
    AnimationSet animsTransformation;
    
    SDL_Texture* textureIdle = nullptr;
    SDL_Texture* textureBall = nullptr;
    SDL_Texture* textureTransformation = nullptr;

    bool uvasPendientes = false;
    int spawnX = 0;
    int spawnY = 0;

    bool ballInitialized = false;
    float ballDirX = 0.0f;
    float dirChangeTimer = 0.0f;
};