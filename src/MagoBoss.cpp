#include "MagoBoss.h"
#include "Engine.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Physics.h"
#include "Input.h" // Lo usaremos para pruebas o inputs si hiciese falta
#include "Uva.h"

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

    animsIdle.LoadFromTSX("assets/Textures/Spritesheets/Wizard Cheese/Cmage_idle.tsx", aliasesIdle);
    animsBall.LoadFromTSX("assets/Textures/Spritesheets/Wizard Cheese/Cmage_ball attack.tsx", aliasesBall);
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
    // 1. Si termina el destello de la transformación, lo mandamos a VIAJAR
    if (stateM == MAGO_TRANSFORMATION_FX && currentAnimTrack->HasFinished())
    {
        stateM = MAGO_VIAJANDO_AL_CENTRO;
    }

    // 2. Lógica de viaje pura y dura
    if (stateM == MAGO_VIAJANDO_AL_CENTRO)
    {
        int currentX, currentY;
        pbody->GetPosition(currentX, currentY);

        float centroX = 6000.0f;
        float centroY = 5000.0f;

        float distX = centroX - currentX;
        float distY = centroY - currentY;

        // Si está muy cerca, lo CLAVAMOS y cambiamos de fase inmediatamente
        if (abs(distX) <= 20.0f && abs(distY) <= 20.0f)
        {
            Engine::GetInstance().physics->SetLinearVelocity(pbody, 0.0f, 0.0f);
            pbody->SetPosition(centroX, centroY);

            // ¡AQUÍ ESTÁ LA CLAVE! Pasamos a la pelea y lo dejamos en FLY_IDLE estable
            currentPhase = BossPhaseM::BOSSFIGHTM;
            stateM = MAGO_FLY_IDLE;
            attackTimer = 0.0f;
            isAttacking = false;
        }
        else
        {
            // Movimiento de aproximación suave
            float factorSuave = 2.0f;
            Engine::GetInstance().physics->SetLinearVelocity(pbody, distX * factorSuave, distY * factorSuave);
        }
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
    // Al empezar el ataque, cambiamos la animación a magia
    if (stateM != MAGO_FLY_MAGIC) {
        stateM = MAGO_FLY_MAGIC;
        attackTimer = 0.0f; // Usamos el timer para controlar la cadencia de disparo
        phaseTimer = 0.0f;  // Usamos phaseTimer para controlar la duración total del ataque
    }

    phaseTimer += dt;
    attackTimer += dt;

    // 1. Spawnear una uva cada 0.3 segundos
    if (attackTimer >= 0.3f)
    {
        attackTimer = 0.0f;

        // Calculamos una X aleatoria alrededor de la pantalla para que caigan en lluvia
        // Ejemplo: entre la X=200 y la X=1700
        float randomX = 200 + (rand() % 1500);
        float spawnY = -50.0f; // Aparecen justo por encima del techo de la pantalla

        // Creamos el proyectil (Asegúrate de tener este EntityType configurado)
        auto uva = Engine::GetInstance().entityManager->CreateEntity(EntityType::UVA);
        if (uva) {
            uva->position.setX(randomX);
            uva->position.setY(spawnY);
            uva->Start(); // En su Start, la uva debe crearse como DYNAMIC o tener una velocidad Y positiva constante hacia abajo
        }
    }

    // 2. El ataque dura 5 segundos en total
    if (phaseTimer >= 5.0f)
    {
        // Terminamos el ataque y vuelve a volar en reposo
        isAttacking = false;
        stateM = MAGO_FLY_IDLE;
        attackTimer = 0.0f; // Reset del cooldown para el próximo ataque
    }
}

