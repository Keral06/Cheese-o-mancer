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
	Dialogue firstStime("resources/Dialogues/HangedMan/Hanged_Man_Dialogues_FirstMeeting.txt", "resources/Dialogues/HangedMan/Hanged_Man_Names_FirstMeeting.txt");
	this->dialogue = firstStime; //dialogo al descubrirlo por primera vez
	pbody = nullptr;
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


    std::unordered_map<int, std::string> aliases = {
           {70, "selling"}
    };
    anims.LoadFromTSX("assets/Textures/Spritesheets/Hangman/sprite_hangedman_01.tsx", aliases);
    /*coinPickupFx = Engine::GetInstance().audio->LoadFx("Assets/Audio/Fx/PREV/coin-collision-sound-342335.wav");*/
    anims.SetCurrent("idle");

    texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Hangman/sprite_hangedman_01.png");
    InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Talk.png");

    //32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
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
    //Poner dialogo dependiendo del nivel
    return true;
}

bool HANDMAN::Update(float dt)
{
	if (!active) return true;

	Draw(dt);
    if (isGettingTouched) {
        if (!isStoreOn) {
            Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - 32, (int)position.getY() + (texH / 2) + 96);
        }
        if (isWaitingForAnimation) {
            if (anims.HasFinished()) {
                isWaitingForAnimation = false;

                if (pendingDialogue != nullptr) {
                    pendingDialogue->hasEnded = false;
                    pendingDialogue->BeginDialogue();
                    pendingDialogue->Draw(dt);
                }
            }
            return true;
        }
        if (firstTime && dialogue.hasStarted && !dialogue.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
                dialogue.NextDialogue();
                if (dialogue.hasEnded) {
                    firstTime = false;
                    dialogueHANDMAN.hasEnded = false;
                    dialogueHANDMAN.BeginDialogue();
                    dialogueHANDMAN.Draw(dt);
                    if (this->level == 1) {
                        SDL_Texture* HangedMan = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_TarotCard_HangMan.png");
                        Engine::GetInstance().scene->TarotCards.push_back(HangedMan);
                    }
                    return true;
                }
            }
            if (!dialogue.hasEnded) dialogue.Draw(dt);
            return true;
        }

        if (!py->beatBoss && dialogueHANDMAN.hasStarted && !dialogueHANDMAN.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
                dialogueHANDMAN.NextDialogue();
            }
            if (!dialogueHANDMAN.hasEnded) dialogueHANDMAN.Draw(dt);
            return true;
        }

        if (py->beatBoss && BeatBoss.hasStarted && !BeatBoss.hasEnded) {
            if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
                BeatBoss.NextDialogue();
                if (BeatBoss.hasEnded) {
                    firstTimeBossKill = false;
                    isStoreOn = true;
                    moneyPlayer = py->score;
                    Engine::GetInstance().scene->SetStore(isStoreOn);
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

        if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {
            bool necesitaAnimacion = false;
            if (!py->beatBoss) {
                if (firstTime) {
                    pendingDialogue = &dialogue;
                    necesitaAnimacion = true;
                }
                else {
                    pendingDialogue = &dialogueHANDMAN;
                    necesitaAnimacion = true;
                }
            }
            else {
                if (firstTimeBossKill) {
                    pendingDialogue = &BeatBoss;
                    necesitaAnimacion = true;
                }
                else {
                    isStoreOn = !isStoreOn;
                    Engine::GetInstance().scene->SetStore(isStoreOn);

                    if (isStoreOn) {
                        moneyPlayer = py->score;
                        pendingDialogue = nullptr;
                        necesitaAnimacion = true;
                    }
                    else {
                        if (py->score < moneyPlayer) {
                            pendingDialogue = &hasBought;
                        }
                        else {
                            pendingDialogue = &hasNotBought;
                        }
                        necesitaAnimacion = false;
                    }
                }
            }

            if (pendingDialogue != nullptr) {
                if (necesitaAnimacion) {
                    anims.SetCurrent("selling");
                    anims.Resets();
                    isWaitingForAnimation = true;
                }
                else {
                    pendingDialogue->hasEnded = false;
                    pendingDialogue->BeginDialogue();
                    pendingDialogue->Draw(dt);
                }
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

    SDL_Rect frameToDraw;

    if (isWaitingForAnimation) {
        anims.Update(dt);
        frameToDraw = anims.GetCurrentFrame();
    }
    else {
        frameToDraw = { 0, 1280, texW, texH };
    }

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

