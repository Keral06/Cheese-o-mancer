#include "Verdugo.h"
#include "Engine.h"
#include "Log.h"
#include "Textures.h"
#include <Scene.h>
#include "coins.h"
#include "EntityManager.h"
#include "WeakWall.h"

Verdugo::Verdugo() : Enemy()
{
    name = "Verdugo";
}

Verdugo::~Verdugo()
{
}

bool Verdugo::Start()
{
    texW = 128*3;
    texH = 128*5;
	attackRange = 5;
    offsetAttackHitboxX = 40;
    offsetAttackHitboxY = -texH/2;
    std::unordered_map<int, std::string> aliasesAnimFase1 = { {0,"ataqueFase1"} };
    std::unordered_map<int, std::string> aliasesAnimIdle = { {0,"idle"}, {30,"walk"}  };

    std::unordered_map<int, std::string> aliasesAnimT = { {0,"transform1"} };
    std::unordered_map<int, std::string> aliasesAnimT2 = { {0,"transform2"} };
    std::unordered_map<int, std::string> aliasesAnim1 = { {0,"attack1"} };
    std::unordered_map<int, std::string> aliasesAnim2 = { {0,"attack2"} };
    std::unordered_map<int, std::string> aliasesAnim3a = { {0,"attack3_start"}, {12,"attack3_run"} };
    std::unordered_map<int, std::string> aliasesAnim3b = { {0,"attack3b"} };
    std::unordered_map<int, std::string> aliasesAnim3c = { {0,"attack3c"} };
    std::unordered_map<int, std::string> aliasesAnim4a = { {0,"attack4a"} };
    std::unordered_map<int, std::string> aliasesAnim4b = { {0,"attack4b"} };
    std::unordered_map<int, std::string> aliasesAnimDeath = { {0,"death"} };

    animsAtaqueFase1.LoadFromTSX("assets/Textures/Spritesheets/Executioner/AtaqueFase1.tsx", aliasesAnimFase1);
    animsIdle.LoadFromTSX("assets/Textures/Spritesheets/Executioner/Verdugo_Idle_Walk.tsx", aliasesAnimIdle);

    animsTransform1.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_transform1.tsx",aliasesAnimT);
    animsTransform2.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_transform2.tsx", aliasesAnimT2);
    animsAtaque1.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_attack1.tsx", aliasesAnim1);
    animsAtaque2.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_attack2.tsx", aliasesAnim2);
    animsAtaque3a.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_attack3a.tsx", aliasesAnim3a);
    animsAtaque3b.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_attack3b.tsx", aliasesAnim3b);
    animsAtaque3c.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_attack3c.tsx", aliasesAnim3c);
    animsAtaque4a.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_attack4a.tsx", aliasesAnim4a);
    animsAtaque4b.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_attack4b.tsx", aliasesAnim4b);
    animsDeath.LoadFromTSX("assets/Textures/Spritesheets/Cheese Executoner/ce_death.tsx", aliasesAnimDeath);
    
    texName = "assets/Textures/Spritesheets/Cheese Executoner/transformar.png";
    spriteSheetName = "";
    
    //Initialize Player parameters
    textureAF1 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Executioner/Verdugo_Attack.png");
    textureIdleWalk = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Executioner/Verdugo_Idle_Walk.png");

    texture = Engine::GetInstance().textures->Load(texName);
    textureA1 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/ataque1.png");
    textureA2 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/ataque2.png");
    textureA3a = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/ataque3a.png");
    textureA3b = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/ataque3b.png");
    textureA3c = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/ataque3c.png");
    textureA4a = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/ataque4a.png");
    textureA4b = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/ataque4b.png");
    textureDeath = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/death.png");
    textureT1 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/test1.png");
    textureT2 = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Cheese Executoner/test2.png");
    
    Engine::GetInstance().scene->cheese = true;
    //Add physics to the enemy - initialize physics body
    pbody = Engine::GetInstance().physics->CreateRectangle(position.getX(), position.getY(), texW, texH, bodyType::DYNAMIC);

    //Assign enemy class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
    pbody->listener = this;

    //ssign collider type
    pbody->ctype = ColliderType::BOSSATTACK;

    // Initialize pathfinding
    pathfinding = std::make_shared<Pathfinding>();
    //Get the position of the enemy
    Vector2D pos = GetPosition();
    //Convert to tile coordinates
    Vector2D tilePos = Engine::GetInstance().map->WorldToMap((int)pos.getX(), (int)pos.getY());
    //Reset pathfinding
    pathfinding->ResetPath(tilePos);
    
    CreateAttackHitbox(GetPosition().getX(),GetPosition().getY(), 70,200);

    phase = PHASE_INTROV;
    introFinished = false;
    transformationFinished = false;

    state = IDLEV;
    lastState = IDLEV;

    currentAnimSet = &animsIdle;
    currentAnimSet->SetCurrent("idle");
    offsetY = 0.0f;
    texture = textureIdleWalk;

    health = 200;

    attackInProgress = false;
    isAttacking = false;
    velocity.x = 0;
    velocity.y = 0;
 
    Engine::GetInstance().scene->cards.push("Justice", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Justice.png"), Engine::GetInstance().textures->Load("assets/UI/Tarot/Invertido/UI_TarotCard_Justice_inverted.png"));

    Dialogue dialogue("assets/Dialogues/Justice/Justice_Dialogues_Battle.txt", "assets/Dialogues/Justice/Justice_Names_Battle.txt");
    this->dialogue = dialogue;
    return true;
}

void Verdugo::Attack()
{
	isAttacking = true;
	attackTimer = attackDuration;

	LOG("Verdugo empieza ataque");
}

bool Verdugo::Update(float dt)
{
    if (health <= 0 && !deathStarted)
    {
        EnterEndState();
        deathStarted = true;
    }


    switch (phase)
    {
    case PHASE_INTROV:
        UpdateIntro(dt);
        break;

    case PHASE_1V:
        UpdatePhase1(dt);
        break;

    case PHASE_TRANSFORMV:
        UpdateTransformation(dt);
        break;

    case PHASE_2V:
        UpdatePhase2(dt);
        break;

    case PHASE_ENDCHOICEV:
        UpdateEndChoice(dt);
        break;

    default:
        break;
    }

    ChangeCurrentAnimation();
    ApplyPhysics();
    Draw(dt);

    return true;
}

void Verdugo::UpdateIntro(float dt)
{
    if (!introTriggered)
    {
        float dist = CalculateDistance();

        //LOG("%f",dist);

        if (dist < 10.0f) // ajusta el trigger
        {
         
            // ZOOM AL BOSS
            Engine::GetInstance().render->SetZoomSmooth(0.5f, 800.0f);
            // LOCK PLAYER
            //Engine::GetInstance().scene->LockPlayer(true);
            if (dialogue.hasStarted == false) { dialogue.AvanzarDialogo(dt, nameNPC); }
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
            
            
                if (dialogue.AvanzarDialogo(dt, nameNPC)) {
                
                    introTriggered = true;
                    return;
                }
            
            }
            if (dialogue.hasEnded == false && dialogue.hasStarted) {

                dialogue.Draw(dt);
                return;

            }
         

            introTimer = 120.0f; // frames o tiempo simple

            state = IDLEV; // boss quieto
            velocity.x = 0;
            Engine::GetInstance().scene->isInBossfight = true;
            LOG("Boss intro cinematic START");
        }

        return;
    }

    if (introTriggered && !introCinematicDone)
    {
        introTimer--;

        if (introTimer <= 0)
        {
            // volver zoom normal
            Engine::GetInstance().render->SetZoomSmooth(0.3f, 800.0f);

            introCinematicDone = true;

            // unlock player
            //Engine::GetInstance().scene->LockPlayer(false);

            phase = PHASE_1V;

            state = IDLEV;
            currentAnimSet->SetCurrent("idle");

            LOG("Boss intro cinematic END -> PHASE 1 START");
        }

        return;
    }
}

