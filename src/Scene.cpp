#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "Item.h"
#include "Enemy.h"
#include "EnemigoVolador.h"
#include "UIManager.h"
#include "UISlider.h"
#include <fstream>
#include "Physics.h"
#include "FINALBOSS.h"
#include "Handman.h"
#include "ParticleSystem.h"
#include "ParticleExample.h"
#include "BossFightPrincessKnight.h"

Scene::Scene() : Module()
{
	name = "scene";
	currentScene = SceneID::INTRO_SCREEN;
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	LoadGame();
	//LoadScene(currentScene); 
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	srand(time(NULL));
	LoadScene(currentScene);

	bossFightController = new BossFightPrincessKnight();
	bossFightController->Start();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{

	return true;
}


// Called each loop iteration
bool Scene::Update(float dt)
{
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		showUIDebug = !showUIDebug; 
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_N) == KEY_DOWN) {
		list = !list;
		SetMissionUI();
		if (inventoryOn) {


			SetInventory(!inventoryOn);

		}
	}
	
	switch (currentScene)
	{
	case SceneID::INTRO_SCREEN:
		UpdateIntro(dt); 
		break;
	case SceneID::MAIN_MENU:
		if (introTexture != nullptr) {
			SDL_Renderer* renderer = Engine::GetInstance().render->renderer;
			SDL_RenderTexture(renderer, introTexture, NULL, NULL);
		}
		UpdateMainMenu(dt);
		if (slidersOn) {
			SDL_Rect VolumeRect = { 520,560, 200, 50 };
			Engine::GetInstance().render->DrawTextureNoCamera(Volume, VolumeRect.x, VolumeRect.y, VolumeRect.w, VolumeRect.h);
			SDL_Rect Fullscreen = { 600,420, 200, 50 };
			Engine::GetInstance().render->DrawTextureNoCamera(fullscreen, Fullscreen.x, Fullscreen.y, Fullscreen.w, Fullscreen.h);

			SDL_Rect EffectsRect = { 520,490, 200, 50 };
			Engine::GetInstance().render->DrawTextureNoCamera(VolumeEffects, EffectsRect.x, EffectsRect.y, EffectsRect.w, EffectsRect.h);





		}
		
		break;
	case SceneID::IN_GAME:
		UpdateLevel(dt);
		SetInventariIcon(true);
		
		break;
	case SceneID::GAME_OVER:
		UpdateGameOver(dt);
		break;
	case SceneID::WIN_SCREEN:
		UpdateWinScreen(dt);
		break;
	case SceneID::FINAL_WIN:
		UpdateFinalWin(dt);
		break;
	}
	if (isPaused) {
		UpdatePauseMenu();
	}

	if (isFading)
	{
		fadeTime += dt / 1000.0f;

		float t = fadeTime / fadeDuration;
		if (t > 1.0f) t = 1.0f;

		
		t = t * t * (3 - 2 * t);
		if (t > 1.0f) t = 1.0f;

		if (fadeIn)
			fadeAlpha = (Uint8)((1.0f - t) * 255);
		else
			fadeAlpha = (Uint8)(t * 255);

		if (t >= 1.0f)
		{
			isFading = false;

			
			if (!fadeIn)
			{
				
			}
		}
	}

	if (!fadeIn && !isFading)
	{
		LoadMap(nextMap);

		StartFadeIn(1.0f);
	}

	if (bossFightController != nullptr)
	{
		bossFightController->Update(dt);
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;
	if (isPaused) {
		UpdatePauseMenu();
	}
	if (misiones.Visualizada() == false && !someoneIsTalking) {
	
	
		
		for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
			if (element->id == 68) {
				element->ChangeImage(misionesnoti, misionesnoticlicado);
			}
		}

	
	
	}
	else {
	
		for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
			if (element->id == 68) {
				element->ChangeImage(misionesicono, misionesiconoclicado);
			}
		}
	}
	switch (currentScene)
	{
	case SceneID::INTRO_SCREEN:
		if (introTexture != nullptr) {
			SDL_RenderTexture(Engine::GetInstance().render->renderer, introTexture, NULL, NULL);
		}
		if ((SDL_GetTicks() / 500) % 2 == 0) { 
			Engine::GetInstance().render->DrawText("PRESS SPACE TO PLAY", 500, 600, 0, 0, { 255, 255, 255, 255 });
		}
	
		break;
	case SceneID::MAIN_MENU:
		if (introTexture != nullptr) {
			SDL_RenderTexture(Engine::GetInstance().render->renderer, introTexture, NULL, NULL);
		}

		if (showCredits) {
			SDL_Rect bg = { 0, 0, 1280, 720 };
			Engine::GetInstance().render->DrawRectangle(bg, 0, 0, 0, 200, true, false);

			Engine::GetInstance().render->DrawText("CREDITS", 550, 150, 0, 0, { 255, 215, 0, 255 });
			Engine::GetInstance().render->DrawText("Irene & Queralt", 550, 250, 0, 0, { 255, 255, 255, 255 });
		}
		if (slidersOn) {

			
			
			SDL_Rect sliderRect = { 520,500, 200, 30 };
			
			SDL_Rect sliderRect2 = { 520,450, 200, 30 };
			

			
		
			
			Engine::GetInstance().render->DrawTextureNoCamera(Volume, 520, 460, 200, 50);
			Engine::GetInstance().render->DrawTextureNoCamera(fullscreen, 520, 360, 200, 50);
			Engine::GetInstance().render->DrawTextureNoCamera(VolumeEffects, 520, 420, 200, 50);



		}
		Engine::GetInstance().uiManager->PostUpdate();
		
		break;

	case SceneID::IN_GAME:
		PostUpdateLevel();
		SaveLevel();
		break;
	
	case SceneID::GAME_OVER:
		if (loseTexture != nullptr) {
			SDL_FRect destRect = { 0, 0, 1280, 720 };

			SDL_RenderTexture(Engine::GetInstance().render->renderer, loseTexture, NULL, &destRect);
		}/*
		Engine::GetInstance().render->DrawText("YOU LOST", 440, 100, 400, 100, { 255, 0, 0, 255 });*/

		Engine::GetInstance().uiManager->PostUpdate();
		break;
	case SceneID::WIN_SCREEN:
		if (loseTexture != nullptr) {
			SDL_Rect sourceRect = loseAnimSet.GetCurrentFrame();
			if (sourceRect.w > 0) {
				SDL_FRect srcFRect = { (float)sourceRect.x, (float)sourceRect.y, (float)sourceRect.w, (float)sourceRect.h };
				SDL_RenderTexture(Engine::GetInstance().render->renderer, loseTexture, &srcFRect, NULL);
			}
		}

		Engine::GetInstance().render->DrawText("LEVEL COMPLETED!", 400, 100, 500, 100, { 0, 255, 0, 255 }); 
		{
			std::string scoreStr = "Score: " + std::to_string(Engine::GetInstance().scene->score);
			Engine::GetInstance().render->DrawText(scoreStr.c_str(), 540, 250, 0, 0, { 255, 255, 255, 255 });
		}

		{
			std::string timeStr = "Time: " + std::to_string((int)levelTimer) + "s";
			Engine::GetInstance().render->DrawText(timeStr.c_str(), 570, 300, 0, 0, { 255, 255, 255, 255 });
		}

		Engine::GetInstance().uiManager->PostUpdate();
		break;
	case SceneID::FINAL_WIN:
		if (loseTexture != nullptr) {
			SDL_Rect sourceRect = loseAnimSet.GetCurrentFrame();
			if (sourceRect.w > 0) {
				SDL_FRect srcFRect = { (float)sourceRect.x, (float)sourceRect.y, (float)sourceRect.w, (float)sourceRect.h };
				SDL_RenderTexture(Engine::GetInstance().render->renderer, loseTexture, &srcFRect, NULL);
			}
		}
		Engine::GetInstance().render->DrawText("CONGRATULATIONS!", 380, 100, 600, 100, { 255, 215, 0, 255 });

		{
			std::string scoreStr = "Final Score: " + std::to_string(Engine::GetInstance().scene->score);
			Engine::GetInstance().render->DrawText(scoreStr.c_str(), 500, 250, 0, 0, { 255, 255, 255, 255 });
		}

		{
			std::string timeStr = "Total Time: " + std::to_string((int)levelTimer) + "s";
			Engine::GetInstance().render->DrawText(timeStr.c_str(), 530, 300, 0, 0, { 255, 255, 255, 255 });
		}

		Engine::GetInstance().uiManager->PostUpdate();
		break;
	default:
		break;
	}
	
	if (showUIDebug) {
		for (const auto& ui : Engine::GetInstance().uiManager->UIElementsList) {

			if (!ui->visible) continue; 

			Uint8 r = 255, g = 0, b = 255;

			switch (ui->state) {
			case UIElementState::NORMAL:   r = 255; g = 0; b = 255;   break; // Magenta
			case UIElementState::FOCUSED:  r = 0; g = 255; b = 255;   break; // Cyan
			case UIElementState::PRESSED:  r = 255; g = 255; b = 0;   break; // Amarillo
			case UIElementState::SELECTED: r = 0; g = 255; b = 0;     break; // Verde
			case UIElementState::DISABLED: r = 255; g = 128; b = 0;   break; // Naranja
			}
			SDL_Rect box = ui->bounds;

			Engine::GetInstance().render->DrawRectangle(box, r, g, b, 255, false, false);

			SDL_Rect boxInner = { box.x + 1, box.y + 1, box.w - 2, box.h - 2 };
			Engine::GetInstance().render->DrawRectangle(boxInner, r, g, b, 255, false, false);

			SDL_Rect boxOuter = { box.x - 1, box.y - 1, box.w + 2, box.h + 2 };
			Engine::GetInstance().render->DrawRectangle(boxOuter, r, g, b, 255, false, false);
			SDL_Rect boxOuter2 = { box.x - 1, box.y - 1, box.w + 3, box.h + 3 };
			Engine::GetInstance().render->DrawRectangle(boxOuter2, r, g, b, 255, false, false);
		
		}
	}

	if (fadeAlpha > 0)
	{
		SDL_Rect screen = { 0, 0, 1280 / 0.3, 720 / 0.3 };
		Engine::GetInstance().render->DrawRectangle(
			screen,
			0, 0, 0,
			fadeAlpha,
			true,
			false
		);
	}

	if (exitGame) return false;

	return ret;
}

