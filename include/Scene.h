#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "UIButton.h"
#include "Animation.h"
#include "Inventario.h"
#include "ListaMisiones.h"
#include "TarotCards.h"
#include "pl_mpeg.h"

class BossFightPrincessKnight;

struct SDL_Texture;

enum class SceneID
{
	INTRO_SCREEN,
	MAIN_MENU,
	IN_GAME,
	GAME_OVER,
	WIN_SCREEN,
	FINAL_WIN,	
	CUTSCENE
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
	void SetInventariIcon(bool on);
	bool continueGame = false;
	int savedLevel = 1;
	bool isPaused = false;
	bool showHelp = true;
	bool firstMapLoad = true;
	bool storeOn = false;
	bool inventoryOn = false;
	int currentInvPage = 0;
	bool cardsInventoryOn = false;
	int currentCardIndex = 0;
	int totalUnlockedCards = 1;
	bool showUIDebug = false;
	bool teleportOn = false;
	int selectedTeleportingLevel = 0;
	SDL_Texture* teleportBg = nullptr;
	int currentMission = 0;
	int actualHelpTexture = 0;
	Scene();
	// help

	void HelpUI();

	void SetHelpUI(bool aaa);
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

	void SetPlayer(std::shared_ptr<Player> _player);
	

	BossFightPrincessKnight* GetBossFightController() const { return bossFightController; }

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

	//tarot

	void TarotUI();

	void SetTarotUI(bool aaa);
	
	

	bool tarot = false;
	

	//Funciones Store
	void CreateStoreLevel1();
	void CreateStoreLevel2();
	void CreateStoreLevel3();

	//Funciones para el inventario
	void CreateInventoryUI();

	//Funciones misiones
	void MissionUI();
	void SetMissionUI();
	SDL_Texture* missionTexture = nullptr;

	// Texturas del inventario
	SDL_Texture* invPaperCombined = nullptr;
	SDL_Texture* iconMap = nullptr;
	SDL_Texture* iconKey = nullptr;
	SDL_Texture* iconLamp = nullptr;
	SDL_Texture* iconSpring = nullptr;
	SDL_Texture* iconHorseMacure = nullptr;
	SDL_Texture* iconGargantuan = nullptr;
	SDL_Texture* inventoryBag = nullptr;
	SDL_Texture* uiCoin = nullptr;

	//Texturas tarot
	SDL_Texture* cardsIcon = nullptr;
	SDL_Texture* cardsIcon2 = nullptr;
	SDL_Texture* cardsBase = nullptr;
	SDL_Texture* cardTheFool = nullptr;
	SDL_Texture* cardTheMagician = nullptr;
	SDL_Texture* cardWheelOfFortune = nullptr;
	SDL_Texture* arrowRight = nullptr;
	SDL_Texture* arrowLeft = nullptr;
	SDL_Texture* arrowRight2 = nullptr;
	SDL_Texture* arrowLeft2 = nullptr;
	//Texturas
	SDL_Texture* inventarioDrawing = nullptr;
	SDL_Texture* inventarioclicado = nullptr;
	SDL_Texture* inventarionotificacion = nullptr;
	SDL_Texture* inventarionotificacion2 = nullptr;

	SDL_Texture* misionesicono = nullptr;
	SDL_Texture* misionesiconoclicado = nullptr;
	SDL_Texture* misionesnoti = nullptr;
	SDL_Texture* misionesnoticlicado = nullptr;
	void InventariIconUI();
	
	void UpdatePauseMenu(float dt);
	void HandleStoreUIEvents(UIElement* uiElement);

	BossFightPrincessKnight* bossFightController = nullptr;

public:
	//Funciones para el teletransporte
	void CreateTeleportUI();
	void SetTeleport(bool teleport);
	void HandleTeleportUIEvents(UIElement* uiElement);
	std::vector<SDL_Texture*> helpTextures;
	
private:

	//L03: TODO 3b: Declare a Player attribute
	std::shared_ptr<Player> player;
	std::string tilePosDebug = "[0,0]";
	SDL_Texture* helpTexture = nullptr;
	SDL_Texture* map1Texture = nullptr;
	
	SDL_Texture* map2Texture = nullptr;
	SDL_Texture* map3Texture = nullptr;
	SDL_Texture* mapToShow = nullptr;
	