void Verdugo::UpdatePhase1(float dt)
{
    if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
    {
        // Accede a tu instancia de Verdugo y llama al debug
        DebugChangePhase();
    }

    if (health <= 100)
    {
        StartTransformation();
        return;
    }

    Vector2D playerPos = Engine::GetInstance().scene->GetPlayerPosition();
    Vector2D myPos = GetPosition();

    float dist = CalculateDistance();

    facingLeft = playerPos.getX() > myPos.getX();

    // =========================
    // FASE 1 AI SIMPLE
    // =========================

    if (attackInProgress)
    {
        velocity.x = 0;
        UpdatePhase1Attack();
        return;
    }

  
    if (dist < 7.0f)
    {
        ExecutePhase1Attack();
        return;
    }

   
    if (dist > 7.0f)
    {
        state = WALKV;
        velocity.x = 0;
    }

    else
    {
        state = IDLEV;
        velocity.x = 0;
    }

    if (state == WALKV)
    {
        

        int frame = currentAnimSet->GetCurrentFrameIndex(); // si existe

        if (frame == 4 || frame == 12)
        {
            velocity.x = facingLeft ? speed : -speed;
        }
        else
        {
            velocity.x = 0;
        }
    }
}

void Verdugo::ExecutePhase1Attack()
{
    attackInProgress = true;
    state = ATAQUEP1;
    currentAnimSet->Resets();

    hitboxActive = false;
    currentAttackId++; // <-- NUEVO: Cada ataque tiene un número de carné único

    // Impulso (opcional, frena a 0 en el Update si no quieres que deslice)
    velocity.x = facingLeft ? 25.0f : -25.0f;

    LOG("PHASE 1 ATTACK START - ID: %d", currentAttackId);
}