void Scene::LoadGame()
{
	std::ifstream file("Assets/savegame.txt");
	if (!file.is_open()) {
		savedLevel = 1; // default value
		return;
	}

	file >> savedLevel;
	file.close();
}
bool Scene::OnUIMouseClickEvent(UIElement* uiElement)
{

	if (uiElement->id == 67) {
		
		SetInventory(!inventoryOn);
		if (list) {
		
			list = !list;
			SetMissionUI();
		
		}if (cardsInventoryOn) {
		
			cardsInventoryOn = !cardsInventoryOn;
			SetTarotUI(cardsInventoryOn);
		
		
		}
	
		return true;
	}if (uiElement->id == 68) {
		list = !list;
		SetMissionUI();
		if (inventoryOn) {
		
		
			SetInventory(!inventoryOn);
		
		}
		if (cardsInventoryOn) {

			cardsInventoryOn = !cardsInventoryOn;
			SetTarotUI(cardsInventoryOn);


		}
		return true;
	}
	if (uiElement->id == 69) {
		cardsInventoryOn = !cardsInventoryOn;

		SetTarotUI(cardsInventoryOn);
		if (inventoryOn) {


			SetInventory(!inventoryOn);

		}
		if (list) {
		
			list = !list;
			SetMissionUI();
		
		
		
		}
		return true;
	}
	if (uiElement->id == 45) {
		cardsInventoryOn = !cardsInventoryOn;
		SetTarotUI(cardsInventoryOn);
		return true;
	}

	if ((uiElement->id >= 60 && uiElement->id <= 64)) {
		HandleTeleportUIEvents(uiElement);
		return true;
	
	}
	if (uiElement->id == 46) { 
		currentCardIndex++;
		if (currentCardIndex >= cards.cards.size()) currentCardIndex = 0;
		Engine::GetInstance().render->DrawTextureNoCamera(cards.cards[currentCardIndex].imagen, 730, 162, 200, 400);
		return true;
	}
	if (uiElement->id == 47) { 
		currentCardIndex--;
		if (currentCardIndex < 0) currentCardIndex = cards.cards.size() -1;
		Engine::GetInstance().render->DrawTextureNoCamera(cards.cards[currentCardIndex].imagen, 730, 162, 200, 400);
		return true;
	}
	if (uiElement->id == 48) {
		if (currentInvPage > 0) currentInvPage--;
		return true;
	}
	if (uiElement->id == 49) {
		currentInvPage++;
		return true;
	}
	if ((uiElement->id >= 20 && uiElement->id <= 30)|| uiElement->id ==52) {
		HandlePauseUIEvents(uiElement);
		return true;
	}
	if(uiElement->id>=35 && uiElement->id <= 51) {
		HandleStoreUIEvents(uiElement);
		return true;
	}

	//HANDLE MISSION UI 65,66
	if (uiElement->id == 65) {
		currentMission++;
		if (currentMission >= misiones.objetos.size()) currentMission = 0;
		return true;
	}
	if (uiElement->id == 66) {
		currentMission--;
		if (currentMission < 0) currentMission = misiones.objetos.size() - 1;
		return true;
	}


	switch (currentScene)
	{
	case SceneID::INTRO_SCREEN:
		break;
	case SceneID::MAIN_MENU:
		HandleMainMenuUIEvents(uiElement);
		
		break;
	case SceneID::GAME_OVER: 
		HandleGameOverUIEvents(uiElement);
		break;
	case SceneID::WIN_SCREEN:
		
		break;
	case SceneID::FINAL_WIN:
		HandleFinalWinUIEvents(uiElement);
		break;
	default:
		break;
	}

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	if (bossFightController != nullptr)
	{
		delete bossFightController;
		bossFightController = nullptr;
	}
	return true;

	UnloadCurrentScene();
	return true;
}

Vector2D Scene::GetPlayerPosition()
{
	if (player) return player->GetPosition();
	else return Vector2D(0, 0);
}


// *********************************************
// Scene change functions
// *********************************************

void Scene::LoadScene(SceneID newScene)
{
	auto& engine = Engine::GetInstance();
	
	switch (newScene)
	{
	case SceneID::INTRO_SCREEN:
		LoadIntro();
		break;
	case SceneID::MAIN_MENU:
		LoadMainMenu();
		CreateStoreLevel1();
		break;

	case SceneID::IN_GAME:
		

		if (lastscene != SceneID::IN_GAME)
		{
			firstMapLoad = true;
		}

		if (continueGame)
		{
			std::ifstream file("Assets/savegame.txt");
			std::string savedMap = "TEST_map_LV1_startRoom_01.tmx"; 
			if (file.is_open()) {
				file >> savedMap;
				file.close();
			}
			LoadMap(savedMap); 
		}
		else {
			LoadMap("TEST_map_LV1_startRoom_01.tmx");
		}
		break;

	case SceneID::GAME_OVER:
		LoadGameOver();
		break;

	case SceneID::WIN_SCREEN:
		LoadWinScreen();
		break;
	case SceneID::FINAL_WIN:
		LoadFinalWin();
		break;
	}
}

void Scene::ChangeScene(SceneID newScene)
{
	lastscene = currentScene;
	/*if (lastscene != SceneID::MAIN_MENU) {
	
	Player lastPlayerState = *player;
	UnloadCurrentScene();
	currentScene = newScene;
	LoadScene(currentScene);
	*player = lastPlayerState;
	return;
	
	}*/
	UnloadCurrentScene();
	currentScene = newScene;
	LoadScene(currentScene);
	
}

void Scene::UnloadCurrentScene() {

	switch (currentScene)
	{
	case SceneID::INTRO_SCREEN:
		UnloadIntro();
		break;
	case SceneID::MAIN_MENU:
		UnloadMainMenu();
		break;
	case SceneID::IN_GAME:      
		UnloadLevel();          
		break;
	case SceneID::GAME_OVER:
		UnloadGameOver();
		break;
	case SceneID::WIN_SCREEN:
		UnloadWinScreen();
		break;
	case SceneID::FINAL_WIN:
		UnloadFinalWin();
		break;
	}

}

// *********************************************
// MAIN MENU functions
// *********************************************

void Scene::LoadMainMenu() {

	Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/MAIN_menu.wav");
	introTexture = Engine::GetInstance().textures->Load("assets/UI/MainMenu/UI_Start_Background_01.png");
	// Instantiate a UIButton in the Scene
	
	//Botón START
	SDL_Rect btPos = { 520, 370, 200, 50 };
	SDL_Texture* buttonStartPressed;
	SDL_Texture* buttonStartNormal;
	buttonStartPressed = Engine::GetInstance().textures->Load("assets/UI/MainMenu/UI_Start_ButtonStart2_02.png");
	buttonStartNormal = Engine::GetInstance().textures->Load("assets/UI/MainMenu/UI_Start_ButtonStart1_02.png");
	Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 1, "START", btPos, this, SDL_Rect{0,0,0,0}, buttonStartNormal, buttonStartPressed);

	
	////Botón CONTINUE
	
	SDL_Rect continuePos = { 520, 420, 200, 50 };
	SDL_Texture* buttonContinuePressed;
	SDL_Texture *buttonContinueNormal;
	buttonContinueNormal = Engine::GetInstance().textures->Load("assets/UI/MainMenu/UI_Start_ButtonContinue1_01.png");
	buttonContinuePressed = Engine::GetInstance().textures->Load("assets/UI/MainMenu/UI_Start_ButtonContinue2_01.png");
	
	Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 5, "CONTINUE", continuePos, this, SDL_Rect{0,0,0,0}, buttonContinueNormal, buttonContinuePressed);

	// Botón OPTIONS
	SDL_Rect optionsBtnRect = { 520, 470, 200, 50 };
	SDL_Texture* buttonOptionsPressed;
	SDL_Texture* buttonOptionsNormal;
	buttonOptionsPressed = Engine::GetInstance().textures->Load("assets/UI/MainMenu/UI_Start_ButtonSettings2_01.png");
	buttonOptionsNormal = Engine::GetInstance().textures->Load("assets/UI/MainMenu/UI_Start_ButtonSettings1_01.png");
	Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 3, "OPTIONS", optionsBtnRect, this, SDL_Rect{0,0,0,0}, buttonOptionsNormal, buttonOptionsPressed);

	// Slider VOLUMEN
	SDL_Texture* SliderBar;
	SDL_Texture* thumbPressed;
	SDL_Texture* thumbNormal;
	SliderBar = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_SliderBar_01.png");
	thumbPressed = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_SliderButton_01.png");
	thumbNormal = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_Checkbox_01.png");

	SDL_Rect sliderRect = { 520,500, 200, 30 };
	auto slider = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, 2, "VOLUME MUSIC", sliderRect, this, SDL_Rect{ 0,0,0,0 }, SliderBar, thumbNormal, thumbPressed);
	if (slider) slider->visible = false;  
	// Slider VOLUMEN EFECTOS
	SDL_Rect sliderRect2 = { 520,450, 200, 30 };
	auto slider2 = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, 11, "VOLUME EFFECTS", sliderRect2, this, SDL_Rect{ 0,0,0,0 }, SliderBar, thumbNormal, thumbPressed);
	if (slider2) slider2->visible = false;

	//Checkbox de la fullscreen
	SDL_Texture* buttonPressed;
	SDL_Texture* buttonNormal;
	buttonPressed = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_CheckboxPressed_01.png");
	buttonNormal = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_Checkbox_01.png");
	SDL_Rect Fullscreen = { 600,390, 35, 35 };
	auto fullscreen = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::CHECKBOX, 12, "FULL SCREEN", Fullscreen, this, SDL_Rect{0,0,0,0}, buttonPressed, buttonNormal);
	if (fullscreen) fullscreen->visible = false;

	// Botón BACK 
	SDL_Texture* buttonBackPressed;
	SDL_Texture* buttonBackNormal;
	buttonBackPressed = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextBackPressed_01.png");
	buttonBackNormal = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextBack1_01.png");
	SDL_Rect backBtnRect = { 520, 530, 200, 50 };
	auto backBtn = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 4, "BACK", backBtnRect, this, SDL_Rect{0,0,0,0}, buttonBackNormal,buttonBackPressed);
	if (backBtn) backBtn->visible = false; 

	//Botón EXIT
	SDL_Texture* buttonExitPressed;
	SDL_Texture* buttonExitNormal;
	buttonExitPressed = Engine::GetInstance().textures->Load("assets/UI/MainMenu/UI_Start_ButtonExitPressed_01.png");
	buttonExitNormal = Engine::GetInstance().textures->Load("assets/UI/MainMenu/UI_Start_ButtonExit1_01.png");
	SDL_Rect exitPosRect = { 520, 560, 200, 50 };
	auto exitPos = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 6, "EXIT", exitPosRect, this, SDL_Rect{0,0,0,0}, buttonExitNormal, buttonExitPressed);

	////Botón CREDITS
	//SDL_Rect creditsPosRect = { 520, 490, 200, 50 };
	//auto creditsPos = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 7, "CREDITS", creditsPosRect, this);

	////Botón BACK CREDITS
	//SDL_Rect backCreditPosRect = { 520, 560, 200, 50 };
	//auto backCreditsBtn = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 8, "BACK", backCreditPosRect, this);
	//if (backCreditsBtn) backCreditsBtn->visible = false;
	this->Volume = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextVolume2.png");
	this->VolumeEffects = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextEffectsVolume2_01.png");
	this->fullscreen = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextFullScreen2_01.png");
}

