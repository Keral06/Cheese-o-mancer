#include "HANDMAN.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

HANDMAN::HANDMAN() : NPC(EntityType::HANDMAN)
{
    this->name = name;
    this->texture = texture;
    this->tsxPath = tsxPath;
    pbody = nullptr;
    currentState = H_IDLE;
}


HANDMAN::~HANDMAN() {
    if (pbody != nullptr) {
        Engine::GetInstance().physics->DeletePhysBody(pbody);
        pbody = nullptr;
    }
}

bool HANDMAN::Awake() {
    return true;
}

bool HANDMAN::Start() {
    //Dialogos
    if (storeID == 1) {
        Dialogue firstStime("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_FirstMeeting.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_FirstMeeting.txt");
        this->dialogue = firstStime; //dialogo al descubrirlo por primera vez
        Dialogue dialogueHandman("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_SalesPitch_BeforeBoss.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_SalesPitch_BeforeBoss.txt");
        dialogueHANDMAN = dialogueHandman; //dialogo antes de beat el boss
        Dialogue hasbeensold("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_Buying.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_Buying.txt");
        this->hasBeenSold = hasbeensold; //dialogo has comprado algo
        Dialogue hasNoMoney("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_NoMoney.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_NoMoney.txt");
        this->hasNoMoney = hasNoMoney; //dialogo no tiene dinero
        Dialogue hasBeatBoss("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_SalesPitch_AfterBoss.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_SalesPitch_AfterBoss.txt");
        this->BeatBoss = hasBeatBoss; //dialogo has beat al boss
        Dialogue notbought("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_Leaving_WithoutBuying.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_Leaving_WithoutBuying.txt");
        this->hasNotBought = notbought; //dialogo salir sin haber comprado
        Dialogue yesbought("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_Leaving_AfterBuying.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_Leaving_AfterBuying.txt");
        this->hasBought = yesbought; //dialogo salir habiendo comprado
    }
    else if (storeID == 2) {
        Dialogue firstStime("assets/Dialogues/Hanged_Man_LVL2/HangedMan_FirstInteraction_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL2/HangedMan_FirstInteraction_Names.txt");
        this->dialogue = firstStime; //dialogo al descubrirlo por primera vez en el nivel 2
        Dialogue dialogueHandman("assets/Dialogues/Hanged_Man_LVL2/HangedMan_SalesBeforeBoss_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL2/HangedMan_SalesBeforeBoss_Names.txt");
        dialogueHANDMAN = dialogueHandman; //dialogo antes de beat el boss2
        Dialogue hasbeensold("assets/Dialogues/Hanged_Man_LVL2/Hanged_Man_Dialogues_Buying.txt", "assets/Dialogues/Hanged_Man_LVL2/Hanged_Man_Names_Buying.txt");
        this->hasBeenSold = hasbeensold; //dialogo has comprado algo
        Dialogue hasNoMoney("assets/Dialogues/Hanged_Man_LVL2/HangedMan_NoMoney_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL2/HangedMan_NoMoney_Names.txt");
        this->hasNoMoney = hasNoMoney; //dialogo no tiene dinero
        Dialogue hasBeatBoss("assets/Dialogues/Hanged_Man_LVL2/HangedMan_SalesAfterBoss_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL2/HangedMan_SalesAfterBoss_Names.txt");
        this->BeatBoss = hasBeatBoss; //dialogo has beat al boss
        Dialogue notbought("assets/Dialogues/Hanged_Man_LVL2/HangedMan_LeavingNoBuying_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL2/HangedMan_LeavingNoBuying_Names.txt");
        this->hasNotBought = notbought; //dialogo salir sin haber comprado
        Dialogue yesbought("assets/Dialogues/Hanged_Man_LVL2/HangedMan_LeavingBuying_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL2/HangedMan_LeavingBuying_Names.txt");
        this->hasBought = yesbought; //dialogo salir habiendo comprado
    }
    else if (storeID == 3) {
        Dialogue firstStime("assets/Dialogues/Hanged_Man_LVL3/HangedMan_FirstInteraction_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL3/HangedMan_FirstInteraction_Names.txt");
        this->dialogue = firstStime; //dialogo al descubrirlo por primera vez en el nivel 2
        Dialogue dialogueHandman("assets/Dialogues/Hanged_Man_LVL3/HangedMan_SalesBeforeBoss_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL3/HangedMan_SalesBeforeBoss_Names.txt");
        dialogueHANDMAN = dialogueHandman; //dialogo antes de beat el boss2
        Dialogue hasbeensold("assets/Dialogues/Hanged_Man_LVL2/Hanged_Man_Dialogues_Buying.txt", "assets/Dialogues/Hanged_Man_LVL2/Hanged_Man_Names_Buying.txt");
        this->hasBeenSold = hasbeensold; //dialogo has comprado algo
        Dialogue hasNoMoney("assets/Dialogues/Hanged_Man_LVL3/HangedMan_NoMoney_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL3/HangedMan_NoMoney_Names.txt");
        this->hasNoMoney = hasNoMoney; //dialogo no tiene dinero
        Dialogue hasBeatBoss("assets/Dialogues/Hanged_Man_LVL3/HangedMan_SalesAfterBoss_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL3/HangedMan_SalesAfterBoss_Names.txt");
        this->BeatBoss = hasBeatBoss; //dialogo has beat al boss
        Dialogue notbought("assets/Dialogues/Hanged_Man_LVL3/HangedMan_LeavingNoBuying_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL3/HangedMan_LeavingNoBuying_Names.txt");
        this->hasNotBought = notbought; //dialogo salir sin haber comprado
        Dialogue yesbought("assets/Dialogues/Hanged_Man_LVL3/HangedMan_LeavingBuying_Dialogues.txt", "assets/Dialogues/Hanged_Man_LVL3/HangedMan_LeavingBuying_Names.txt");
        this->hasBought = yesbought; //dialogo salir habiendo comprado
    }

    std::unordered_map<int, std::string> aliases = {
               {0, "idle"},
               {35, "shop_start"},
               {36, "shop_static"},
               {37, "shop_end"}
    };

    anims.LoadFromTSX("assets/Textures/Spritesheets/Hangman/sprite_hangedman_01.tsx", aliases);
    anims.SetCurrent("idle");
    currentState = H_IDLE;

    texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Hangman/sprite_hangedman_01.png");
    InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

    texW = 256;
    texH = 640;


    if (pbody == nullptr) {
        position.setX(xInicial);
        position.setY(yInicial);
        pbody = Engine::GetInstance().physics->CreateRectangleSensor(
            (int)position.getX(),
            (int)position.getY() + 300,
            texW,
            texH + 280,
            bodyType::DYNAMIC
        );
        b2Body_SetGravityScale(pbody->body, 0.0f);
        pbody->listener = this;
        pbody->ctype = ColliderType::MAGICIAN;
    }

    return true;
}