void Verdugo::UpdatePhase1Attack()
{
    // Frenamos el movimiento para que no patine durante el garrotazo
    velocity.x = 0;

    int currentFrame = currentAnimSet->GetCurrentFrameIndex();

    // 1. CONTROL DE FRAMES ACTIVOS (Del 5 al 7)
    if (currentFrame >= 12 && currentFrame <= 15)
    {
        hitboxActive = true;

        // ====================================================
        // OPTIONAL: Si necesitas que la hitbox se mueva con el jefe
        // antes de medir, actualiza su posición aquí. Por ejemplo:
        // int bx, by;
        // pbody->GetPosition(bx, by);
        // int dirX = facingLeft ? offsetAttackHitboxX : -offsetAttackHitboxX;
        // attackHitbox->SetPosition(bx + dirX, by + offsetAttackHitboxY);
        // ====================================================

        // 2. EXTRAER EL RECTÁNGULO REAL DE TU ATTACKHITBOX
        int hx, hy;
        attackHitbox->GetPosition(hx, hy); // Obtenemos la posición física actual de tu sensor

        // Creamos el SDL_Rect usando el tamaño exacto que le diste en el Start (70x200)
        // Nota: Box2D suele medir desde el centro, si ves que el QueryArea se desplaza, 
        // réstale la mitad del ancho y del alto como hacías en el Draw.
        SDL_Rect hitboxRect = {
            hx - (70 / 2),
            hy - (200 / 2),
            70,
            200
        };

        // 3. ESCANEO DEL ÁREA
        std::vector<PhysBody*> hits = Engine::GetInstance().physics->QueryArea(hitboxRect);

        for (auto* body : hits)
        {
            if (body->ctype == ColliderType::PLAYER)
            {
                Player* p = static_cast<Player*>(body->listener);

                // 4. LÓGICA DE DAÑO ÚNICO (Para que no te destroze la vida en 1 frame)
                if (p && p->lastAttackId != currentAttackId)
                {
                    // ¡Aquí pones la función real de quitar vida a tu jugador!
                    p->HasBeenHit(pbody);

                    p->lastAttackId = currentAttackId; // El jugador se anota el ID para ser inmune al resto de este golpe
                    LOG("¡HIT! El Verdugo te ha dado con su attackHitbox real en el frame %d", currentFrame);
                }
            }
        }
    }
    else
    {
        hitboxActive = false;
    }

    // 5. FIN DE LA ANIMACIÓN
    if (currentAnimSet->HasFinished())
    {
        attackInProgress = false;
        hitboxActive = false;
        currentAnimSet->Resets();
        state = IDLEV; // Volver a Idle
        LOG("PHASE 1 ATTACK END");
    }
}