void Scene::UnloadMainMenu() {
	// Clean up UI elements related to the main menu
	Engine::GetInstance().textures->UnLoad(introTexture);
	introTexture = nullptr;
	Engine::GetInstance().textures->UnLoad(Volume);
	Volume = nullptr;
	Engine::GetInstance().textures->UnLoad(VolumeEffects);
	VolumeEffects = nullptr;
	Engine::GetInstance().textures->UnLoad(fullscreen);
	fullscreen = nullptr;
	Engine::GetInstance().uiManager->CleanUp();
}

void Scene::UpdateMainMenu(float dt) {
	


}

void Scene::HandleMainMenuUIEvents(UIElement* uiElement)
{
	switch (uiElement->id)
	{
	case 1:
		LOG("Main Menu: MyButton clicked!");
		continueGame = false;
		ChangeScene(SceneID::IN_GAME);
		
		break;
	case 2: 
		if (uiElement->type == UIElementType::SLIDER) {
			if (uiElement->id==2) {
			
			UISlider* slider = static_cast<UISlider*>(uiElement);
			float vol = slider->GetValue();
			Engine::GetInstance().audio->SetMusicVol(vol);
		
			
			
			}
		}
		break;

	case 3: 
		for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
				element->visible = false;
			
			if (element->id == 2 || element->id == 4 || element->id==11 || element->id==12) {
				element->visible = true;
			}
		}
		slidersOn = true;
		break;

	case 4: 
		for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
			element->visible = false; 
			if (element->id == 1 || element->id == 3 || element->id == 5 || element->id == 6 || element->id == 7) {
				element->visible = true;
			}
		}
		slidersOn = false;
	
		break;
	case 5:
		continueGame = true;

		ChangeScene(SceneID::IN_GAME);

		break;
	case 6: 
		exitGame = true; 
		break;
	case 7: 
		showCredits = true;
		for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
			element->visible = (element->id == 8);
		}
		break;
	case 8:
		showCredits = false;
		for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
			if (element->id == 1 || element->id == 3 || element->id == 5 || element->id == 6 || element->id == 7) {
				element->visible = true;
			}
			if (element->id == 8) {
				element->visible = false;
			}
		}
		break;
	case 11:
		if (uiElement->type == UIElementType::SLIDER) {
			if (uiElement->id == 11) {
				UISlider* slider = static_cast<UISlider*>(uiElement);
				float vol = slider->GetValue();
				Engine::GetInstance().audio->SetSFXVol(vol);
			}
		}
		break;
	case 12:
		if(uiElement->state== UIElementState::SELECTED) {
			Engine::GetInstance().window->SetFullscreen(true);
		}
		else {
			Engine::GetInstance().window->SetFullscreen(false);
		}

		
		break;
	}
}

// *********************************************
// Level 1 functions
// *********************************************


void Scene::UpdateLevel(float dt) {
	//DIbujar mapa	

	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		SetPause(!isPaused);
	}
	

	// Lógica de Ayuda (H)
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_H) == KEY_DOWN)
	{
		showHelp = !showHelp;
	}
	//INVENTARIO
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		SetInventory(!inventoryOn);
		if (list) {

			list = !list;
			SetMissionUI();

		}
		
	}
	//DEBUG INVENTARIO
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_G) == KEY_DOWN) {
		if (player != nullptr) {

			inventario.push("Spring", iconSpring, nullptr);
			inventario.push("HorseMacure", iconHorseMacure, nullptr);
			inventario.push("Gargantuan", iconGargantuan, nullptr);
			inventario.push("Map", iconMap, nullptr);
			inventario.push("Lamp", iconLamp, nullptr);
			inventario.push("Key", iconKey, nullptr);

		}
	}
	
	if (inventoryOn && player != nullptr) {
		int totalItems = inventario.objetos.size();

		for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
			if (el->id == 48) { 
				el->visible = (currentInvPage > 0);
			}
			if (el->id == 49) { 
				el->visible = ((currentInvPage + 1) * 3 < totalItems);
			}
		}
	}

	if (inventoryOn && inventario.tieneObjeto("Map")) {
		if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {
			Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
			int mx = mousePos.getX();
			int my = mousePos.getY();
			if (mx >= rectInvMap.x && mx <= rectInvMap.x + rectInvMap.w &&
				my >= rectInvMap.y && my <= rectInvMap.y + rectInvMap.h) {
				showMap = !showMap;
			}
		}
	}
	if (cardsInventoryOn) {
		if (cardsBase != nullptr) {
			Engine::GetInstance().render->DrawTextureNoCamera(cardsBase, 240, 60, 800, 600);
		}

		if (cards.cards.size() > 0) {


			Engine::GetInstance().render->DrawTextureNoCamera(cards.cards[currentCardIndex].imagen, 730, 162, 200, 400);
		}


	}
	if (isPaused||showHelp||inventoryOn) {
		return;
	}
	//logica de mapa
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_M) == KEY_DOWN && inventario.tieneObjeto("Map"))
	{
		showMap = !showMap;
	}

	if (player && !player->isDead()) {
		levelTimer += dt / 1000.0f;
	}

	if (player && Engine::GetInstance().scene->lives <= 0) {
		ChangeScene(SceneID::GAME_OVER);
		return;
	}

	//Lógica de cambio de mapa (F1-F4)
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		LoadMap("TEST_map_LV1_startRoom_01.tmx"); 
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		LoadMap("TEST_map_LV1_towerCenter_01.tmx");          
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		LoadMap("TEST_map_LV1_pantryRoom_01.tmx");           
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN) {
		LoadMap("TEST_map_LV1_tortureRoom_02.tmx");
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
		LoadMap("TEST_map_LV1_bossRoom_01.tmx");
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_J) == KEY_DOWN) {
		LoadMap("Map_LV2_bossTower.tmx");
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) {
		LoadMap("Map_LV3_left_01.tmx");
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_O) == KEY_DOWN) {
		LoadMap("Map_LV3_temple_01.tmx");
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_V) == KEY_DOWN) {
		LoadMap("Map_LV3_right_02.tmx");
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN) {
		LoadMap("Map_LV2_towersBotanica_02.tmx");
	}


	// Lógica de Checkpoints (Teclas 1-9)
	for (int i = 0; i < 9; ++i) {
		if (Engine::GetInstance().input->GetKey((SDL_Scancode)(SDL_SCANCODE_1 + i)) == KEY_DOWN) {
			if (i < Engine::GetInstance().map->checkpoints.size()) {
				Vector2D checkpointPos = Engine::GetInstance().map->checkpoints[i]->position;
				checkpointPos.setY(checkpointPos.getY() - 150);
				if (player) player->SetPosition(checkpointPos);
			}
		}
	}
	

	for (const auto& checkpoint : Engine::GetInstance().map->checkpoints) {
		if (checkpoint->name == "end" && checkpoint->isActivated) {
			ChangeScene(SceneID::WIN_SCREEN);
			return;
		}
	}

	// Lógica de gestión de enemigos muertos
	if (player && player->isDead()) {
		for (auto it = enemies.begin(); it != enemies.end(); ) {
			if ((*it)->toDelete) { // si se tiene que borrar la destruye
				it = enemies.erase(it);
			}
			/*else {
				(*it)->Reset();
				++it;
			}*/
		}
	}

	//DEBUG TELEPORT
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_T) == KEY_DOWN && !inventoryOn) {
		SetTeleport(!teleportOn);
	}
	
}

void Scene::UnloadLevel() {

	// Clean up UI elements
	auto& uiManager = Engine::GetInstance().uiManager;
	uiManager->CleanUp();

	// Reset player reference
	player.reset();

	// Clear enemies list
	enemies.clear();
	heartTexture = nullptr;
	invPaperCombined = nullptr;
	iconMap = nullptr;
	iconKey = nullptr;
	iconLamp = nullptr;
	iconSpring = nullptr;
	iconHorseMacure = nullptr;
	iconGargantuan = nullptr;
	inventoryBag = nullptr;
	uiCoin = nullptr;
	cardsBase = nullptr;
	cardTheFool = nullptr;
	cardsIcon = nullptr;
	cardTheMagician = nullptr;
	cardWheelOfFortune = nullptr;
	arrowLeft = nullptr;
	arrowRight = nullptr;
	// Clean up map and entities
	Engine::GetInstance().map->CleanUp();
	Engine::GetInstance().entityManager->CleanUp();
	Engine::GetInstance().physics->CleanUp();
	Engine::GetInstance().physics->Start();
	//tendria que hacer unload de las texturas del panel y corazones...

}

