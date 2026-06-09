#include "MagoBoss.h"
#include "Engine.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Physics.h"
#include "Input.h" // Lo usaremos para pruebas o inputs si hiciese falta

MagoBoss::MagoBoss() : Enemy()
{
    name = "MagoBoss";
}

MagoBoss::~MagoBoss() {}

bool MagoBoss::Start()
{
    texW = 640; // Ajusta al tamaño real de tu spritesheet del mago
    texH = 640;

    // 1. Cargar animaciones desde el TSX (Ajusta las rutas y los IDs de tu asset)
    std::unordered_map<int, std::string> aliasesIdle = { {0,"fly_idle"}, {14,"fly_weak"}, {28,"fly_magic"} };
    std::unordered_map<int, std::string> aliasesBall = { {0,"attack_start"}, {25,"attack_loop"},{30,"attack_end"} };
    std::unordered_map<int, std::string> aliasesTransform = { {0,"normal_idle"}, {56,"weak_start"}, {70,"weak_loop"}, {84,"transformation"} };

    animsIdle.LoadFromTSX("assets/Textures/Spritesheets/Wizard Cheese/Cmage_transformation.tsx", aliasesIdle);
    animsBall.LoadFromTSX("assets/Textures/Spritesheets/Wizard Cheese/Cmage_transformation.tsx", aliasesBall);
    animsTransformation.LoadFromTSX("assets/Textures/Spritesheets/Wizard Cheese/Cmage_transformation.tsx", aliasesTransform);

    

    // 2. Inicializar la física. 
    // Empezamos con un sensor estático o Kinematic para la fase de Intro
    pbody = Engine::GetInstance().physics->CreateRectangleFriction(position.getX(), position.getY(), texW, texH, bodyType::DYNAMIC, 0.1f);
    pbody->listener = this;
    pbody->ctype = ColliderType::ENEMY; // O BOSS si tienes un tipo asignado
    attackHitbox = nullptr;
    // Inicializamos el estado del Boss
    currentPhase = BossPhaseM::INTROM;
    stateM = MAGO_NORMAL_IDLE;
    currentAnimTrack = &animsTransformation;
    currentTexture = textureTransformation;
    animsTransformation.SetCurrent("normal_idle");
    hasBeenHit = false;
    isAttacking = false;
    phaseTimer = 0.0f;

    return true;
}

bool MagoBoss::Update(float dt)
{
    // Máquina de estados principal para las FASES del combate
    switch (currentPhase)
    {
    case BossPhaseM::INTROM:
        UpdateIntroPhase(dt);
        break;

    case BossPhaseM::TRANSFORMATIONM:
        UpdateTransformationPhase(dt);
        break;

    case BossPhaseM::BOSSFIGHTM:
        UpdateBossfightPhase(dt);
        break;
    }

    // Actualiza la animación actual según el estado "stateM"
    ChangeCurrentAnimation();

    // Dibuja al Boss en pantalla
    Draw(dt);

    return true;
}

void MagoBoss::UpdateIntroPhase(float dt)
{
    // El mago spawnea y se queda en MAGO_NORMAL_IDLE

    // Al interactuar con él (hablarle), chequeas la variable de la Scene.
    // Reemplaza "Engine::GetInstance().sceneManager->currentScene->tuVariable" por tu acceso real.
    bool sceneCondition = true; // Simulación de tu variable de la Scene

    if (stateM == MAGO_NORMAL_IDLE && sceneCondition /* && jugador Pulsa E o habla */)
    {
        stateM = MAGO_WEAK_START;
    }

    // Si está en weak_start, esperamos a que acabe esa animación para pasar al loop de espera
    if (stateM == MAGO_WEAK_START && currentAnimTrack->HasFinished())
    {
        stateM = MAGO_WEAK_LOOP;
    }

    // Mientras esté en weak_loop, espera a recibir un golpe.
    // Cuando el player le pega (se activa en OnCollision), pasa a transformarse.
    if (stateM == MAGO_WEAK_LOOP && hasBeenHit)
    {
        currentPhase = BossPhaseM::TRANSFORMATIONM;
        stateM = MAGO_TRANSFORMATION_FX;
    }
}
void MagoBoss::UpdateTransformationPhase(float dt)
{
    // El mago está ejecutando la animación "transformation"
    if (stateM == MAGO_TRANSFORMATION_FX && currentAnimTrack->HasFinished())
    {
        // Al terminar, vuela al centro de la pantalla para empezar la pelea
        // Aquí moverías su pbody a la posición del medio de la pantalla
        // int centroX = ...; int centroY = ...;
        // pbody->SetPosition(centroX, centroY);

        currentPhase = BossPhaseM::BOSSFIGHTM;
        stateM = MAGO_FLY_IDLE; // Estado por defecto en combate
    }
}

void MagoBoss::UpdateBossfightPhase(float dt)
{
    // FASE 3: El combate real
    if (!isAttacking)
    {
        attackTimer += dt;

        // Cada 3 segundos en reposo, el boss elige un ataque aleatorio
        if (attackTimer >= 3.0f)
        {
            isAttacking = true;
            attackTimer = 0.0f;

            // Decidir ataque: 1 o 2 de forma aleatoria
            currentAttack = (rand() % 2) + 1;
        }
    }
    else
    {
        // Ejecutar el ataque seleccionado
        if (currentAttack == 1)
        {
            ExecuteMeteorAttack(dt);
        }
        else if (currentAttack == 2)
        {
            ExecuteBouncingBallAttack(dt);
        }
    }
}