bool HANDMAN::Update(float dt)
{
    if (!active) return true;

    if (currentState == H_SHOP_START && anims.HasFinished()) {
        currentState = H_SHOP_STATIC;
        anims.SetCurrent("shop_static");
    }
    else if (currentState == H_SHOP_END && anims.HasFinished()) {
        currentState = H_IDLE;
        anims.SetCurrent("idle");
    }

	Draw(dt);
    if (isGettingTouched) {
        if (!isStoreOn) {
            Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - 32, (int)position.getY() + (texH / 2) + 96);
        }
        bool bossDefeated = false;
        if (storeID == 1) {
            bossDefeated = Engine::GetInstance().scene->beatBoss;
        }
        else if (storeID == 2) {
            bossDefeated = Engine::GetInstance().scene->beatPrincess;
        }
        else if (storeID == 3) {
            bossDefeated = Engine::GetInstance().scene->beatPrincess;
        }
        if (isWaitingForAnimation) {
            if (anims.HasFinished()) {
                isWaitingForAnimation = false;

                if (pendingDialogue != nullptr) {
                    pendingDialogue->hasEnded = false;
                    pendingDialogue->BeginDialogue(nameNPC);
                    pendingDialogue->Draw(dt);
                }
            }
            return true;
        }
        if (firstTime && dialogue.hasStarted && !dialogue.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
              
                if (dialogue.AvanzarDialogo(dt, nameNPC)) {
                    firstTime = false;
                    if (!bossDefeated) {
                        dialogueHANDMAN.hasEnded = false;
                        dialogueHANDMAN.BeginDialogue(nameNPC);
                        dialogueHANDMAN.Draw(dt);
                    }
                    else {
                        BeatBoss.hasEnded = false;
                        BeatBoss.BeginDialogue(nameNPC);
                        BeatBoss.Draw(dt);
                    }

                    if (this->level == 1) {
                       
                        Engine::GetInstance().scene->cards.push("HangMan", Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_HangMan.png"), nullptr);
                    }
                    return true;
                }
            }
            if (!dialogue.hasEnded) dialogue.Draw(dt);
            return true;
        }

        if (!bossDefeated && dialogueHANDMAN.hasStarted && !dialogueHANDMAN.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
                dialogueHANDMAN.NextDialogue();
            }
            if (!dialogueHANDMAN.hasEnded) dialogueHANDMAN.Draw(dt);
            return true;
        }

        // AQUÍ ES DONDE SE ABRE LA TIENDA CUANDO ACABA DE HABLAR 
        if (bossDefeated && BeatBoss.hasStarted && !BeatBoss.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
               
                if (BeatBoss.AvanzarDialogo(dt)) {
                    /*firstTimeBossKill = false;*/
                    isStoreOn = true;
                    currentState = H_SHOP_START;
                    anims.SetCurrent("shop_start");
                    anims.Resets();

                    moneyPlayer = Engine::GetInstance().scene->score;
                    Engine::GetInstance().scene->SetStore(isStoreOn, storeID);
                    return true;
                }
            }

            if (!BeatBoss.hasEnded) BeatBoss.Draw(dt);
            return true;
        }

        if (hasBeenSold.hasStarted && !hasBeenSold.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
                hasBeenSold.NextDialogue();
            }
            if (!hasBeenSold.hasEnded) hasBeenSold.Draw(dt);
            return true;
        }

        if (hasNoMoney.hasStarted && !hasNoMoney.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
                hasNoMoney.NextDialogue();
            }
            if (!hasNoMoney.hasEnded) hasNoMoney.Draw(dt);
            return true;
        }

        if (hasBought.hasStarted && !hasBought.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
                hasBought.NextDialogue();
            }
            if (!hasBought.hasEnded) hasBought.Draw(dt);
            return true;
        }

        if (hasNotBought.hasStarted && !hasNotBought.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
                hasNotBought.NextDialogue();
            }
            if (!hasNotBought.hasEnded) hasNotBought.Draw(dt);
            return true;
        }

        // CONTROL DE TECLA E PRINCIPAL PARA INICIAR DIÁLOGOS O CERRAR LA TIENDA
        if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
            if (firstTime) {
                pendingDialogue = &dialogue;
            }
            else if (!bossDefeated) {
                pendingDialogue = &dialogueHANDMAN;
            }
            else {
                if (!isStoreOn) {
                    pendingDialogue = &BeatBoss;
                }
                else {
                    // AQUÍ ES DONDE SE CIERRA LA TIENDA VOLUNTARIAMENTE
                    isStoreOn = false;
                    Engine::GetInstance().scene->SetStore(isStoreOn, storeID);

                    // PASAMOS AL ESTADO DE CERRAR TIENDA
                    currentState = H_SHOP_END;
                    anims.SetCurrent("shop_end");
                    anims.Resets();

                    if (Engine::GetInstance().scene->score < moneyPlayer) {
                        pendingDialogue = &hasBought;
                    }
                    else {
                        pendingDialogue = &hasNotBought;
                    }
                }
            }

            // Reproducimos el diálogo solicitado directamente (Ya no bloqueamos con "isWaitingForAnimation")
            if (pendingDialogue != nullptr) {
                pendingDialogue->hasEnded = false;
                pendingDialogue->BeginDialogue(nameNPC);
                pendingDialogue->Draw(dt);
            }
        }
    }

    return true;
}
//		//parlarli i has beat el boss primer cop
//		if (py->beatBoss && firstTimeBossKill && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
//			if (BeatBoss.hasStarted) {
//
//				BeatBoss.NextDialogue();
//				BeatBoss.Draw(dt);
//				return true;
//			}
//			BeatBoss.BeginDialogue();
//			BeatBoss.Draw(dt);
//
//
//			return true;
//			
//		}
//		if (BeatBoss.hasStarted && !BeatBoss.hasEnded) {
//			BeatBoss.Draw(dt);
//			return true;
//
//		}
//		//parlarli (obre store)
//		 if (py->beatBoss && Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
//			isStoreOn = !isStoreOn;
//			if (isStoreOn == true)moneyPlayer = py->score;
//			Engine::GetInstance().scene->SetStore(isStoreOn);
//			if (isStoreOn == false) {
//				if (moneyPlayer == py->score) { //no ha comprat
//					
//
//					if (hasNotBought.hasStarted) {
//
//						hasNotBought.NextDialogue();
//						hasNotBought.Draw(dt);
//						return true;
//					}
//					hasNotBought.BeginDialogue();
//					hasNotBought.Draw(dt);
//
//
//					return true;
//				}
//				else {//h comprat
//					
//
//					if (hasBought.hasStarted) {
//
//						hasBought.NextDialogue();
//						hasBought.Draw(dt);
//						return true;
//					}
//					hasBought.BeginDialogue();
//					hasBought.Draw(dt);
//
//
//					return true;
//				}
//
//			}
//
//
//
//
//		}
//		 if (hasNotBought.hasStarted && !hasNotBought.hasEnded) {
//			 hasNotBought.Draw(dt);
//			 return true;
//
//		 }
//		 if (hasBought.hasStarted && !hasBought.hasEnded) {
//			 hasBought.Draw(dt);
//			 return true;
//
//		 }
//
//		
//
//	}
//
//
//
//
//	return true;
//}
void HANDMAN::Draw(float dt) {

    int x, y;
    pbody->GetPosition(x, y);
    position.setX((float)x);
    position.setY((float)y);

    // Actualizamos SIEMPRE la animación para que se vea moverse en todo momento
    anims.Update(dt);
    const SDL_Rect& frameToDraw = anims.GetCurrentFrame();

    Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2, &frameToDraw);
}

bool HANDMAN::CleanUp()
{
	LOG("Unloading Coin");
	Engine::GetInstance().textures->UnLoad(texture);
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	return true;
}

void HANDMAN::OnCollision(PhysBody* physA, PhysBody* physB) {
	Player* pp = static_cast<Player*>(physB->listener);
	py = pp;
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		isGettingTouched = true;
		break;
	}



}

void HANDMAN::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	isGettingTouched = false;


}