void  Scene::PostUpdateLevel() {

	// Cargar/Guardar estado (F5/F6)
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		Engine::GetInstance().map->LoadEntities(player, enemies);
	}
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		Engine::GetInstance().map->SaveEntities(player);
		savedLevel = 1;
	}


	// Dibujar Mapa

	// Dibujar Ayuda
	if (showHelp && helpTexture != nullptr)
	{
		SDL_FRect centrar = { 0, 0, 1280, 720 };
		SDL_RenderTexture(Engine::GetInstance().render->renderer, helpTexture, NULL, &centrar);
	}
	

	if (player != nullptr && Engine::GetInstance().scene->hasTalkedMagician == true) {
		
			if (panelTexture != nullptr) {
				float w, h;
				SDL_GetTextureSize(panelTexture, &w, &h);


				SDL_FRect destRect = { 0, 0, w/2, h/2 };
				SDL_RenderTexture(Engine::GetInstance().render->renderer, panelTexture, NULL, &destRect);

			}
			SDL_Texture* texturaDibujar = nullptr;

			if (Engine::GetInstance().scene->maxLives == 6) {
				// SEXTOS
				if (Engine::GetInstance().scene->lives == 6) texturaDibujar = quesoSextos_6;
				else if (Engine::GetInstance().scene->lives == 5) texturaDibujar = quesoSextos_5;
				else if (Engine::GetInstance().scene->lives == 4) texturaDibujar = quesoSextos_4;
				else if (Engine::GetInstance().scene->lives == 3) texturaDibujar = quesoSextos_3;
				else if (Engine::GetInstance().scene->lives == 2) texturaDibujar = quesoSextos_2;
				else if (Engine::GetInstance().scene->lives == 1) texturaDibujar = quesoSextos_1;
			}
			else if (Engine::GetInstance().scene->maxLives == 5) {
				// QUINTOS
				if (Engine::GetInstance().scene->lives == 5) texturaDibujar = quesoQuintos_5;
				else if (Engine::GetInstance().scene->lives == 4) texturaDibujar = quesoQuintos_4;
				else if (Engine::GetInstance().scene->lives == 3) texturaDibujar = quesoQuintos_3;
				else if (Engine::GetInstance().scene->lives == 2) texturaDibujar = quesoQuintos_2;
				else if (Engine::GetInstance().scene->lives == 1) texturaDibujar = quesoQuintos_1;
			}
			else {
				//CUARTOS
				if (Engine::GetInstance().scene->lives == 4) texturaDibujar = heart4Texture;
				else if (Engine::GetInstance().scene->lives == 3) texturaDibujar = heart3Texture;
				else if (Engine::GetInstance().scene->lives == 2) texturaDibujar = heart2Texture;
				else if (Engine::GetInstance().scene->lives == 1) texturaDibujar = heart1Texture;
			}

			if (texturaDibujar != nullptr) {
				float w, h;
				SDL_GetTextureSize(texturaDibujar, &w, &h);
				SDL_FRect destRect = { 90, 112, w / 3, h / 3 };
				SDL_RenderTexture(Engine::GetInstance().render->renderer, texturaDibujar, NULL, &destRect);
			}

		
		std::string scoreText = "Score: " + std::to_string(Engine::GetInstance().scene->score);
		Engine::GetInstance().render->DrawText(scoreText.c_str(), 1100, 30, 0, 0, { 255, 255, 255, 255 });
	}

	if (player != nullptr) {
		std::string scoreText = "Score: " + std::to_string(Engine::GetInstance().scene->score);
		Engine::GetInstance().render->DrawText(scoreText.c_str(), 1100, 30, 0, 0, { 255, 255, 255, 255 });

		std::string timerText = "Time: " + std::to_string((int)levelTimer);
		Engine::GetInstance().render->DrawText(timerText.c_str(), 1100, 70, 0, 0, { 255, 255, 255, 255 });
	}
	if (isPaused) {
		SDL_Rect screenRect = { -10000, -10000, 50000, 50000 };
		Engine::GetInstance().render->DrawRectangle(screenRect, 0, 0, 0, 150, true, false);

		Engine::GetInstance().render->DrawText("PAUSE", 600, 150, 0, 0, { 255, 255, 255, 255 });
	}
	if (storeOn) {
		float w, h;
		SDL_GetTextureSize(storeBag, &w, &h);
		Engine::GetInstance().render->DrawTextureNoCamera(storeBag, 80, -100, w / 1.5, h / 1.5);

		if (selectedStoreItem == 1) {
			SDL_GetTextureSize(storePaperMap, &w, &h);
			Engine::GetInstance().render->DrawTextureNoCamera(storePaperMap, 550, -50, w / 1.5, h / 1.5);
		}
		else if (selectedStoreItem == 2) {
			SDL_GetTextureSize(storePaperKey, &w, &h);
			Engine::GetInstance().render->DrawTextureNoCamera(storePaperKey, 550, -50, w / 1.5, h / 1.5);
		}
		else if (selectedStoreItem == 3) {
			SDL_GetTextureSize(storePaperLife, &w, &h);
			Engine::GetInstance().render->DrawTextureNoCamera(storePaperLife, 550, -50, w / 1.5, h / 1.5);
		}
		else if (selectedStoreItem == 4) {
			SDL_GetTextureSize(storePaperDamage, &w, &h);
			Engine::GetInstance().render->DrawTextureNoCamera(storePaperDamage, 550, -50, w / 1.5, h / 1.5);
		}
		else if (selectedStoreItem == 5) {
			SDL_GetTextureSize(storePaperPermLife, &w, &h);
			Engine::GetInstance().render->DrawTextureNoCamera(storePaperPermLife, 550, -50, w / 1.5, h / 1.5);
		}
	}

	if (inventoryOn) {
		SDL_Rect blackBackground = { 0, 0, 10000, 10000 };
		Engine::GetInstance().render->DrawRectangle(blackBackground, 0, 0, 0, 200, true, false);

		if (invPaperCombined != nullptr) {
			Engine::GetInstance().render->DrawTextureNoCamera(invPaperCombined, 490, 60, 800, 600);

			if (player != nullptr) {
				int startX = 700; 
				int startY = 230; 
				int offsetX = 160; 

				int itemsPorPagina = 3;

				int inicio = currentInvPage * itemsPorPagina;

				int fin = inicio + itemsPorPagina;
				if (fin > inventario.objetos.size()) {
					fin = inventario.objetos.size();
				}

				for (int j = inicio; j < fin; j++) {

					SDL_Texture* tex = inventario.objetos[j].imagen;

					if (tex != nullptr) {
						
						int posEnPantalla = j - inicio;
						Engine::GetInstance().render->DrawTextureNoCamera(tex, startX + (posEnPantalla * offsetX), startY, 160, 160);

						if (inventario.objetos[j].nombre == "Map") {
							rectInvMap = { startX + (posEnPantalla * offsetX), startY, 160, 160 };
						}
					}
				}
			}
			if (inventoryBag != nullptr) {
				Engine::GetInstance().render->DrawTextureNoCamera(inventoryBag, 90, 30, 800, 800);
			}
			if (uiCoin != nullptr) {
				int coinSize = 65;
				int coinX = 800;
				int coinY = 410;

				Engine::GetInstance().render->DrawTextureNoCamera(uiCoin, coinX, coinY, coinSize, coinSize);
				Engine::GetInstance().render->DrawTextureNoCamera(uiCoin, coinX + 25, coinY +25, coinSize, coinSize);
				Engine::GetInstance().render->DrawTextureNoCamera(uiCoin, coinX -25, coinY +25, coinSize, coinSize);

				if (player != nullptr) {
					std::string scoreText = std::to_string(Engine::GetInstance().scene->score);

					int textX = coinX + coinSize + 30;
					int textY = coinY + 40; 
					Engine::GetInstance().render->DrawText(scoreText.c_str(), textX, textY, 0, 0, { 0, 0, 0, 255 });
				}
			}
			
		
		}
	}

	//MISSIONS

	if (list && !someoneIsTalking ) {
	
	
		if (misiones.objetos.size() > 0) {
			SDL_Rect screenRect = { -10000, -10000, 50000, 50000 };
			Engine::GetInstance().render->DrawRectangle(screenRect, 0, 0, 0, 150, true, false);
			float w, h;
			SDL_FRect centrar = { 0, 0, 1280, 720 };
			SDL_GetTextureSize(misiones.objetos[currentMission].imagen, &w, &h);
			Engine::GetInstance().render->DrawTextureNoCamera(misiones.objetos[currentMission].imagen, 370, 130, w/2, h/2);
			misiones.objetos[currentMission].visualizada = true;
		}
	
	
	
	}
	if (showMap && map1Texture != nullptr)
	{
		SDL_FRect centrar = { 0, 0, 1280, 720 };
		SDL_RenderTexture(Engine::GetInstance().render->renderer, map1Texture, NULL, &centrar);
	}
	Engine::GetInstance().uiManager->PostUpdate();

}

// *********************************************
// GAME OVER functions
// *********************************************

void Scene::LoadGameOver() {
	LOG("Loading Game Over Screen");
	Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/Death_loop.wav");
	loseTexture = Engine::GetInstance().textures->Load("assets/UI/GameOver/UI_GameOver_.png");


	 Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/Death_loop.wav");

	/*SDL_Rect btnPos = { 1000, 650, 250, 50 };
	Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 9, "BACK TO TITLE", btnPos, this);*/

	 SDL_Texture* buttonTitleNormal = Engine::GetInstance().textures->Load("assets/UI/Pause/UI_Pause_ButtonQuit1_01.png");
	 SDL_Texture* buttonTitleClicked = Engine::GetInstance().textures->Load("assets/UI/Pause/UI_Pause_ButtonQuitPressed_01.png");

	 SDL_Rect btnPos = { 1000, 650, 200, 50 };

	 Engine::GetInstance().uiManager->CreateUIElement(
		 UIElementType::BUTTON,
		 9,
		 "",
		 btnPos,
		 this,
		 SDL_Rect{ 0,0,0,0 },
		 buttonTitleNormal,
		 buttonTitleClicked
	 );
}

void Scene::UpdateGameOver(float dt) {
	loseAnimSet.Update(dt);
}

void Scene::UnloadGameOver() {
	LOG("Unloading Game Over Screen");
	Engine::GetInstance().textures->UnLoad(loseTexture);
	loseTexture = nullptr;
	Engine::GetInstance().uiManager->CleanUp();
}

void Scene::HandleGameOverUIEvents(UIElement* uiElement)
{
	switch (uiElement->id)
	{
	case 9: 
		ChangeScene(SceneID::MAIN_MENU);
		break;
	default:
		break;
	}
}

// *********************************************
// WIN SCREEN functions
// *********************************************

void Scene::LoadWinScreen() {
	LOG("Loading Win Screen");
	Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/PREV/winmusic.wav");
	loseTexture = Engine::GetInstance().textures->Load("Assets/Screens/PREV/lose-win-screen.png");

	std::unordered_map<int, std::string> aliases;
	aliases[0] = "win"; 
	loseAnimSet.LoadFromTSX("Assets/Maps/PREV/lose.tsx", aliases);
	loseAnimSet.SetCurrent("win");

	SDL_Rect btnPos = { 1000, 650, 250, 50 };
	Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 10, "NEXT LEVEL -->", btnPos, this);
}