void MagoBoss::ExecuteBouncingBallAttack(float dt)
{
    // PASO A: Iniciar la transformación a bola (Animación de carga)
    if (stateM != MAGO_BALL_START && stateM != MAGO_BALL_LOOP && stateM != MAGO_BALL_END)
    {
        stateM = MAGO_BALL_START;
        phaseTimer = 0.0f;
        Engine::GetInstance().physics->SetLinearVelocity(pbody, 0.0f, 0.0f);
        return;
    }

    // PASO B: ¡Aquí empieza lo bueno! La bola se activa
    if (stateM == MAGO_BALL_START && currentAnimTrack->HasFinished())
    {
        stateM = MAGO_BALL_LOOP;
        phaseTimer = 0.0f;

        // 1. LE DEVOLVEMOS LA GRAVEDAD NATIVA A BOX2D (Escala 1.0f)
        b2Body_SetGravityScale(pbody->body, 0.3f);

        // 2. Le damos su velocidad horizontal inicial hacia la derecha (p.ej. 5.0f en metros/s)
        // Usamos tu wrapper para no romper las conversiones
        Engine::GetInstance().physics->SetXVelocity(pbody, 250.0f);
    }

    // PASO C: Bucle de movimiento (Box2D se encarga de la gravedad en Y de forma nativa)
    if (stateM == MAGO_BALL_LOOP)
    {
        phaseTimer += dt;

        // Solo nos encargamos de controlar los límites horizontales de la sala (Paredes)
        int currentX, currentY;
        pbody->GetPosition(currentX, currentY);

        float limiteIzquierdo = 4000.0f;
        float limiteDerecho = 7000.0f;

        // Si toca un límite, invertimos su velocidad X actual usando tus wrappers
        float currentVx = Engine::GetInstance().physics->GetXVelocity(pbody);
        if (currentX <= limiteIzquierdo && currentVx < 0) {
            Engine::GetInstance().physics->SetXVelocity(pbody, abs(currentVx));
        }
        else if (currentX >= limiteDerecho && currentVx > 0) {
            Engine::GetInstance().physics->SetXVelocity(pbody, -abs(currentVx));
        }

        // Duración total del ataque
        if (phaseTimer >= 7.0f) {
            stateM = MAGO_BALL_END;
        }
    }

    // PASO D: Terminar el ataque de la bola
    if (stateM == MAGO_BALL_END && currentAnimTrack->HasFinished())
    {
        // 1. LE VOLVEMOS A QUITAR LA GRAVEDAD para que pueda flotar de nuevo
        b2Body_SetGravityScale(pbody->body, 0.0f);

        // 2. Lo mandamos al estado de viaje para que regrese al centro flotando suavemente
        currentPhase = BossPhaseM::TRANSFORMATIONM;
        stateM = MAGO_VIAJANDO_AL_CENTRO;
        isAttacking = false;
        attackTimer = 0.0f;
    }
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
    case MAGO_VIAJANDO_AL_CENTRO:
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

    // Si estamos en la Fase de Combate, modo Bola Rebotando, y chocamos con el suelo
    if (currentPhase == BossPhaseM::BOSSFIGHTM && stateM == MAGO_BALL_LOOP)
    {
        if (physB->ctype == ColliderType::PLATFORM)
        {
            // ¡EL IMPULSO HACIA ARRIBA NATIVO!
            // Al tocar el suelo, obligamos a la velocidad Y a ser negativa (hacia arriba)
            // Tu wrapper de Physics ya traduce este -700.0f a los metros por segundo que Box2D entiende.
            Engine::GetInstance().physics->SetYVelocity(physA, -700.0f);

            // ... Tu código de spawnear las uvas laterales se queda exactamente igual ...
            int currentX, currentY;
            pbody->GetPosition(currentX, currentY);

            auto entityLeft = Engine::GetInstance().entityManager->CreateEntity(EntityType::UVA);
            if (entityLeft) {
                Uva* uvaIzq = static_cast<Uva*>(entityLeft.get());
                uvaIzq->SetConfiguration(UvaSize::PEQUEÑA, UvaType::LINEAL, -1.0f);
                uvaIzq->position.setX(currentX - 50);
                uvaIzq->position.setY(currentY);
                uvaIzq->Start();
            }

            auto entityRight = Engine::GetInstance().entityManager->CreateEntity(EntityType::UVA);
            if (entityRight) {
                Uva* uvaDer = static_cast<Uva*>(entityRight.get());
                uvaDer->SetConfiguration(UvaSize::PEQUEÑA, UvaType::LINEAL, 1.0f);
                uvaDer->position.setX(currentX + 50);
                uvaDer->position.setY(currentY);
                uvaDer->Start();
            }
        }
    }
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