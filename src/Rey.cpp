#include "Rey.h"
#include "Engine.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Textures.h"
#include "Physics.h"
#include "MagoBoss.h"
#include "Log.h"
#include "Input.h" // Asegúrate de tener el include de Input para detectar teclas
#include "Render.h"


Rey::Rey() : Enemy()
{
    name = "Rey";

    stateR = IDLER;
    lastStateR = DEATH_STATICR;

    offsetX = 0.0f;
    offsetY = 0.0f;

    playerInRange = false;
    magoSpawned = false;
    choosingFinal = false;
    eKeyWasPressed = false;
}

Rey::~Rey() {}

bool Rey::Start() {

    texW = 650;
    texH = 650;

    std::unordered_map<int, std::string> aliasesAnim = {
            {0,"idle"},
            {1, "talking"},
            {28,"death_start"},
            {29, "death_static"}
    };

    anims.LoadFromTSX("assets/Textures/Spritesheets/King/king.tsx", aliasesAnim);
    texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/King/sprite_king_01.png");

    textureIdle = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/Cmage_Idles_spritesheet.png");
    textureBall = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/Cmage_ball attack_spritesheet.png");
    textureTransformation = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/Cmage_transformation_spritesheet.png");

    // NOTA: Se eliminó el pbody duplicado que tenías aquí de fondo
    pbody = Engine::GetInstance().physics->CreateRectangleSensor(position.getX(), position.getY(), texW, texH, bodyType::STATIC);
    pbody->listener = this;
    pbody->ctype = ColliderType::REY;

    anims.SetCurrent("idle");

    Dialogue help("assets/Dialogues/Final_Bossfight/Final_Before_Bossfight_Dialogues.txt", "assets/Dialogues/Final_Bossfight/Final_Before_Bossfight_Names.txt");
    choice = help;
    Dialogue help2("assets/Dialogues/Final_Bossfight/Final_Before_Bossfight_Dialogues2.txt", "assets/Dialogues/Final_Bossfight/Final_Before_Bossfight_Names2.txt");
    noChoice = help2;


    Dialogue help3("assets/Dialogues/Final_Bossfight/Final_WithArtifact_Dialogue.txt", "assets/Dialogues/Final_Bossfight/Final_WithArtifact_Names.txt");
    Artifact = help3;


    Dialogue help4("assets/Dialogues/Endings/Ending_Judgement_Dialogues.txt", "assets/Dialogues/Endings/Ending_Judgement_Names.txt");
    NoArtifact = help3;

    Dialogue help5("assets/Dialogues/Endings/Ending_Temperance_Dialogues.txt", "assets/Dialogues/Endings/Ending_Temperance_Names.txt");
    Temperance = help4;

    Dialogue help6("assets/Dialogues/Endings/Ending_TheDevil_Dialogues.txt", "assets/Dialogues/Endings/Ending_TheDevil_Names.txt");
    theDevil = help5;

    attackHitbox = nullptr;
    return true;
}

void Rey::ChangeCurrentAnimation() {
    if (stateR == lastStateR) return;
    lastStateR = stateR;

    switch (stateR)
    {
    case IDLER:
        anims.SetCurrent("idle");
        offsetY = 0.0f;
        break;
    case TALKINGR:
        anims.SetCurrent("talking");
        offsetY = 0.0f;
        break;
    case DEADR:
        anims.SetCurrent("death_start");
        offsetY = 0.0f;
        break;
    case DEATH_STATICR:
        anims.SetCurrent("death_static");
        offsetY = 0.0f;
        break;
    default:
        break;
    }
}