void Scene::UpdateWinScreen(float dt) {
	loseAnimSet.Update(dt);
}

void Scene::UnloadWinScreen() {
	Engine::GetInstance().textures->UnLoad(loseTexture);
	loseTexture = nullptr;
	Engine::GetInstance().uiManager->CleanUp();
}

void Scene::CreatePauseUI() {
	/*auto btnPauseHUD = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 30, "||", { 1200, 20, 50, 50 }, this);*/
	/*btnPauseHUD->visible = true;*/
	int x = 520;
	int y = 250;

	// RESUME
	SDL_Texture* buttonResumeNormal;
	SDL_Texture* buttonResumedClicked;
	buttonResumeNormal = Engine::GetInstance().textures->Load("assets/UI/Pause/UI_Pause_ButtonContinue1_01.png");
	buttonResumedClicked = Engine::GetInstance().textures->Load("assets/UI/Pause/UI_Pause_ButtonContinuePressed_01.png");
	auto btnResume = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 20, "RESUME", { x, y - 50, 200, 50 }, this, SDL_Rect{0,0,0,0}, buttonResumeNormal, buttonResumedClicked);
	btnResume->visible = false;

	// OPTIONS
	SDL_Texture* buttonOptionsNormal;
	SDL_Texture* buttonOptionsClicked;
	buttonOptionsNormal = Engine::GetInstance().textures->Load("assets/UI/Pause/UI_Pause_ButtonOptions1_01.png");
	buttonOptionsClicked = Engine::GetInstance().textures->Load("assets/UI/Pause/UI_Pause_ButtonOptionsPressed_01.png");
	auto btnOptions = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 21, "OPTIONS", { x, y + 20, 200, 50 }, this, SDL_Rect{ 0,0,0,0 }, buttonOptionsNormal,buttonOptionsClicked);
	btnOptions->visible = false;

	//BACK TO TITLE
	SDL_Texture* buttonTitleNormal;
	SDL_Texture* buttonTitleClicked;
	
	buttonTitleNormal = Engine::GetInstance().textures->Load("assets/UI/Pause/UI_Pause_ButtonQuit1_01.png");
	buttonTitleClicked = Engine::GetInstance().textures->Load("assets/UI/Pause/UI_Pause_ButtonQuitPressed_01.png");
	auto btnTitle = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 22, "TITLE SCREEN", { x, y + 100, 200, 50 }, this, SDL_Rect{0,0,0,0}, buttonTitleNormal, buttonTitleClicked);
	btnTitle->visible = false;

	// EXIT
	SDL_Texture* buttonExitPressed;
	SDL_Texture* buttonExitNormal;
	buttonExitPressed = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextExitPressed_01.png");
	buttonExitNormal = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextExit1_01.png");
	auto btnExit = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 23, "EXIT GAME", { x, y + 200, 200, 50 }, this, SDL_Rect{0,0,0,0}, buttonExitNormal, buttonExitPressed);
	btnExit->visible = false;

	//SLIDER MUSICA
	SDL_Texture* SliderBar;
	SDL_Texture* thumbPressed;
	SDL_Texture* thumbNormal;
	SliderBar = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_SliderBar_01.png");
	thumbPressed = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_SliderButton_01.png");
	thumbNormal = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_Checkbox_01.png");

	auto sliderMusic = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, 26, "MUSIC", { x, y+15, 200, 30 }, this, SDL_Rect{0,0,0,0}, SliderBar, thumbNormal, thumbPressed);
	sliderMusic->visible = false;

	//SLIDER FX
	auto sliderFX = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::SLIDER, 27, "FX", { x, y + 75, 200, 30 }, this, SDL_Rect{0,0,0,0}, SliderBar, thumbNormal, thumbPressed);
	sliderFX->visible = false;

	//BACK FROM OPTIONS
	SDL_Texture* buttonBackPressed;
	SDL_Texture* buttonBackNormal;
	buttonBackPressed = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextBackPressed_01.png");
	buttonBackNormal = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextBack1_01.png");
	auto btnBackOpt = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 25, "BACK", { x, y + 140, 200, 50 }, this, SDL_Rect{0,0,0,0}, buttonBackNormal, buttonBackPressed);
	btnBackOpt->visible = false;

	//fullscreen
	//Checkbox de la fullscreen
	SDL_Texture* buttonPressed;
	SDL_Texture* buttonNormal;
	buttonPressed = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_CheckboxPressed_01.png");
	buttonNormal = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_Checkbox_01.png");
	SDL_Rect Fullscreen = { x+90,y-50, 35, 35 };
	auto fullscreen = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::CHECKBOX, 52, "FULL SCREEN", Fullscreen, this, SDL_Rect{ 0,0,0,0 }, buttonPressed, buttonNormal);
	if (fullscreen) fullscreen->visible = false;
	this->Volume = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextVolume1.png");
	this->VolumeEffects = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextEffectsVolume_01.png");
	this->fullscreen = Engine::GetInstance().textures->Load("assets/UI/Options/UI_Settings_TextFullScreen_01.png");
}


void Scene::SetPause(bool pause) {
	isPaused = pause;
	for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
		if (element->id >= 20 && element->id <= 23 || element->id == 52) {
			element->visible = isPaused;
		}
		if (element->id == 30 || element->id == 52) {
			element->visible = !isPaused;
		}
		if (element->id >= 25 && element->id <= 29) {
			element->visible = false;
		}
		if (element->id == 52){
			element->visible = false;
		}
	}
}


void Scene::HandlePauseUIEvents(UIElement* uiElement) {
	switch (uiElement->id) {
	case 20: 
		SetPause(false);
		break;
	case 21:
		for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
			if (el->id >= 20 && el->id <= 23) el->visible = false;
			if (el->id == 25 || el->id == 26 || el->id == 27 || el->id ==52) el->visible = true;
			slidersOn = true;
		}
		break;
	case 22: 
		SetPause(false);
		ChangeScene(SceneID::MAIN_MENU);
		break;
	case 23: 
		exitGame = true;
		break;
	case 26: 
		if (uiElement->type == UIElementType::SLIDER) {
			UISlider* slider = static_cast<UISlider*>(uiElement);
			float vol = slider->GetValue();
			Engine::GetInstance().audio->SetMusicVol(vol);
			Engine::GetInstance().audio->SetSFXVol(vol);
		}
		break;
	case 25: 
		for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
			if (el->id >= 20 && el->id <= 23) el->visible = true;
			if (el->id == 52 || (el->id >= 25 && el->id <= 28)) el->visible = false;
		}
		slidersOn = false;
		break;
	case 27:
		if (uiElement->type == UIElementType::SLIDER) {
			float vol = ((UISlider*)uiElement)->GetValue();
			Engine::GetInstance().audio->SetSFXVol(vol); 
		}
		break;
	case 30: 
		SetPause(true); 
		break;
	case 52:
		if (uiElement->state == UIElementState::SELECTED) {
			Engine::GetInstance().window->SetFullscreen(true);
		}
		else {
			Engine::GetInstance().window->SetFullscreen(false);
		}
		break;
	}
}
void Scene::UpdatePauseMenu() {
	if (slidersOn) {
		SDL_Rect VolumeRect = { 520,220, 200, 50 };
		Engine::GetInstance().render->DrawTextureNoCamera(Volume, VolumeRect.x, VolumeRect.y, VolumeRect.w, VolumeRect.h);
		SDL_Rect Fullscreen = { 520,175, 200, 50 };
		Engine::GetInstance().render->DrawTextureNoCamera(fullscreen, Fullscreen.x, Fullscreen.y, Fullscreen.w, Fullscreen.h);

		SDL_Rect EffectsRect = { 520,280, 200, 50 };
		Engine::GetInstance().render->DrawTextureNoCamera(VolumeEffects, EffectsRect.x, EffectsRect.y, EffectsRect.w, EffectsRect.h);





	}
}
void Scene::SaveLevel()
{
	std::ofstream file("Assets/savegame.txt");
	if (!file.is_open()) return;

	if (Engine::GetInstance().map != nullptr) {
		file << Engine::GetInstance().map->mapFileName << std::endl;
	}

	file.close();
}

// *********************************************
// FINAL WIN functions 
// *********************************************

void Scene::LoadFinalWin() {
	LOG("Loading Final Win Screen");

	loseTexture = Engine::GetInstance().textures->Load("Assets/Screens/PREV/lose-win-screen.png");
	std::unordered_map<int, std::string> aliases;
	aliases[0] = "win";
	loseAnimSet.LoadFromTSX("Assets/Maps/PREV/lose.tsx", aliases);
	loseAnimSet.SetCurrent("win");

	Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/PREV/win_sound.wav"); //poner audio
	SDL_Rect btnPos = { 950, 650, 250, 50 };
	Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 13, "GO TO TITLE ->", btnPos, this);
}

void Scene::UpdateFinalWin(float dt) {
	loseAnimSet.Update(dt);
}

void Scene::UnloadFinalWin() {
	Engine::GetInstance().textures->UnLoad(loseTexture);
	loseTexture = nullptr;
	Engine::GetInstance().uiManager->CleanUp();
}

void Scene::HandleFinalWinUIEvents(UIElement* uiElement) {
	if (uiElement->id == 13) {
		ChangeScene(SceneID::MAIN_MENU);
	}
}