	bool showMap = false;
	SDL_Texture* introTexture = nullptr;
	SDL_Texture* heartTexture = nullptr;
	SDL_Texture* panelTexture = nullptr;
	SDL_Texture* heart1Texture = nullptr;
	SDL_Texture* heart2Texture = nullptr;
	SDL_Texture* heart3Texture = nullptr;
	SDL_Texture* heart4Texture = nullptr;
	SDL_Texture* quesoQuintos_5 = nullptr; 
	SDL_Texture* quesoQuintos_4 = nullptr; 
	SDL_Texture* quesoQuintos_3 = nullptr; 
	SDL_Texture* quesoQuintos_2 = nullptr; 
	SDL_Texture* quesoQuintos_1 = nullptr;
	SDL_Texture* quesoSextos_6 = nullptr;
	SDL_Texture* quesoSextos_5 = nullptr;
	SDL_Texture* quesoSextos_4 = nullptr;
	SDL_Texture* quesoSextos_3 = nullptr;
	SDL_Texture* quesoSextos_2 = nullptr;
	SDL_Texture* quesoSextos_1 = nullptr;
	SDL_Texture* extraHeartTexture = nullptr;

	std::vector<std::shared_ptr<Enemy>> enemies;

	std::shared_ptr<UIButton> uiBt;
	float volume = 1.0;
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
	SDL_Texture* storePaperPermLife = nullptr;
	SDL_Texture* storePaperLife = nullptr;
	SDL_Texture* storePaperKey = nullptr;
	SDL_Texture* storePaperDamage = nullptr;
	int selectedStoreItem = 0;


public:
	void SetStore(bool store, int storeID = 1);
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
	int score = 0;
	bool hasDamagePlus = false;
	bool hasTalkedMagician = false;

	//inventario
	Inventario inventario;
	//MARC ESTOS SON LOS BOOLS DE EL PLAYER PARA CONVERSACIONES

	
	bool beatBoss = false;
	bool beatPrincess = false;	
	bool extralife = false;
	int maxLives = 4;

	//BOOLS OF THE PAPERS
	bool day = false;
	bool night = false;
	bool dusk = false;
	bool dawn = false;

	bool hasAllPoems = false;

	//Hermit bools for player
	bool springWater = false;
	bool HorsekinManure = false;
	bool Gargantuan = false;
	//for hermit

	bool springWaterHermit = false;
	bool HorsekinManureHermit = false;
	bool GargantuanHermit = false;
	bool finishedmissionHermit = false;

	bool hasTalkedHermit = false;

	//Tarot bools
	TarotCards cards;

	//Misiones

	ListaMisiones misiones;
	bool list = false;

	//dialogue movement restriction

	bool someoneIsTalking = false;
	bool ObjectObserved = false;

	//psalms

	bool psalm1 = false;
	bool psalm2 = false;
	bool psalm3 = false;
	bool hasReadAllPsalms = false;
	bool hasTalkedAboutPsalmsB4 = false;
	bool DefeatedHighPrietest = false;
	bool hasBeenWhistledblowed = false;

	// MiniBoss
	void StartMiniBoss();
	void CheckMiniBossStatus();
	bool miniBossFinished = false;

	//Well's echo bools

	bool hasTalkedWell = false;
	bool hasShownPoemToWell = false;

	//level 3 bools

	bool talkedTiredPreacher = false;


		//Little Finley
		

		int hidingPlaceATM = 0;
		bool hasFoundTimmyThreeTimes = false;
		bool hasTalkedToTimmyOnce = false;
		bool talkedOnceAfterDefeatBoss = false;

		//Empress

		bool EmpressTrustedDialogue = false;
		bool hasAllFragments = false;
		bool hasSparedPrincessAndKnight = false;
		bool hasTalkedOnceEmpress = false;
		bool talkedTwiceEmpress = false;

		//Sculptor
		bool hasTalkedSculptor = false;
		bool finishedMissionSculptor = false;
		//teleport variables

		bool teleportUnlocked = false;
		bool ratTalkedOnce = false;
		bool ratmissionfinished = false;
		int whereIsRat = 1;

private:
	//all video stuff
	struct VideoData {
		plm_t* plm = nullptr;
		SDL_Texture* texture = nullptr;
		uint8_t* buffer = nullptr;
		int width = 0;
		int height = 0;		
	};
	
	bool isPlayingVideo = false;
	VideoData  video;
	

	void LoadVideo(VideoData* video, const char* name);
	void PlayVideo(const char* name);
	void StopVideo();

	static void OnVideoFrame(plm_t* mpeg, plm_frame_t* frame, void* user);
};