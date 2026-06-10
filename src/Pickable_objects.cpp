#include "Pickable_objects.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"

Pics::Pics(EntityType entityType) : Entity(entityType)
{
	

	pbody = nullptr;
}

Pics::Pics()
{
	pbody = nullptr;
}

Pics::~Pics() {
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
}

bool Pics::Awake() {
	return true;
}

bool Pics::Start() {

	//here i make them different depending on what it is
	if (AlreadyPicked())
	{
		beenPicked = true;
		active = false;
		return true;
	}
	InteractTexture = Engine::GetInstance().textures->Load("resources/UI/UI_interaction/UI_ Interaction_Indicator1Interact.png");

	//32 sujeto a cambio, el tile del tsx es de 32x32 en el ejemplo, luego hare que sea algo que viene de constructor o algo asi
	texW = 128;
	texH = 128;


	if (pbody == nullptr) {
		position.setX(xInicial);
		position.setY(yInicial);
		pbody = Engine::GetInstance().physics->CreateRectangleSensor(
			(int)position.getX(),
			(int)position.getY(),
			texW / 2,
			texH / 2,
			bodyType::DYNAMIC
		);
		b2Body_SetGravityScale(pbody->body, 0.0f);

		pbody->listener = this;
		pbody->ctype = ColliderType::MAGICIAN;




	}

	if (pbody != nullptr) {
		pbody = nullptr;
		position.setX(xInicial);
		position.setY(yInicial);
		pbody = Engine::GetInstance().physics->CreateRectangleSensor(
			(int)position.getX(),
			(int)position.getY(),
			texW,
			texH,
			bodyType::DYNAMIC
		);
		b2Body_SetGravityScale(pbody->body, 0.0f);

		pbody->listener = this;
		pbody->ctype = ColliderType::NPC;




	}

	return true;
}
void Pics:: ChooseWhoIs() {


	if (name == "Dawn") {

	
		PopUpImage = Engine::GetInstance().textures->Load("assets/UI/UI_Poem/requadre_ajustat/UI_Poem_1_02.png");
		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Carta/Carta1.png"); //placeholder
		BoolOfPlayer = 1;
		 objectDeleteIs = true;

	}
	else if (name == "Day") {

	
		PopUpImage = Engine::GetInstance().textures->Load("assets/UI/UI_Poem/requadre_ajustat/UI_Poem_2_02.png");
		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Carta/Carta2.png"); //placeholder
		BoolOfPlayer = 2;

		 objectDeleteIs = true;


	}
	else if (name == "Dusk") {

		
		PopUpImage = Engine::GetInstance().textures->Load("assets/UI/UI_Poem/requadre_ajustat/UI_Poem_3_02.png");
		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Carta/Carta3.png"); //placeholder
		BoolOfPlayer = 3;

		 objectDeleteIs = true;


	}
	else if (name == "Night") {

		
		PopUpImage = Engine::GetInstance().textures->Load("assets/UI/UI_Poem/requadre_ajustat/UI_Poem_4_02.png");
		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Carta/Carta4.png"); //placeholder

		BoolOfPlayer = 4;
		 objectDeleteIs = true;
		

	}
	else if (name == "Spring") {

		Dialogue paperDialogue("assets/Dialogues/Pickable/SpringWater.txt");
		this->dialogue = paperDialogue;

		BoolOfPlayer = 5;

		 objectDeleteIs = true;

	}
	else if (name == "Horsekin") {

		Dialogue paperDialogue("assets/Dialogues/Pickable/Horsekin.txt");
		this->dialogue = paperDialogue;


		BoolOfPlayer = 6;

		 objectDeleteIs = true;

	}
	else if (name == "TreeRoot") {

		Dialogue paperDialogue("assets/Dialogues/Pickable/TreeRoot.txt");
		this->dialogue = paperDialogue;


		BoolOfPlayer = 7;

		 objectDeleteIs = true;

	}
	else if (name == "Psalm1") {

		Dialogue paperDialogue("assets/Dialogues/Pickable/Psalm1_Dialogues.txt", "assets/Dialogues/Pickable/Psalm1_Names.txt");
		this->dialogue = paperDialogue;

		BoolOfPlayer = 8;

		objectDeleteIs = false;

	}
	else if (name == "Psalm2") {


		Dialogue paperDialogue("assets/Dialogues/Pickable/Psalm2_Dialogues.txt", "assets/Dialogues/Pickable/Psalm2_Names.txt");
		this->dialogue = paperDialogue;


		BoolOfPlayer = 9;

		objectDeleteIs = false;

	}
	else if (name == "Psalm3") {


		Dialogue paperDialogue("assets/Dialogues/Pickable/Psalm3_Dialogues.txt", "assets/Dialogues/Pickable/Psalm3_Names.txt");
		this->dialogue = paperDialogue;


		BoolOfPlayer = 10;

		objectDeleteIs = false;

	}
	else if (name == "Bloody") {

		PopUpImage = Engine::GetInstance().textures->Load("assets/UI/UI_Poem/requadre_ajustat/UI_Poem_4_02.png");
		texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Carta/Carta4.png"); //placeholder


		BoolOfPlayer = 11;

		objectDeleteIs = true;
		 EmpressSpecial = true;

		}
	else if (name == "Rusty") {

			PopUpImage = Engine::GetInstance().textures->Load("assets/UI/UI_Poem/requadre_ajustat/UI_Poem_4_02.png");
			texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Carta/Carta4.png"); //placeholder


			BoolOfPlayer = 12;
			EmpressSpecial = true;
			objectDeleteIs = true;

			}
	else if (name == "Moldy") {

				PopUpImage = Engine::GetInstance().textures->Load("assets/UI/UI_Poem/requadre_ajustat/UI_Poem_4_02.png");
				texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Carta/Carta4.png"); //placeholder


				BoolOfPlayer = 13;
				EmpressSpecial = true;
				objectDeleteIs = true;

				}
	else if (name == "Core") {

					PopUpImage = Engine::GetInstance().textures->Load("assets/UI/UI_Poem/requadre_ajustat/UI_Poem_4_02.png");
					texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Carta/Carta4.png"); //placeholder


					BoolOfPlayer = 14;
					EmpressSpecial = true;
					objectDeleteIs = true;

					}

}
void Pics::CheckBoolOfPlayer() {
	SDL_Texture* help = nullptr;
	switch (BoolOfPlayer) {

	case 0:
		break;
	case 1:
		Engine::GetInstance().scene->dawn = true;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemPoem2_.png");
		Engine::GetInstance().scene->inventario.push("Dawn", help, nullptr);
		if (Engine::GetInstance().scene->dawn == true && Engine::GetInstance().scene->day == true && Engine::GetInstance().scene->dusk == true && Engine::GetInstance().scene->night == true) {
			Engine::GetInstance().scene->hasAllPoems = true;

		}
		break;
	case 2:
		Engine::GetInstance().scene->day = true;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemPoem2_.png");
		Engine::GetInstance().scene->inventario.push("Day", help, nullptr);
		if (Engine::GetInstance().scene->dawn == true && Engine::GetInstance().scene->day == true && Engine::GetInstance().scene->dusk == true && Engine::GetInstance().scene->night == true) {
			Engine::GetInstance().scene->hasAllPoems = true;

		}
		break;
	case 3:
		Engine::GetInstance().scene->dusk = true;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemPoem2_.png");
		Engine::GetInstance().scene->inventario.push("Dusk", help, nullptr);
		if (Engine::GetInstance().scene->dawn == true && Engine::GetInstance().scene->day == true && Engine::GetInstance().scene->dusk == true && Engine::GetInstance().scene->night == true) {
			Engine::GetInstance().scene->hasAllPoems = true;

		}
		break;
	case 4:
		Engine::GetInstance().scene->night = true;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemPoem2_.png");
		Engine::GetInstance().scene->inventario.push("Night", help, nullptr);
		if (Engine::GetInstance().scene->dawn == true && Engine::GetInstance().scene->day == true && Engine::GetInstance().scene->dusk == true && Engine::GetInstance().scene->night == true) {
			Engine::GetInstance().scene->hasAllPoems = true;

		}
		break;
	case 5:
		Engine::GetInstance().scene->springWater = true;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemSacredSpringWater1_.png");
		Engine::GetInstance().scene->inventario.push("SpringWater", help, nullptr);
		break;
	case 6:
		Engine::GetInstance().scene->HorsekinManure = true;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemHorsekinManure1_.png");
		Engine::GetInstance().scene->inventario.push("HorseskinManure", help, nullptr);
		break;
	case 7:
		Engine::GetInstance().scene->Gargantuan = true;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemGargantualTreeRoot1_.png");
		Engine::GetInstance().scene->inventario.push("Gargantuan", help, nullptr);
		break;
	case 8:
		Engine::GetInstance().scene->psalm1 = true;
		if (Engine::GetInstance().scene->psalm1 == true && Engine::GetInstance().scene->psalm2 == true && Engine::GetInstance().scene->psalm3 == true) {
			Engine::GetInstance().scene->hasReadAllPsalms = true;

		}

		break;
	case 9:
		Engine::GetInstance().scene->psalm2 = true;
		if (Engine::GetInstance().scene->psalm1 == true && Engine::GetInstance().scene->psalm2 == true && Engine::GetInstance().scene->psalm3 == true) {
			Engine::GetInstance().scene->hasReadAllPsalms = true;

		}
		break;
	case 10:
		Engine::GetInstance().scene->psalm3 = true;
		if (Engine::GetInstance().scene->psalm1 == true && Engine::GetInstance().scene->psalm2 == true && Engine::GetInstance().scene->psalm3 == true) {
			Engine::GetInstance().scene->hasReadAllPsalms = true;

		}
		break;
		//Artifact
	case 11:
		if (Engine::GetInstance().scene->inventario.tieneObjeto("Bloody"))return;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ArtifactBloody2_.png");
		Engine::GetInstance().scene->inventario.push("Bloody", help, nullptr);
		
		//bloody
		if (Engine::GetInstance().scene->inventario.tieneObjeto("Bloody") && Engine::GetInstance().scene->inventario.tieneObjeto("Rusty") && Engine::GetInstance().scene->inventario.tieneObjeto("Moldy") && Engine::GetInstance().scene->inventario.tieneObjeto("Core")) {
			Engine::GetInstance().scene->hasAllFragments = true;

		}
		break;
	case 12:
		if (Engine::GetInstance().scene->inventario.tieneObjeto("Rusty"))return;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ArtifactRusty2_png");
		Engine::GetInstance().scene->inventario.push("Rusty", help, nullptr); //rusty
		if (Engine::GetInstance().scene->inventario.tieneObjeto("Bloody") && Engine::GetInstance().scene->inventario.tieneObjeto("Rusty") && Engine::GetInstance().scene->inventario.tieneObjeto("Moldy") && Engine::GetInstance().scene->inventario.tieneObjeto("Core")) {
			Engine::GetInstance().scene->hasAllFragments = true;

		}
			break; 
	case 13:
		if (Engine::GetInstance().scene->inventario.tieneObjeto("Moldy"))return;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ArtifactMoldy2.png");
		Engine::GetInstance().scene->inventario.push("Moldy", help, nullptr);
		if (Engine::GetInstance().scene->inventario.tieneObjeto("Bloody") && Engine::GetInstance().scene->inventario.tieneObjeto("Rusty") && Engine::GetInstance().scene->inventario.tieneObjeto("Moldy") && Engine::GetInstance().scene->inventario.tieneObjeto("Core")) {
			Engine::GetInstance().scene->hasAllFragments = true;

		}
				break;
	
	case 14:
		if (Engine::GetInstance().scene->inventario.tieneObjeto("Core"))return;
		help = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ArtifactCore2_.png");
		Engine::GetInstance().scene->inventario.push("Core", help, nullptr);//core
		if (Engine::GetInstance().scene->inventario.tieneObjeto("Bloody") && Engine::GetInstance().scene->inventario.tieneObjeto("Rusty") && Engine::GetInstance().scene->inventario.tieneObjeto("Moldy") && Engine::GetInstance().scene->inventario.tieneObjeto("Core")) {
			Engine::GetInstance().scene->hasAllFragments = true;

		}
					break;

	default: break;



	}


	
}