void Scene::LoadMap(std::string map)
{
	UnloadLevel();

	Engine::GetInstance().entityManager->CleanUp();

	if (map == "TEST_map_LV1_startRoom_01.tmx") {
		Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/PREV/level2music.wav");
	}
	else if (map == "TEST_map_LV1_tortureRoom_02.tmx") {
		Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/COMBATdowninthedungeons.wav");
	}
	else if (map == "TEST_map_LV1_pantryRoom_01.tmx"||"Map_LV2_encreuada_02.tmx"|| "Map_LV2_botanica_02.tmx"|| "Map_LV2_justes_02.tmx") {
		Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/DOWNTIME_ratmosphere.wav");
	}
	else if (map == "TEST_map_LV1_bossRoom_01.tmx" || "Map_LV2_bossTower.tmx") {
		Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/COMBATdanceofthedungeons.wav");
	}
	else {
		Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/PREV/level2music.wav");
	}
	
	isPaused = false;
	CreatePauseUI();
	MissionUI();
	CreateTeleportUI();
	InventariIconUI();
	if (map.find("LV2") != std::string::npos) {
		CreateStoreLevel2();
	}
	else {
		CreateStoreLevel1();
	}
	CreateInventoryUI();
	helpTexture = Engine::GetInstance().textures->Load("assets/UI/UI_TutorialControls.png");
	map1Texture = Engine::GetInstance().textures->Load("assets/UI/Map/UI_Map_Level1.png");
	heartTexture = Engine::GetInstance().textures->Load("assets/Textures/PREV/heart4.png");
	panelTexture = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_01.png");
	heart1Texture = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_Cheese1_01.png");
	heart2Texture = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_Cheese2_01.png");
	heart3Texture = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_Cheese3_01.png");
	heart4Texture = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_Cheese4_01.png");
	quesoQuintos_1 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_02/UI_LifeBar_Cheese1_02.png");
	quesoQuintos_2 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_02/UI_LifeBar_Cheese2_02.png");
	quesoQuintos_3 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_02/UI_LifeBar_Cheese3_02.png");
	quesoQuintos_4 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_02/UI_LifeBar_Cheese4_02.png");
	quesoQuintos_5 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_02/UI_LifeBar_Cheese5_02.png");
	quesoSextos_1 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_03/UI_LifeBar_Cheese1_03.png");
	quesoSextos_2 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_03/UI_LifeBar_Cheese2_03.png");
	quesoSextos_3 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_03/UI_LifeBar_Cheese3_03.png");
	quesoSextos_4 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_03/UI_LifeBar_Cheese4_03.png");
	quesoSextos_5 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_03/UI_LifeBar_Cheese5_03.png");
	quesoSextos_6 = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_03/UI_LifeBar_Cheese6_03.png");
	extraHeartTexture = Engine::GetInstance().textures->Load("assets/UI/LifeBar/UI_LifeBar_CheeseExtra_01.png");

	missionTexture = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_Mission_NotificationStart_.png");
	//Call the function to load the map. 
	Engine::GetInstance().map->Load("assets/Maps/", map);

	//Call the function to load entities from the map
	Engine::GetInstance().map->LoadEntities(player, enemies);

	if (firstMapLoad)
	{
		showHelp = true;
		firstMapLoad = false;
	}
	else
	{
		showHelp = false;
	}

	if (continueGame == false) {

		levelTimer = 0.0f;
		Engine::GetInstance().scene->score = 0;
		showMap = false;
		inventoryOn = false;
		Engine::GetInstance().scene->extralife = false; 
		Engine::GetInstance().scene->lives = 4;
		//firstDoor = true;       
		//cheese = false;     
		//nextSpawnPoint = "";
		//nextMap = "";
		if (player) {
			Engine::GetInstance().scene->lives = 4;

			/*
			player->extralife = false;
			player->hasCheese = false;
			player->hasMap1 = false;
			player->hasTalkedMagician = false;
			player->isDeadDefinitive = false;*/
		}

		/*Vector2D startPos = Engine::GetInstance().map->GetStartPoint("Checkpoints", "Player");

		if (startPos.getX() != 0 || startPos.getY() != 0) {
			player->SetPosition(startPos);
			player->respawnPosition = { PIXEL_TO_METERS(startPos.getX()), PIXEL_TO_METERS(startPos.getY()) };
		}*/
	}
	else {
		//showHelp = false;
		showMap = false;
		inventoryOn = false;
	}

}

// *********************************************
// INTRO SCREEN functions
// *********************************************

void Scene::LoadIntro() {
	LOG("Loading Intro Screen");
	introTexture = Engine::GetInstance().textures->Load("Assets/Screens/logo_white.png");
}

void Scene::UpdateIntro(float dt) {
	if (Engine::GetInstance().input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) {
		ChangeScene(SceneID::MAIN_MENU);
	}
}

void Scene::UnloadIntro() {
	LOG("Unloading Intro Screen");
	Engine::GetInstance().textures->UnLoad(introTexture);
	introTexture = nullptr;
}


void Scene::StartFadeOut(float duration)
{
	isFading = true;
	fadeIn = false;
	fadeTime = 0.0f;
	fadeDuration = duration;
}

void Scene::StartFadeIn(float duration)
{
	isFading = true;
	fadeIn = true;
	fadeTime = 0.0f;
	fadeDuration = duration;
}

//ALL STORE THINGS

void Scene::CreateStoreLevel1() {
	
	int x = 200;
	int y = 120;

	// MAP
	SDL_Texture* MapNormal;
	SDL_Texture* MapClicked;
	MapClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemMap1_01.png");
	MapNormal = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemMap2_01.png");
	auto btnMAP = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 35, "MAP", { 350, y, 170, 170 }, this, SDL_Rect {0,0,0,0}, MapNormal, MapClicked);
	btnMAP->visible = false;

	// KEY
	SDL_Texture* KeyNormal;
	SDL_Texture* KeyClicked;
	KeyNormal = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemKey2_01.png");
	KeyClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemKey1_01.png");
	auto btnKEY = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 36, "KEY", { x, y + 150, 170, 170 }, this, SDL_Rect {0,0,0,0}, KeyNormal, KeyClicked);
	btnKEY->visible = false;

	//LIFE
	SDL_Texture* LifeNormal;
	SDL_Texture* LifeClicked;
	LifeNormal = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemLife2_01.png");
	LifeClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemLife1_01.png");
	auto btnLIFE = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 37, "TEMPORARY LIFE", { 350, y +250, 170, 170 }, this, SDL_Rect {0,0,0,0}, LifeNormal, LifeClicked);
	btnLIFE->visible = false;


	//PERMANENT LIFE
	SDL_Texture* PermLife;
	SDL_Texture* PermLifeClicked;
	PermLife = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemPermanentLife1_.png");
	PermLifeClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemPermanentLife2_.png");
	auto btnPERMLIFE = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 50, "PERMANENT LIFE", { 450, y + 150, 170, 170 }, this, SDL_Rect{ 0,0,0,0 }, PermLife, PermLifeClicked);
	btnPERMLIFE->visible = false;

	//// LLANTERN
	//auto btnLLANTERN = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 38, "LLANTERN", { x, y + 210, 200, 50 }, this);
	//btnLLANTERN->visible = false;

	// BUYMAP
	SDL_Texture* BuyNormal;
	SDL_Texture* BuyClicked;
	BuyNormal = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ButtonBuy1_01.png");
	BuyClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ButtonBuy2_01.png");
	auto btnBUYMAP = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 39, "BUYMAP", { 750, y+ 280, 300, 150 }, this, SDL_Rect{0,0,0,0}, BuyNormal, BuyClicked);
	btnBUYMAP->visible = false;

	//BUYKEY
	auto btnBUYKEY = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 40, "BUYKEY", { 750, y+ 280, 300, 150 }, this, SDL_Rect{ 0,0,0,0 }, BuyNormal, BuyClicked);
	btnBUYKEY->visible = false;
	
	//BUYLIFE
	
	auto btnBUYLIFE = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 41, "BUYLIFE", { 750, y+280, 300, 150 }, this, SDL_Rect{ 0,0,0,0 }, BuyNormal, BuyClicked);
	btnBUYLIFE->visible = false;


	// BUY PERMANENT LIFE
	auto btnBUYPERMLIFE = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 51, "BUYPERMLIFE", { 750, y + 280, 300, 150 }, this, SDL_Rect{ 0,0,0,0 }, BuyNormal, BuyClicked);
	btnBUYPERMLIFE->visible = false;


	////BUYLLANTERN
	//auto btnBUYLLANTERN = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 42, "BUYLLANTERN", { x + 70, y, 200, 50 }, this);
	//btnBUYLLANTERN->visible = false;

	//texturas decoracion
	storeBag = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_Bag_02.png");
	storePaperMap = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_Paper_Map.png");
	storePaperLife = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_Paper_Life.png");
	storePaperKey = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_Paper_Key.png");
	storePaperPermLife = Engine::GetInstance().textures->Load("assets/UI/Store/StoreUI_Store_Paper_PermanentLife_.png");
}

void Scene::CreateStoreLevel2() {

	int x = 200;
	int y = 120;

	// MAP
	SDL_Texture* MapNormal;
	SDL_Texture* MapClicked;
	MapClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemMap1_01.png");
	MapNormal = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemMap2_01.png");
	auto btnMAP = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 35, "MAP", { 350, y, 170, 170 }, this, SDL_Rect{ 0,0,0,0 }, MapNormal, MapClicked);
	btnMAP->visible = false;

	// KEY
	SDL_Texture* KeyNormal;
	SDL_Texture* KeyClicked;
	KeyNormal = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemKey2_01.png");
	KeyClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemKey1_01.png");
	auto btnKEY = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 36, "KEY", { x, y + 150, 170, 170 }, this, SDL_Rect{ 0,0,0,0 }, KeyNormal, KeyClicked);
	btnKEY->visible = false;

	//LIFE
	SDL_Texture* LifeNormal;
	SDL_Texture* LifeClicked;
	LifeNormal = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemLife2_01.png");
	LifeClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemLife1_01.png");
	auto btnLIFE = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 37, "TEMPORARY LIFE", { 350, y + 250, 170, 170 }, this, SDL_Rect{ 0,0,0,0 }, LifeNormal, LifeClicked);
	btnLIFE->visible = false;

	//MILLORA MAL
	SDL_Texture* Damageplus;
	SDL_Texture* DamageplusClicked;
	Damageplus = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemDamage1_.png");
	DamageplusClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemDamage2_.png");
	auto btnDAMAGE = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 43, "DAMAGE PLUS", { 450, y + 150, 170, 170 }, this, SDL_Rect{ 0,0,0,0 }, Damageplus, DamageplusClicked);
	btnDAMAGE->visible = false;


	//// LLANTERN
	//auto btnLLANTERN = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 38, "LLANTERN", { x, y + 210, 200, 50 }, this);
	//btnLLANTERN->visible = false;

	// BUYMAP
	SDL_Texture* BuyNormal;
	SDL_Texture* BuyClicked;
	BuyNormal = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ButtonBuy1_01.png");
	BuyClicked = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ButtonBuy2_01.png");
	auto btnBUYMAP = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 39, "BUYMAP", { 750, y + 280, 300, 150 }, this, SDL_Rect{ 0,0,0,0 }, BuyNormal, BuyClicked);
	btnBUYMAP->visible = false;

	//BUYKEY
	auto btnBUYKEY = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 40, "BUYKEY", { 750, y + 280, 300, 150 }, this, SDL_Rect{ 0,0,0,0 }, BuyNormal, BuyClicked);
	btnBUYKEY->visible = false;

	//BUYLIFE

	auto btnBUYLIFE = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 41, "BUYLIFE", { 750, y + 280, 300, 150 }, this, SDL_Rect{ 0,0,0,0 }, BuyNormal, BuyClicked);
	btnBUYLIFE->visible = false;

	//BUYDAMAGE

	auto btnBUYDAMAGE = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 44, "BUYLIFE", { 750, y + 280, 300, 150 }, this, SDL_Rect{ 0,0,0,0 }, BuyNormal, BuyClicked);
	btnBUYDAMAGE->visible = false;

	////BUYLLANTERN
	//auto btnBUYLLANTERN = Engine::GetInstance().uiManager->CreateUIElement(UIElementType::BUTTON, 42, "BUYLLANTERN", { x + 70, y, 200, 50 }, this);
	//btnBUYLLANTERN->visible = false;

	//texturas decoracion
	storeBag = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_Bag_02.png");
	storePaperMap = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_Paper_Map.png");
	storePaperLife = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_Paper_Life.png");
	storePaperKey = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_Paper_Key.png");
	storePaperDamage = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_Paper_Damage_.png");
}