void Verdugo::StartTransformation()
{
    phase = PHASE_TRANSFORMV;

    velocity.x = 0;
    velocity.y = 0;

    attackInProgress = false;

    state = TRANSFORM1;
    currentAnimSet = &animsTransform1;
    currentAnimSet->Resets();

    // bloquear player aquí si tienes sistema
    LOG("TRANSFORMACIÓN INICIADA");
}

void Verdugo::UpdatePhase2(float dt)
{
    if (phase != PHASE_2V) return;

    float distance = CalculateDistance();

    facingLeft = Engine::GetInstance().scene->GetPlayerPosition().getX() > GetPosition().getX();

    repathTimer++;
    GetPhysicsValues();

    if (!attackInProgress)
    {
        if (distance > 200.0f)
        {
            currentAttack = ATTACK_2;
            ExecuteAttack();
        }
        else if (distance > 100.0f)
        {
            currentAttack = ATTACK_3;
            ExecuteAttack();
        }
        else
        {
            currentAttack = ChooseRandomAttack();
            ExecuteAttack();
        }
    }
    else
    {
        UpdateAttackLogic();
    }
}

void Verdugo::EnterEndState()
{
    phase = PHASE_ENDCHOICEV;

    velocity.x = 0;
    velocity.y = 0;

    attackInProgress = false;

    state = MUERTO;
    currentAnimSet = &animsDeath;
    currentAnimSet->Resets();

    LOG("Boss muerto");
}

void Verdugo::UpdateEndChoice(float dt)
{
    currentAnimSet->Update(dt);

    // aquí luego meterás input de perdonar / ejecutar
}

void Verdugo::Draw(float dt)
{
    currentAnimSet->Update(dt);
    const SDL_Rect& animFrame = currentAnimSet->GetCurrentFrame();

    //// Update render position using your PhysBody helper
    int x, y;
    pbody->GetPosition(x, y);
    position.setX((float)x);
    position.setY((float)y);

    

    SDL_FlipMode flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    int drawX = x - animFrame.w / 2 - offsetX;
    int drawY = y - animFrame.h / 2 - offsetY;

    Engine::GetInstance().render->DrawTexture(
        texture,
        drawX,
        drawY,
        &animFrame,
        1.0f,
        0.0,
        INT_MAX,
        INT_MAX,
        flip
    );
    //Draw the player using the texture and the current animation frame
    /*Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &sect, 1, 0, 0, 0, SDL_FLIP_NONE);*/

    //SDL_FlipMode flip = facingLeft ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    /*Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &animFrame, 1.0f, 0.0, INT_MAX, INT_MAX, SDL_FLIP_NONE);*/
}

void Verdugo::UpdateAttack()
{
    if (!isAttacking) return;

    attackTimer--;

    // Ventana de ataque
    if (attackTimer <= (attackDuration - hitboxStart) &&
        attackTimer >= (attackDuration - hitboxEnd))
    {
        if (!hitboxActive) {
            hitboxActive = true;
            hasHit = false; 
            LOG("Hitbox ACTIVADA");
        }
    }
    else {
        if (hitboxActive) {
            hitboxActive = false;
            LOG("Hitbox DESACTIVADA");
        }
    }

    
    if (hitboxActive && playerInHitbox && !hasHit) {
        LOG("AAUAUCHHH");

      

        hasHit = true; 
    }

    // Fin del ataque
    if (attackTimer <= 0) {
        isAttacking = false;
        hitboxActive = false;
        attackTimer = attackCooldown;

        LOG("Ataque terminado");
    }
}