bool Pics::Update(float dt)
{
	if (!active) return true;
	if (EmpressSpecial) {
	
		if (Engine::GetInstance().scene->EmpressTrustedDialogue == false) { return true; }
	
	
	}
	if (hasPbody ==false) return true;
	if (beenPicked && PopUpOn==false){
		if (pbody != nullptr) {
			Engine::GetInstance().physics->DeletePhysBody(pbody);
			pbody = nullptr;
		}
		hasPbody = false;
		return true;
	}
	if (texture && PopUpOn==false) {
	Draw(dt);
	
	
	}
	if(PopUpOn){
		float w, h;
		SDL_GetTextureSize(PopUpImage, &w, &h);
		Engine::GetInstance().render->DrawTextureNoCamera(PopUpImage, 100, 100, w, h);
	
	}
	if (isGettingTouched) {
		Engine::GetInstance().render->DrawTexture(InteractTexture, (int)position.getX() - texW / 2, (int)position.getY() + texH / 2);



		if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_E) == KEY_DOWN) {

			if (dialogue.dialogue.size() <= 0) {

				if (PopUpOn) {
					if (objectDeleteIs) {

						beenPicked = true;

					}
					PopUpOn = false;
					Engine::GetInstance().scene->ObjectObserved = false;
				}
				else {
					if (objectDeleteIs) {

						beenPicked = true;

					}
					PopUpOn = true;
					Engine::GetInstance().scene->ObjectObserved = true;
				}
				CheckBoolOfPlayer();

				return true;

			}
			if (dialogue.hasStarted) {

				if (dialogue.AvanzarDialogo(dt, nameNPC)) {
					CheckBoolOfPlayer();

				}
			}

			if (dialogue.hasStarted && !dialogue.hasEnded) {
				dialogue.Draw(dt);
				return true;

			}
		}


	}

	return true;
}
void Pics::Draw(float dt) {

	//anims.Update(dt);
	//const SDL_Rect& animFrame = anims.GetCurrentFrame();

	int x, y;
	pbody->GetPosition(x, y);
	position.setX((float)x);
	position.setY((float)y);


	Engine::GetInstance().render->DrawTexture(texture, x - texW / 2, y - texH / 2/*, &animFrame*/);

}