void Scene::SetStore(bool store, int storeID) {
	storeOn = store;
	if (storeOn) {
		if (storeID == 1) {
			for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
				if (element->id == 35 || element->id == 36 || element->id == 37|| element->id == 50) {
					element->visible = storeOn;
				}
			}
		}
		else if (storeID == 2) {
			for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
				if (element->id == 35 || element->id == 36 || element->id == 37 || element->id == 43) {
					element->visible = storeOn;
				}
			}
		}
	}
	else {
		for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
			if (element->id >= 35 && element->id <= 51) {
				element->visible = false;
			}
		}
	}
	if (!storeOn) {
		selectedStoreItem = 0;
	}
	
		
}


void Scene::HandleStoreUIEvents(UIElement* uiElement) {
	switch (uiElement->id) {
	case 35:
		for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
			if (el->id == 39) el->visible = true;
			if (el->id > 39 && el->id <= 42 || el->id == 44) el->visible = false;

		}
		selectedStoreItem = 1;
		break;
	case 36:
		for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
			if (el->id == 40) el->visible = true;
			if (el->id == 39 || el->id >= 41 && el->id <= 42 || el->id == 44) el->visible = false;

		}
		selectedStoreItem = 2;
		break;
	case 37:
		for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
			if (el->id == 41) el->visible = true;
			if (el->id == 39 || el->id == 40 || el->id == 42 || el->id == 44) el->visible = false;

		}

		selectedStoreItem = 3;
		break;
		//case 38:
		//	for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
		//		if (el->id == 42) el->visible = true;
		//		if (el->id == 39 || el->id == 40 || el->id == 41) el->visible = false;

		//	}
		//	//imagen informativa de lo que hace en grande tmbn
		//	break;
	case 39:
		//mira si el jugador tiene dinero
		if (Engine::GetInstance().scene->score >= 20) {
			Engine::GetInstance().scene->score -= 20;
			for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
				if (el->id == 39) {
					el->visible = false;
					el->state = UIElementState::DISABLED;
				}
				if (el->id == 35) {
					SDL_Texture* BeenBought = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_SoldOut_01.png");
					el->SetTexture(BeenBought);
				}
			}
			inventario.push("Map", iconMap, nullptr);
			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasBeenSold.hasEnded = false;
					handman->hasBeenSold.BeginDialogue();
					break;
				}
			}
		}
		else {
			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasNoMoney.hasEnded = false;
					handman->hasNoMoney.BeginDialogue();
					break;
				}
			}

		}
		break;
	case 40:
		//mira si el jugador tiene dinero
		if (Engine::GetInstance().scene->score >= 50) {
			Engine::GetInstance().scene->score -= 50;
			for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
				if (el->id == 40) el->Destroy();
				if (el->id == 36) {

					SDL_Texture* BeenBought = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_SoldOut_01.png");
					el->SetTexture(BeenBought);
				}

			}
			Engine::GetInstance().scene->inventario.push("Key", iconKey, nullptr);

			//FUNCION DE QUE PLAYER TIENE LA LLAVE
			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasBeenSold.hasEnded = false;
					handman->hasBeenSold.BeginDialogue();
					break;
				}
			}
		}
		else {
			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasNoMoney.hasEnded = false;
					handman->hasNoMoney.BeginDialogue();
					break;
				}
			}
		

		}
		

		break;
	case 41:
		//mira si el jugador tiene dinero
		if (Engine::GetInstance().scene->score >= 10) {
			Engine::GetInstance().scene->score -= 10;
			for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
				if (el->id == 41) el->Destroy();
				if (el->id == 37) {
					SDL_Texture* BeenBought = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_SoldOut_01.png");
					el->SetTexture(BeenBought);

				}

			}
			player->extralife = true;
			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasBeenSold.hasEnded = false;
					handman->hasBeenSold.BeginDialogue();
					break;
				}
			}
		}
		else {
			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasNoMoney.hasEnded = false;
					handman->hasNoMoney.BeginDialogue();
					break;
				}
			}

		}
		break;
	
	case 43: 
		for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
			if (el->id == 44) el->visible = true;
			if (el->id >= 39 && el->id <= 42) el->visible = false;
		}
		selectedStoreItem = 4; 
		break;

	case 44:
		if (Engine::GetInstance().scene->score >= 80) {
			Engine::GetInstance().scene->hasDamagePlus = true;
			for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
				if (el->id == 44) el->Destroy();
				if (el->id == 43) {
					SDL_Texture* BeenBought = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_SoldOut_01.png");
					el->SetTexture(BeenBought);
				}
			}

			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasBeenSold.hasEnded = false;
					handman->hasBeenSold.BeginDialogue();
					break;
				}
			}
		}
		else {
			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasNoMoney.hasEnded = false;
					handman->hasNoMoney.BeginDialogue();
					break;
				}
			}
		}
		break;
	case 50:
		for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
			if (el->id == 51) el->visible = true; 
			if (el->id == 39 || el->id == 40 || el->id == 41 || el->id == 42 || el->id == 44) el->visible = false;
		}
		selectedStoreItem = 5; 
		break;

	case 51:
		if (Engine::GetInstance().scene->score >= 200 && Engine::GetInstance().scene->maxLives < 6) {

			Engine::GetInstance().scene->score -= 200;

			Engine::GetInstance().scene->maxLives++;
			Engine::GetInstance().scene->lives = Engine::GetInstance().scene->maxLives;

			Engine::GetInstance().scene->extralife = true;

			if (Engine::GetInstance().scene->maxLives >= 6) {
				for (auto& el : Engine::GetInstance().uiManager->UIElementsList) {
					if (el->id == 51) el->Destroy(); 
					if (el->id == 50) {
						SDL_Texture* BeenBought = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_SoldOut_01.png");
						el->SetTexture(BeenBought); 
					}
				}
			}

			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasBeenSold.hasEnded = false;
					handman->hasBeenSold.BeginDialogue();
					break;
				}
			}
		}
		else {
			for (auto& entity : Engine::GetInstance().entityManager->entities) {
				if (entity->type == EntityType::HANDMAN) {
					HANDMAN* handman = static_cast<HANDMAN*>(entity.get());
					handman->hasNoMoney.hasEnded = false;
					handman->hasNoMoney.BeginDialogue();
					break;
				}
			}
		}
		break;
	}

}

Player* Scene::GetPlayer() const
{
	return player.get();
}

//funciones inventario
void Scene::CreateInventoryUI() {
	invPaperCombined = Engine::GetInstance().textures->Load("assets/UI/Inventario/UI_Inventari_PaperAll_01.png"); 
	iconMap = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemMap1_01.png"); 
	iconKey = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemKey1_01.png");
	iconLamp = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_ItemLamp1_01.png"); 
	iconSpring = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemSacredSpringWater1_.png"); 
	iconHorseMacure = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemHorsekinManure1_.png"); 
	iconGargantuan = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Items/UI_Mission_ItemGargantualTreeRoot1_.png"); 
	inventoryBag = Engine::GetInstance().textures->Load("assets/UI/Inventario/UI_Inventari_Bag_01.png"); 
	uiCoin = Engine::GetInstance().textures->Load("assets/UI/Store/UI_Store_coin.png"); 
	cardsIcon = Engine::GetInstance().textures->Load("assets/UI/Inventario/UI_Tarot_InventoryItem1_.png");
	cardsIcon2 = Engine::GetInstance().textures->Load("assets/UI/Inventario/UI_Tarot_InventoryItem2_.png");
	arrowRight = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_BurronRight_.png");
	arrowLeft = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_ButtonLeft_.png");
	arrowRight2 = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_BurronRight2_.png");
	arrowLeft2 = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_ButtonLeft2_.png");
	cardsBase = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_Base_.png");

	auto invPrevPage = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 48, "",
		{ 610, 295, 50, 50 },
		this, SDL_Rect{ 0,0,0,0 },
		arrowLeft, arrowLeft2
	);
	if (invPrevPage) invPrevPage->visible = false;

	auto invNextPage = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 49, "",
		{ 1170, 295, 50, 50 }, 
		this, SDL_Rect{ 0,0,0,0 },
		arrowRight2, arrowRight
	);
	if (invNextPage) invNextPage->visible = false;

	auto cardButton = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 45, "",
		{ 1000, 425, 96, 96 }, 
		this, SDL_Rect{ 0,0,0,0 },
		cardsIcon, cardsIcon2
	);
	if (cardButton) cardButton->visible = false;

	auto previousCard = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 47, "",
		{ 690, 350, 50, 50 }, this, SDL_Rect{ 0,0,0,0 },
		arrowLeft2, arrowLeft
	);
	if (previousCard) previousCard->visible = false;

	auto nextCard = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 46, "",
		{ 920, 350, 50, 50 }, this, SDL_Rect{ 0,0,0,0 },
		arrowRight2, arrowRight
	);
	if (nextCard) nextCard->visible = false;
}