void Verdugo::OnCollision(PhysBody* physA, PhysBody* physB)
{
    switch (physB->ctype)
    {
    case ColliderType::CHEESEBALL:
    {
        bolazo = true;
        break;
    }
    // Si tienes otros proyectiles o trampas que afecten al jefe, se quedan aquí.
    default:
        break;
    }
}

void Verdugo::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
    if (physA == attackHitbox && physB->ctype == ColliderType::PLAYER) {
        playerInHitbox = false;
    }
}

void Verdugo::ChangeCurrentAnimation() {

    if (state == lastState) return;

    lastState = state;

    switch (state)
    {
    case IDLEV:
        hitboxActive = false;
        currentAnimSet = &animsIdle;
        texture = textureIdleWalk;
        playerInHitbox = false;
        currentAnimSet->SetCurrent("idle");
        offsetY = 0.0f;
        break;

    case WALKV:
        hitboxActive = false;
        playerInHitbox = false;
        currentAnimSet = &animsIdle;
        texture = textureIdleWalk;
        currentAnimSet->SetCurrent("walk");
        offsetY = 0.0f;
        offsetX = -128.0f / 2;
        break;

    case ATAQUEP1:
        currentAnimSet = &animsAtaqueFase1;
        texture = textureAF1;
        currentAnimSet->SetCurrent("ataqueFase1");
        offsetY = 128.0f/2;
        offsetX = 128.0f/2;
        break;

    case ATAQUE1:
        currentAnimSet = &animsAtaque1;
        texture = textureA1;
        currentAnimSet->SetCurrent("attack1");
        offsetY = 0.0f;
        break;

    case ATAQUE2:
        currentAnimSet = &animsAtaque2;
        texture = textureA2;
        currentAnimSet->SetCurrent("attack2");
        offsetY = 0.0f;
        break;

    case ATAQUE3START:
        currentAnimSet = &animsAtaque3a;
        texture = textureA3a;
        currentAnimSet->SetCurrent("attack3_start");
        offsetY = texH / 2;
        break;

    case ATAQUE3A:
        currentAnimSet = &animsAtaque3a;
        texture = textureA3a;
        currentAnimSet->SetCurrent("attack3_run"); 
        offsetY = texH/2;
        break;

    case ATAQUE3B:
        currentAnimSet = &animsAtaque3b;
        texture = textureA3b;
        currentAnimSet->SetCurrent("attack3b");
        offsetY = texH/2;
        break;

    case ATAQUE3C:
        currentAnimSet = &animsAtaque3c;
        texture = textureA3c;
        currentAnimSet->SetCurrent("attack3c");
        offsetY = texH/2;
        break;

    case ATAQUE4A:
        currentAnimSet = &animsAtaque4a;
        texture = textureA4a;
        currentAnimSet->SetCurrent("attack4a");
        offsetY = texH/2;
        break;

    case ATAQUE4B:
        currentAnimSet = &animsAtaque4b;
        texture = textureA4b;
        currentAnimSet->SetCurrent("attack4b");
        offsetY = texH/2;
        break;

    case MUERTO:
        currentAnimSet = &animsDeath;
        texture = textureDeath;
        currentAnimSet->SetCurrent("death");
        offsetY = 0.0f;
        break;

    case TRANSFORM1:
        currentAnimSet = &animsTransform1;
        texture = textureT1;
        currentAnimSet->SetCurrent("transform1");
        break;

    case TRANSFORM2:
        currentAnimSet = &animsTransform2;
        texture = textureT2;
        currentAnimSet->SetCurrent("transform2");
        break;
    default:
        break;
    }

}

void Verdugo::DebugChangeState()
{
    int next = (int)state + 1;

    if (next > MUERTO)
        next = ATAQUE1;

    state = (VerdugoState)next;
    currentAnimSet->Resets();
    LOG("Cambio de estado a: %d", state);
}