bool Pics::CleanUp()
{
	LOG("Unloading Coin");
	Engine::GetInstance().textures->UnLoad(texture);
	Engine::GetInstance().textures->UnLoad(PopUpImage);
	dialogue.CleanUp();
	Engine::GetInstance().textures->UnLoad(InteractTexture);
	if (pbody != nullptr) {
		Engine::GetInstance().physics->DeletePhysBody(pbody);
		pbody = nullptr;
	}
	active = false;
	return true;
}
void Pics::OnCollision(PhysBody* physA, PhysBody* physB) {

	Player* pp = static_cast<Player*>(physB->listener);
	py = pp;
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		isGettingTouched = true;
		
		break;
	}


}
void Pics::OnCollisionEnd(PhysBody* physA, PhysBody* physB) {
	isGettingTouched = false;



}

bool Pics::AlreadyPicked()
{
	

	if (name == "Dawn") return Engine::GetInstance().scene->dawn;
	if (name == "Day") return Engine::GetInstance().scene->day;
	if (name == "Dusk") return Engine::GetInstance().scene->dusk;
	if (name == "Night") return Engine::GetInstance().scene->night;

	if (name == "Spring") return Engine::GetInstance().scene->springWater;
	if (name == "Horsekin") return Engine::GetInstance().scene->HorsekinManure;
	if (name == "TreeRoot") return Engine::GetInstance().scene->Gargantuan;

	if (name == "Bloody" || name == "Rusty" || name == "Moldy" || name == "Core") {
	
		if (Engine::GetInstance().scene->inventario.tieneObjeto("Artifact"))return true;
	
	}
	if (name == "Bloody") return Engine::GetInstance().scene->inventario.tieneObjeto("Bloody");
	if (name == "Rusty") return Engine::GetInstance().scene->inventario.tieneObjeto("Rusty");
	if (name == "Moldy") return Engine::GetInstance().scene->inventario.tieneObjeto("Moldy");
	if (name == "Core") return Engine::GetInstance().scene->inventario.tieneObjeto("Core");

	return false;
}