void Scene::SetInventory(bool inventory) {
	inventoryOn = inventory;
	
	for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
		if (element->id == 45) {
			element->visible = inventoryOn;
		}
		if (element->id == 46 || element->id == 47) {
			element->visible = (inventoryOn && cardsInventoryOn);
		}
	}
}

void Scene::CreateTeleportUI() {
	int x = 450;
	int y = 200;
	int buttonWidth = 250;
	int buttonHeight = 60;
	int spacing = 80;

	// LEVEL 1
	SDL_Texture* levelNormal = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Level1_Normal.png");
	SDL_Texture* levelClicked = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Level1_Clicked.png");
	auto btnLevel1 = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 60, "LEVEL 1",
		{ x, y, buttonWidth, buttonHeight },
		this, SDL_Rect{ 0,0,0,0 }, levelNormal, levelClicked
	);
	btnLevel1->visible = false;

	// LEVEL 2
	levelNormal = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Level2_Normal.png");
	levelClicked = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Level2_Clicked.png");
	auto btnLevel2 = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 61, "LEVEL 2",
		{ x, y + spacing, buttonWidth, buttonHeight },
		this, SDL_Rect{ 0,0,0,0 }, levelNormal, levelClicked
	);
	btnLevel2->visible = false;

	// LEVEL 3
	levelNormal = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Level3_Normal.png");
	levelClicked = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Level3_Clicked.png");
	auto btnLevel3 = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 62, "LEVEL 3",
		{ x, y + (spacing * 2), buttonWidth, buttonHeight },
		this, SDL_Rect{ 0,0,0,0 }, levelNormal, levelClicked
	);
	btnLevel3->visible = false;

	// LEVEL 4
	levelNormal = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Level4_Normal.png");
	levelClicked = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Level4_Clicked.png");
	auto btnLevel4 = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 63, "LEVEL 4",
		{ x, y + (spacing * 3), buttonWidth, buttonHeight },
		this, SDL_Rect{ 0,0,0,0 }, levelNormal, levelClicked
	);
	btnLevel4->visible = false;

	// EXIT
	SDL_Texture* exitNormal = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Exit_Normal.png");
	SDL_Texture* exitClicked = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Exit_Clicked.png");
	auto btnExit = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 64, "EXIT",
		{ x, y + (spacing * 4), buttonWidth, buttonHeight },
		this, SDL_Rect{ 0,0,0,0 }, exitNormal, exitClicked
	);
	btnExit->visible = false;

	// Ejemplo para textura, no he probado asi que no se si funciona.
	// teleportBg = Engine::GetInstance().textures->Load("assets/UI/Teleport/UI_Teleport_Background.png");
}


void Scene::SetTeleport(bool teleport) {
	teleportOn = teleport;

	for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
		if (element->id >= 60 && element->id <= 64) {
			element->visible = teleportOn;
		}
	}

	if (!teleportOn) {
		selectedTeleportingLevel = 0;
	}
}

void Scene::HandleTeleportUIEvents(UIElement* uiElement) {
	int targetLevel = 0;
	std::string targetMap;

	switch (uiElement->id) { // CAMBIAR targetMap por el mapa donde esté el object KingRat final con la propiedad int Level = targetLevel para cada caso
	case 60:  // LEVEL 1
		targetLevel = 1;
		targetMap = "TEST_map_LV1_startRoom_01.tmx";
		break;

	case 61:  // LEVEL 2
		targetLevel = 2;
		targetMap = "Map_LV2_bossTower.tmx";
		break;

	case 62:  // LEVEL 3
		targetLevel = 3;
		targetMap = "Map_LV3_left_01.tmx";
		break;

	case 63:  // LEVEL 4
		targetLevel = 4;
		targetMap = "Map_LV3_temple_01.tmx";
		break;

	case 64:  // EXIT
		SetTeleport(false);
		return;

	default:
		return;
	}

	// Cargar el mapa
	whereIsRat = targetLevel;
	LoadMap(targetMap);

	// Buscar el objeto RatKing con la propiedad Level = targetLevel
	auto ratKingSpawn = Engine::GetInstance().map->GetObjectByProperty("Entities", "Level", std::to_string(targetLevel));

	// Si encuentra el objeto, posicionar el jugador ahí
	if (ratKingSpawn != nullptr && player) {
		Vector2D spawnPos(ratKingSpawn->x, ratKingSpawn->y);
		player->SetPosition(spawnPos);
		player->pbody->SetPosition((int)spawnPos.getX(), (int)spawnPos.getY());
		player->respawnPosition = { PIXEL_TO_METERS(spawnPos.getX()), PIXEL_TO_METERS(spawnPos.getY()) };
		LOG("Jugador teletransportado a Level %d en posición (%.0f, %.0f)", targetLevel, spawnPos.getX(), spawnPos.getY());
	}
	else {
		LOG("ADVERTENCIA: No se encontró objeto RatKing con Level = %d", targetLevel);
	}

	selectedTeleportingLevel = targetLevel;
	SetTeleport(false);
}

// EL MAIN DE LAS PARTICULAS SEGUN EL TUTO LOL


//int main(int, char* argv[])
//{
//	SDL_Init(SDL_INIT_VIDEO);
//	auto win = SDL_CreateWindow("SDL3 Particles", 1024, 768, SDL_WINDOW_OPENGL);
//	auto ren = SDL_CreateRenderer(win, nullptr);
//
//	auto p = new ParticleExample();        // create a new particle system pointer
//	p->setRenderer(ren);                   // set the renderer
//	p->setPosition(512, 384);              // set the position
//	p->setStyle(ParticleExample::FIRE);    // set the example effects
//	p->setStartSpin(0);
//	p->setStartSpinVar(90);
//	p->setEndSpin(90);
//	p->setStartSpinVar(90);
//
//	bool running = true;
//	while (running)
//	{
//		SDL_Event e;
//		while (SDL_PollEvent(&e))
//		{
//			if (e.type == SDL_EVENT_KEY_UP)
//			{
//				int s = (e.key.key - SDLK_A + 1);
//				p->setStyle(ParticleExample::PatticleStyle(s));    // switch the example effects
//			}
//			if (e.type == SDL_EVENT_QUIT)
//			{
//				running = false;
//			}
//		}
//
//		SDL_RenderClear(ren);
//		p->draw();    // you have to draw it in each loop
//		SDL_RenderPresent(ren);
//		SDL_Delay(10);
//	}
//
//	SDL_DestroyRenderer(ren);
//	SDL_DestroyWindow(win);
//	delete p;    // destroy it
//
//	return 0;
//}

//MISSIONS

void Scene::MissionUI() {
	
	arrowRight = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_BurronRight_.png");
	arrowRight2 = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_BurronRight2_.png");
	arrowLeft = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_ButtonLeft_.png");
	arrowLeft2 = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_ButtonLeft2_.png");
	auto previousMission = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 65, "",
		{ 380, 350, 50, 50 }, this, SDL_Rect{ 0,0,0,0 },
		arrowLeft2, arrowLeft
	);
	if (previousMission) previousMission->visible = false;

	auto nextMission = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 66, "",
		{ 850, 350, 50, 50 }, this, SDL_Rect{ 0,0,0,0 },
		arrowRight2, arrowRight
	);
	if (nextMission) nextMission->visible = false;

}

void Scene::SetMissionUI() {

	for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
		if (element->id ==65 || element->id ==66) {
			element->visible = list;
		}
	}
}

//BOTON INVENTARIO

void Scene::InventariIconUI() {

	//BOTON INVENTARIO
	inventarioDrawing = Engine::GetInstance().textures->Load("assets/UI/Inventario/UI_Inventari_ScreenIcon2.png");
	inventarioclicado = Engine::GetInstance().textures->Load("assets/UI/Inventario/UI_Inventari_ScreenIcon_.png");
	
	misionesicono = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_Missions_Icon2.png");
	misionesiconoclicado = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_Missions_Icon1.png");
	misionesnoti = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_Missions_Icon2_notification.png");
	misionesnoticlicado = Engine::GetInstance().textures->Load("assets/UI/UI_Mission_Info/UI_Missions_Icon1_notification.png");

	cardsIcon = Engine::GetInstance().textures->Load("assets/UI/Inventario/UI_Tarot_InventoryItem1_.png");
	cardsIcon2 = Engine::GetInstance().textures->Load("assets/UI/Inventario/UI_Tarot_InventoryItem2_.png");
	auto bag = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 67, "",
		{ 1110, 560, 160, 160 }, this, SDL_Rect{ 0,0,0,0 },
		inventarioDrawing, inventarioclicado
	);
	if (bag) bag->visible = false;
	auto libreta = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 68, "",
		{ 1020, 590, 110, 110 }, this, SDL_Rect{ 0,0,0,0 },
		misionesicono, misionesiconoclicado
	);

	auto tarot = Engine::GetInstance().uiManager->CreateUIElement(
		UIElementType::BUTTON, 69, "",
		{ 940, 600, 100, 100 }, this, SDL_Rect{ 0,0,0,0 },
		cardsIcon, cardsIcon2
	);
	if (libreta) libreta->visible = false;
	if (tarot) tarot->visible = false;



}

void Scene::SetInventariIcon(bool on) {
	
	for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
		if (element->id == 67 || element->id == 68 || element->id == 69) {
			element->visible = on;
		}
	}
}

//tarot Logic

void Scene::TarotUI() {

	arrowRight = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_BurronRight_.png");
	arrowLeft = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_ButtonLeft_.png");
	arrowRight2 = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_BurronRight2_.png");
	arrowLeft2 = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_ButtonLeft2_.png");
	cardsBase = Engine::GetInstance().textures->Load("assets/UI/Tarot/UI_Tarot_Base_.png");
	
}

void Scene::SetTarotUI(bool on) {
	for (auto& element : Engine::GetInstance().uiManager->UIElementsList) {
		if (element->id == 46 || element->id == 47) {
			element->visible = cardsInventoryOn;
		}
	}
	if (on) {
		if (cardsBase != nullptr) {
			Engine::GetInstance().render->DrawTextureNoCamera(cardsBase, 240, 60, 800, 600);
		}

		if (cards.cards.size() > 0) {


			Engine::GetInstance().render->DrawTextureNoCamera(cards.cards[currentCardIndex].imagen, 730, 162, 200, 400);
		}


	}
}