AttackType Verdugo::ChooseRandomAttack()
{
    int r = rand() % 4; 

    AttackType atk;

    switch (r)
    {
    case 0: atk = ATTACK_1; break;
    case 1: atk = ATTACK_2; break;
    case 2: atk = ATTACK_3; break;
    case 3: atk = ATTACK_4; break;
    default: atk = ATTACK_1; break;
    }

    LOG("Elegido ataque: %d", atk);

    return atk;
}
void Verdugo::ExecuteAttack()
{
    float targetX = facingLeft ? 900.0f : 10000.0f;
    float dx = targetX - GetPosition().getX();
    switch (currentAttack)
    {
    case ATTACK_1:
        LOG("INICIO ATAQUE 1 (garrotazo)");
        state = ATAQUE1;
        attackInProgress = true;
        velocity.x = facingLeft ? 25.0f : -25.0f;
        Engine::GetInstance().physics->SetLinearVelocity(pbody, velocity);
        break;

    case ATTACK_2:
        LOG("INICIO ATAQUE 2 (salto)");
        state = ATAQUE2;

        // impulso parabólico simple
        velocity.y = -30.0f;
        velocity.x = facingLeft ? 25.0f : -25.0f;
        Engine::GetInstance().physics->SetLinearVelocity(pbody, velocity);
        attackInProgress = true;
        break;

    case ATTACK_3:
        LOG("INICIO ATAQUE 3 (persecución)");
        state = ATAQUE3START;
        velocity.x = facingLeft ? 25.0f : -25.0f;
        attackInProgress = true;
        break;
    case ATTACK_4:
        LOG("INICIO ATAQUE 4 (pared)");

        state = ATAQUE3START; 
        attackInProgress = true;
        break;
    default:
        break;
    }
}

void Verdugo::UpdateAttackLogic()
{
    float targetX = facingLeft ? 6000.0f : 900.0f;
    float myX = GetPosition().getX();
    float playerX = Engine::GetInstance().scene->GetPlayerPosition().getX();

    switch (currentAttack)
    {
    case ATTACK_1:
        // termina cuando animación acaba
        if (currentAnimSet->HasFinished())
        {
            currentAnimSet->Resets();
            attackInProgress = false;
        }
        break;

    case ATTACK_2:
        if (currentAnimSet->HasFinished())
        {
            currentAnimSet->Resets();
            attackInProgress = false;
            velocity.x = 0;
        }
        break;

    case ATTACK_3:
        // fase A: acercarse corriendo
        if (state == ATAQUE3START) {
            if (currentAnimSet->HasFinished())
            {
                currentAnimSet->Resets();
                state = ATAQUE3A;
                hitboxActive = true;
                bolazo = false;
            }
        }
        if (state == ATAQUE3A)
        {
            velocity.x = (playerX > myX) ? speed : -speed;

            Engine::GetInstance().physics->SetLinearVelocity(pbody, velocity);

            if (playerInHitbox)
            {
                state = ATAQUE3B;
                currentAnimSet->Resets();
                hitboxActive = false;
                break;
            }

            
            if (bolazo)
            {
                bolazo = false;
                state = ATAQUE3C;
                currentAnimSet->Resets();
                hitboxActive = false;
                break;
            }


        }
        if (state == ATAQUE3B)
        {
            if (currentAnimSet->HasFinished())
            {
                currentAnimSet->Resets();
                attackInProgress = false;
            }
        }
        if (state == ATAQUE3C)
        {
            if (currentAnimSet->HasFinished())
            {
                currentAnimSet->Resets();
                attackInProgress = false;
            }
        }
        break;
    case ATTACK_4:
        
        if (state == ATAQUE3START) {
            if (currentAnimSet->HasFinished())
            {
                Engine::GetInstance().render->SetZoomSmooth(0.15f, 600.0f);
                currentAnimSet->Resets();
                state = ATAQUE3A;
            }
        }
        // =========================
        // FASE 1: CORRER AL BORDE
        // =========================
        if (state == ATAQUE3A)
        {
            velocity.x = (targetX > myX) ? speed : -speed;

            if (abs(targetX - myX) < 20.0f)
            {
                velocity.x = 0;
                position.setX(targetX); // fijar

                state = ATAQUE4A;
                currentAnimSet->Resets();

                LOG("Llega al borde ATAQUE4A");
            }
        }

        // =========================
        // FASE 2: ATAQUE NORMAL
        // =========================
        else if (state == ATAQUE4A)
        {
            if (currentAnimSet->HasFinished() && !wallSpawned) {
                SpawnWeakWall();
                
                wallSpawned = true;
            }

            if (wallDestroyed)
            {
                wallDestroyed = false;
                wallSpawned = false;

                state = ATAQUE4B;
                currentAnimSet->Resets();

                LOG("Paso a ATAQUE4B");
            }
        }
        else if (state == ATAQUE4B)
        {
            if (currentAnimSet->HasFinished())
            {
                Engine::GetInstance().render->SetZoomSmooth(0.3f, 600.0f);
                currentAnimSet->Resets();
                attackInProgress = false;
            }
        }

        
        break;
    
    default:
        break;
    }
}
bool Verdugo::MoveToAttackRange(float targetRange)
{
    Vector2D playerPos = Engine::GetInstance().scene->GetPlayerPosition();
    Vector2D myPos = GetPosition();

    float dx = playerPos.getX() - myPos.getX();
    float dy = playerPos.getY() - myPos.getY();

    float dist = sqrt(dx * dx + dy * dy);

    facingLeft = dx < 0;

    if (dist > targetRange)
    {
        velocity.x = facingLeft ? -speed : speed;
        return false;
    }

    velocity.x = 0;
    return true;
}