bool Rey::Update(float dt)
{
    bool eKeyCurrent = (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN);

    // Obtenemos el puntero crudo correcto de tu Scene modular
   

    // --- BLOQUE 1: El jugador está eligiendo activamente entre SPARE o KILL ---
    if (choosingFinal) {

        if (Engine::GetInstance().scene->misiones.HowManyCompleted() > 3 && Engine::GetInstance().scene->hasSparedPrincessAndKnight) {
        
        
            if (choice.hasStarted == false) { choice.AvanzarDialogo(dt, "Rey"); return true; }

            if (eKeyCurrent) {
            
            
                if (choice.AvanzarDialogo(dt, "Rey")) {


                    choosingFinal = false;

                    decision = choice.WhatChoice();
                    return true;
                 }
            
            }

            if (choice.hasStarted && !choice.hasEnded) { choice.Draw(dt); return true; }
        
        
        
        }
        else {
        
        
            if (noChoice.hasStarted == false) { choice.AvanzarDialogo(dt, "Rey"); return true; }

            if (eKeyCurrent) {


                if (noChoice.AvanzarDialogo(dt, "Rey")) {


                    choosingFinal = false;
                    decision = false;//kill him


                }

            }

            if (noChoice.hasStarted && !noChoice.hasEnded) { noChoice.Draw(dt); return true; }
        
        
        
        
        }

        if (decision)//spare
        {

            if (Engine::GetInstance().scene->inventario.tieneObjeto("Artifact")) {
                LOG("¡Tienes el artefacto/amuleto! El Mago se debilita. Comienza la Bossfight");

                Engine::GetInstance().render->SetZoomSmooth(0.5f, 800.0f);

                if (Artifact.hasStarted == false) { Artifact.AvanzarDialogo(dt, "Rey"); return true; }

                if (eKeyCurrent && Artifact.hasEnded == false) {
                
                    if (Artifact.AvanzarDialogo(dt, "Rey")) {
                    
                    
                        Engine::GetInstance().scene->cards.push("Judgement", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Judgement.png"), nullptr);
                    }
                    return true;
                
                
                }
                if (Artifact.hasStarted && !Artifact.hasEnded) { Artifact.Draw(dt); return true; }
                //COMENÇA LLUITA
                // 
                // DESPRES LLUITA, ES EL VIDEO
                // 
                // 
                // Cambiará a DEADR y spawneará al MagoBoss en el bloque de abajo

            }
            else {
                LOG("No tienes el amuleto... El Mago te destruye sin piedad (Final 3)");

                if (NoArtifact.hasStarted == false) { NoArtifact.AvanzarDialogo(dt, "Rey"); return true; }

                if (eKeyCurrent && Artifact.hasEnded == false) {

                    if (NoArtifact.AvanzarDialogo(dt, "Rey")) {
                    
                    
                        Engine::GetInstance().scene->cards.push("Judgement", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Judgement.png"), nullptr);
                    }
                    return true;


                }
                if (NoArtifact.hasStarted && !NoArtifact.hasEnded) { NoArtifact.Draw(dt); return true; }
               
                //MUERTE JESTER

                // TODO: Activar aquí la cinemática del FINAL 3
                // scene->ChangeScene(SceneID::GAME_OVER); // O tu pantalla/video correspondiente
            }



        }

        else if (!decision) //kill him
        {
            LOG("Has elegido: MATAR AL REY (Final 2)");
            choosingFinal = false;


            if (Engine::GetInstance().scene->hasSparedPrincessAndKnight) {
            //Temperance ending
            
                if (Temperance.hasStarted == false) { Temperance.AvanzarDialogo(dt, "Rey"); return true; }

                if (eKeyCurrent && Temperance.hasEnded == false) {

                    if (Temperance.AvanzarDialogo(dt, "Rey")) {
                    
                    
                        Engine::GetInstance().scene->cards.push("Temperance", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Temperance.png"), nullptr);
                    }
                    return true;


                }
                if (Temperance.hasStarted && !Temperance.hasEnded) { NoArtifact.Draw(dt); return true; }
            //kill king, acaba el juego
            }
            else {
            
            //The devil ending
            
                if (theDevil.hasStarted == false) { theDevil.AvanzarDialogo(dt, "Rey"); return true; }

                if (eKeyCurrent && theDevil.hasEnded == false) {

                    if (theDevil.AvanzarDialogo(dt, "Rey")) {


                        Engine::GetInstance().scene->cards.push("Temperance", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_Devil.png"), nullptr);
                  }
                    return true;


                }
                if (theDevil.hasStarted && !theDevil.hasEnded) { theDevil.Draw(dt); return true; }
            }
            // muere rey, acaba el juego

            Die(); // Ejecuta la animación de muerte del rey


        }







        //// Tecla 1: MATAR (KILL) -> Final 2
        //if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
        //    LOG("Has elegido: MATAR AL REY (Final 2)");
        //    choosingFinal = false;

        //    // TODO: Activar aquí la cinemática del FINAL 2 si tienes un método para vídeos o escenas
        //    // scene->PlayVideo("assets/Videos/final2.mpeg"); 

        //    Die(); // Ejecuta la animación de muerte del rey
        //}
        //// Tecla 2: PERDONAR (SPARE) -> Bifurcación entre Final 3 y Jefe (Final 4)
        //else if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
        //    LOG("Has elegido: PERDONAR AL REY");
        //    choosingFinal = false;

        //    // REQUISITO AMULETO: Comprobamos si tiene el artefacto montado o todos los fragmentos
        //    if (Engine::GetInstance().scene->hasAllFragments) {
        //        LOG("¡Tienes el artefacto/amuleto! El Mago se debilita. Comienza la Bossfight");

        //        Engine::GetInstance().render->SetZoomSmooth(0.5f, 800.0f);
        //        Die(); // Cambiará a DEADR y spawneará al MagoBoss en el bloque de abajo

        //        // NOTA: Si quieres que el Mago sepa que está débil, puedes activar un flag en la escena
        //        // scene->isInBossfight = true;
        //    }
        //    else {
        //        LOG("No tienes el amuleto... El Mago te destruye sin piedad (Final 3)");

        //        // TODO: Activar aquí la cinemática del FINAL 3
        //        // scene->ChangeScene(SceneID::GAME_OVER); // O tu pantalla/video correspondiente
        //    }
        //}
    }
    // --- BLOQUE 2: Interacción normal pulsando la E ---
    else if (playerInRange && eKeyCurrent && !eKeyWasPressed) {
        LOG("Has interactuado con el Rey (Tecla E)");

        if (stateR == IDLER) {
            stateR = TALKINGR; // Pasa a hablar
        }
        else if (stateR == TALKINGR) {

            // --- EVALUACIÓN DE REQUISITOS DE LOS FINALES ---

            // 1. ¿Ha perdonado a la princesa y al caballero?
            if (!Engine::GetInstance().scene->hasSparedPrincessAndKnight) {
                LOG("No perdonaste a la princesa y al caballero. Ejecutando Final 1.");
                finalEscogido = FINAL1;
                
            }
            else {
                
                int completedMissionsCount = 0;

                if (Engine::GetInstance().scene->finishedMissionSculptor) completedMissionsCount++;
                if (Engine::GetInstance().scene->finishedmissionHermit)   completedMissionsCount++;
                if (Engine::GetInstance().scene->ratmissionfinished)       completedMissionsCount++;

            

                // 3. ¿Tiene un mínimo de 3 misiones completadas?
                if (completedMissionsCount < 3) {
                    LOG("Perdonaste a los personajes pero faltan misiones completadas (%d/3). Volviendo al Final 1.", completedMissionsCount);
                    finalEscogido = FINAL1;
                }
                else {
                    // Cumple absolutamente todo: Se abre la encrucijada de decisiones
                    LOG("¡REQUISITOS COMPLETADOS! Elige sabiamente: [1] MATAR (Final 2) o [2] PERDONAR (Mago)");
                    choosingFinal = true;

                    
                }
            }
        }
    }

    eKeyWasPressed = eKeyCurrent;

    // Cuando termina la animación de muerte (Bloque original de spawn del Mago)
    if (stateR == DEADR && anims.HasFinished() && !magoSpawned) {
        stateR = DEATH_STATICR;
        magoSpawned = true;

        std::shared_ptr<Entity> entityPtr = Engine::GetInstance().entityManager->CreateEntity(EntityType::MAGOBOSS);

        if (entityPtr) {
            MagoBoss* newMago = static_cast<MagoBoss*>(entityPtr.get());

            newMago->position.setX(this->position.getX() + 2000);
            newMago->position.setY(this->position.getY());

            newMago->SetTextures(
                this->textureIdle,
                this->textureBall,
                this->textureTransformation
            );

            newMago->Start();
        }
        Engine::GetInstance().render->SetZoomSmooth(0.3f, 800.0f);
    }

    ChangeCurrentAnimation();
    Draw(dt);

    return true;
}

void Rey::Draw(float dt)
{
    anims.Update(dt);
    const SDL_Rect& animFrame = anims.GetCurrentFrame();

    if (animFrame.w == 0 || animFrame.h == 0) {
        LOG("CUIDADO: El fotograma de la animacion del Rey mide 0x0. Revisa el TSX y los IDs.");
    }

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
}

void Rey::Die() {
    stateR = DEADR; // Cambia el estado para que dibuje la animación de muerte

   
    // Opcional: Marcar al Rey para ser destruido por el EntityManager si ya no se va a dibujar
    // this->pendingToDelete = true;
}

void Rey::OnCollision(PhysBody* physA, PhysBody* physB) {
    // 2. SOLUCIÓN A LA INTERACCIÓN: Detectamos también los pies del jugador
    if (physB->ctype == ColliderType::PLAYER || physB->ctype == ColliderType::PLAYERFEET) {
        playerInRange = true;
        LOG("JUGADOR EN RANGO - Puedes pulsar la tecla E");
    }
}

void Rey::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
    // Detectamos cuando el jugador se aleja
    if (physB->ctype == ColliderType::PLAYER) {
        playerInRange = false;
    }
}