void MagoBoss::ExecuteMeteorAttack(float dt)
{
    //stateM = MAGO_FLOATING;

    // Lógica del Ataque 1:
    // - Moverse al centro de la pantalla flotando.
    // - Ir spawneando entidades "UvaMeteorito" verticalmente usando el EntityManager de forma periódica.
    // - Cuando pase X tiempo, isAttacking = false y vuelve a la rutina normal.
}

void MagoBoss::ExecuteBouncingBallAttack(float dt)
{
    //stateM = MAGO_BOUNCING_BALL;

    // Lógica del Ataque 2:
    // - Cambiar el pbody a DYNAMIC para que responda a rebotes (o simularlo por código).
    // - Aplicar fuerzas para que rebote por el escenario.
    // - En OnCollision, si toca el suelo, spawnear proyectiles de uva a los lados.
}

void MagoBoss::ChangeCurrentAnimation()
{
    if (stateM == lastStateM) return;
    lastStateM = stateM;

    switch (stateM)
    {
        // --- TEXTURA Y ANIMACIONES DE TRANSFORMAClÓN ---
    case MAGO_NORMAL_IDLE:
        currentAnimTrack = &animsTransformation;
        currentTexture = textureTransformation;
        animsTransformation.SetCurrent("normal_idle");
        break;
    case MAGO_WEAK_START:
        currentAnimTrack = &animsTransformation;
        currentTexture = textureTransformation;
        animsTransformation.SetCurrent("weak_start");
        break;
    case MAGO_WEAK_LOOP:
        currentAnimTrack = &animsTransformation;
        currentTexture = textureTransformation;
        animsTransformation.SetCurrent("weak_loop");
        break;
    case MAGO_TRANSFORMATION_FX:
        currentAnimTrack = &animsTransformation;
        currentTexture = textureTransformation;
        animsTransformation.SetCurrent("transformation");
        break;

        // --- TEXTURA Y ANIMACIONES DE IDLE / VUELO ---
    case MAGO_FLY_IDLE:
        currentAnimTrack = &animsIdle;
        currentTexture = textureIdle;
        animsIdle.SetCurrent("fly_idle");
        break;
    case MAGO_FLY_WEAK:
        currentAnimTrack = &animsIdle;
        currentTexture = textureIdle;
        animsIdle.SetCurrent("fly_weak");
        break;
    case MAGO_FLY_MAGIC:
        currentAnimTrack = &animsIdle;
        currentTexture = textureIdle;
        animsIdle.SetCurrent("fly_magic");
        break;

        // --- TEXTURA Y ANIMACIONES DE LA BOLA ---
    case MAGO_BALL_START:
        currentAnimTrack = &animsBall;
        currentTexture = textureBall;
        animsBall.SetCurrent("attack_start");
        break;
    case MAGO_BALL_LOOP:
        currentAnimTrack = &animsBall;
        currentTexture = textureBall;
        animsBall.SetCurrent("attack_loop");
        break;
    case MAGO_BALL_END:
        currentAnimTrack = &animsBall;
        currentTexture = textureBall;
        animsBall.SetCurrent("attack_end");
        break;
    }
}

void MagoBoss::OnCollision(PhysBody* physA, PhysBody* physB)
{
    // Detectar si estamos en Fase 1 (Intro) y el jugador nos ataca
    if (currentPhase == BossPhaseM::INTROM)
    {
        // Ajusta esto según cómo gestiones los ataques del jugador (p.ej. si el collider del jugador cambia a "ATTACK")
        if (physB->ctype == ColliderType::PLAYERATTACK)
        {
            hasBeenHit = true;
        }
    }

    // Si estamos en la Fase 3 y es el ataque de la bola rebotando contra el suelo
    //if (currentPhase == BossPhase::BOSSFIGHT && stateM == MAGO_BOUNCING_BALL)
    //{
    //    if (physB->ctype == ColliderType::PLATFORM)
    //    {
    //        // Aquí irá la lógica de hacer aparecer uvas proyectil hacia los lados
    //    }
    //}
}

void MagoBoss::SetTextures(SDL_Texture* idle, SDL_Texture* ball, SDL_Texture* trans) {
    textureIdle = idle;
    textureBall = ball;
    textureTransformation = trans;
}
void MagoBoss::Draw(float dt)
{
    if (currentAnimTrack == nullptr || currentTexture == nullptr) return;

    // Actualizamos el track de animación que esté activo en este frame
    currentAnimTrack->Update(dt);
    const SDL_Rect& animFrame = currentAnimTrack->GetCurrentFrame();

    int x, y;
    pbody->GetPosition(x, y);
    position.setX((float)x);
    position.setY((float)y);

    SDL_FlipMode flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    int drawX = x - animFrame.w / 2;
    int drawY = y - animFrame.h / 2;

    Engine::GetInstance().render->DrawTexture(
        currentTexture, // <--- Usa la textura del estado actual
        drawX,
        drawY,
        &animFrame,     // <--- Usa el frame del estado actual
        1.0f,
        0.0,
        INT_MAX,
        INT_MAX,
        flip
    );
}