void Verdugo::OnWallDestroyed()
{
    LOG("Verdugo detecta que la pared se rompió");
    wallDestroyed = true;
    
}

void Verdugo::Die() {
    anims.SetCurrent("death");
    Engine::GetInstance().scene->cards.GirarCarta("Justice");
    int numeroDeMonedas = 10;
    const Vector2D& pos = this->GetPosition();
    Engine::GetInstance().scene->isInBossfight = false;
    for (int i = 0; i < numeroDeMonedas; ++i) {
        auto newCoin = Engine::GetInstance().entityManager->CreateEntity(EntityType::COIN);
        auto coinEntity = std::static_pointer_cast<Coins>(newCoin);

        if (coinEntity) {
            int offsetX = (i * 30);

            coinEntity->xInicial = (int)pos.getX() + offsetX;
            coinEntity->yInicial = (int)pos.getY();
            coinEntity->Start();
        }
    }
    //this->toDelete = true;
}

void Verdugo::SpawnWeakWall()
{
    Vector2D playerPos = Engine::GetInstance().scene->GetPlayerPosition();
    Vector2D myPos = GetPosition();

    auto newWall = Engine::GetInstance().entityManager->CreateEntity(EntityType::WEAKWALL);
    auto wall = std::static_pointer_cast<WeakWall>(newWall);

    if (wall)
    {
        wall->width = 128;
        wall->height = texH;
        wall->owner = this;
        int offset = 128;

        if (facingLeft)
        {
            
            wall->position.setX(myPos.getX() + offset);
        }
        else
        {
            
            wall->position.setX(myPos.getX() - offset);
        }

        wall->position.setY(myPos.getY());

        wall->Start();
        wallSpawned = true;
    }
}

void Verdugo::DebugChangePhase()
{
    LOG("DEBUG: Saltando a FASE 2 directamente");

    // Forzamos el estado de vida y llamamos a la función de transformación
    health = 50; // O el valor que dispare tu condición
    StartTransformation();
}

void Verdugo::UpdateTransformation(float dt)
{
    velocity.x = 0;
    velocity.y = 0;

    if (state == TRANSFORM1)
    {
        if (currentAnimSet->HasFinished())
        {
            state = TRANSFORM2;
            currentAnimSet->Resets();
        }
    }
    else if (state == TRANSFORM2)
    {
        if (currentAnimSet->HasFinished())
        {
            phase = PHASE_2V;

            state = IDLEV;
            currentAnimSet->Resets();

            attackInProgress = false;

            LOG("PHASE 2 ACTIVADA");
        }
    }
}