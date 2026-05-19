#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "UIButton.h"
#include "Animation.h"

struct SDL_Texture;

enum class SceneID
{
	INTRO_SCREEN,
	MAIN_MENU,
	IN_GAME,
	GAME_OVER,
	WIN_SCREEN,
	FINAL_WIN
};

enum Level {
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4
};

enum Areas1 {
	START,
	TOWER,
	PANTRY,
	TORTURE,
	BOSS
};

class Scene : public Module
{
public:
	int savedLevel = 1;
	bool isPaused = false;
	bool showHelp = true;
	bool storeOn = false;
	bool inventoryOn = false;
	bool cardsInventoryOn = false;
	int currentCardIndex = 0;
	int totalUnlockedCards = 1;
	bool showUIDebug = false;
	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	bool IsGamePaused() const
	{
		return showHelp;
	}
	// Return the player position
	Vector2D GetPlayerPosition();

	// Get tilePosDebug value
	std::string GetTilePosDebug() {
		return tilePosDebug;
	}

	bool OnUIMouseClickEvent(UIElement* uiElement);
	float levelTimer = 0.0f;
	void ChangeScene(SceneID newScene);
	void UnloadCurrentScene();
	void LoadScene(SceneID newScene);
	SceneID GetCurrentScene()
	{
		return currentScene;
	}
	SceneID GetLastScene()
	{
		return lastscene;
	}
	void SaveLevel();
	void LoadGame();
	void LoadMap(std::string map);

	void StartFadeOut(float duration);

	void StartFadeIn(float duration);

	Player* Scene::GetPlayer() const;
private:
	//Introscreen functions
	void LoadIntro();
	void UpdateIntro(float dt);
	void UnloadIntro();

	

	// L17 TODO 3: Define specific function for main menu scene: Load, Unload, Handle UI events
	void LoadMainMenu();
	void UnloadMainMenu();
	void UpdateMainMenu(float dt);
	void HandleMainMenuUIEvents(UIElement* uiElement);

	// L17 TODO 4: Define specific functions for level1 scene: Load, Unload, Update, PostUpdate
	void UnloadLevel();
	void UpdateLevel(float dt);
	void PostUpdateLevel();

	// Funciones para la pantalla de Game Over
	void LoadGameOver(); 
	void UpdateGameOver(float dt); 
	void UnloadGameOver();
	void HandleGameOverUIEvents(UIElement* uiElement);

	// Funciones para Win Screen
	void LoadWinScreen();           
	void UpdateWinScreen(float dt);  
	void UnloadWinScreen();          


	// Funciones para el Menú de Pausa
	void CreatePauseUI();            
	void UpdatePauseMenu();           
	void HandlePauseUIEvents(UIElement* uiElement); 
	void SetPause(bool pause);        

	// Funciones para Win Screen final
	void LoadFinalWin();
	void UpdateFinalWin(float dt);
	void UnloadFinalWin();
	void HandleFinalWinUIEvents(UIElement* uiElement);
	

	//Funciones Store
	void CreateStoreLevel1();

	//Funciones para el inventario
	void CreateInventoryUI();

	// Texturas del inventario
	SDL_Texture* invPaperCombined = nullptr;
	SDL_Texture* iconMap = nullptr;
	SDL_Texture* iconKey = nullptr;
	SDL_Texture* iconLamp = nullptr;
	SDL_Texture* inventoryBag = nullptr;
	SDL_Texture* uiCoin = nullptr;

	//Texturas tarot
	SDL_Texture* cardsBase = nullptr;
	SDL_Texture* cardTheFool = nullptr;
	SDL_Texture* cardTheMagician = nullptr;
	SDL_Texture* arrowRight = nullptr;
	SDL_Texture* arrowLeft = nullptr;

	void UpdatePauseMenu(float dt);
	void HandleStoreUIEvents(UIElement* uiElement);

	
private:

	//L03: TODO 3b: Declare a Player attribute
	std::shared_ptr<Player> player;
	std::string tilePosDebug = "[0,0]";
	SDL_Texture* helpTexture = nullptr;
	SDL_Texture* map1Texture = nullptr;
	
	bool showMap = false;
	SDL_Texture* introTexture = nullptr;
	SDL_Texture* heartTexture = nullptr;
	SDL_Texture* panelTexture = nullptr;
	SDL_Texture* heart1Texture = nullptr;
	SDL_Texture* heart2Texture = nullptr;
	SDL_Texture* heart3Texture = nullptr;
	SDL_Texture* heart4Texture = nullptr;
	SDL_Texture* extraHeartTexture = nullptr;

	std::vector<std::shared_ptr<Enemy>> enemies;

	std::shared_ptr<UIButton> uiBt;
	float volume = 1.0;
	bool continueGame = false;
	bool exitGame = false;
	bool showCredits = false;
	SceneID currentScene = SceneID::MAIN_MENU;
	SceneID lastscene = SceneID::MAIN_MENU;
	SDL_Texture* loseTexture = nullptr;
	AnimationSet loseAnimSet;
	bool slidersOn = false;
	SDL_Texture* Volume = nullptr;
	SDL_Texture* VolumeEffects = nullptr;
	SDL_Texture* fullscreen = nullptr;
	
	SDL_Texture* storeBag = nullptr;
	SDL_Texture* storePaperMap = nullptr;
	SDL_Texture* storePaperLife = nullptr;
	SDL_Texture* storePaperKey = nullptr;
	int selectedStoreItem = 0;


public:
	void SetStore(bool store);
	void SetInventory(bool inventory);
	SDL_Rect rectInvMap = { 0, 0, 0, 0 };
	std::string nextMap = "";
	std::string nextSpawnPoint = "Door_1_1";
	std::string nextDoor = "";
	bool firstDoor = true;
	bool cheese = false;

	bool isFading = false;
	bool fadeIn = false;   // true = fade in, false = fade out
	float fadeTime = 0.0f;
	float fadeDuration = 1.0f; // segundos
	Uint8 fadeAlpha = 0;

	int lives = 4;
	bool hasTalkedMagician = false;


	//MARC ESTOS SON LOS BOOLS DE EL PLAYER PARA CONVERSACIONES

	
	bool beatBoss = false;
	
	bool extralife = false;

	//BOOLS OF THE PAPERS
	bool day = false;
	bool night = false;
	bool dusk = false;
	bool dawn = false;

	//Hermit bools for player
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;
	//for hermit

	bool springWaterHermit = false;
	bool HorsekinManureHermit = false;
	bool GargantuanHermit = false;
	bool finishedmissionHermit = false;

};