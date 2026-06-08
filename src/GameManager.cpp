#include "GameManager.h"
#include "Engine.h"
#include "Scene.h"
#include "Player.h"
#include "Log.h"

GameManager::GameManager() : Module() {
    name = "gameManager";
}

GameManager::~GameManager() {}

bool GameManager::Start() {
    return true;
}

bool GameManager::CleanUp() {
    return true;
}

void GameManager::SaveGame() {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("SaveState");

    Scene* scene = Engine::GetInstance().scene.get();
    Player* player = scene->GetPlayer();

    // Player y mundo
    pugi::xml_node world = root.append_child("WorldState");
    world.append_attribute("savedLevel").set_value(scene->savedLevel);
    world.append_attribute("lives").set_value(scene->lives);
    world.append_attribute("maxLives").set_value(scene->maxLives);
    world.append_attribute("score").set_value(scene->score);

    if (player != nullptr) {
        pugi::xml_node pos = world.append_child("Position");
        pos.append_attribute("x").set_value(player->GetPosition().getX());
        pos.append_attribute("y").set_value(player->GetPosition().getY());
    }

    // Historia?
    pugi::xml_node story = root.append_child("StoryFlags");

    // Cosas de misiones y boses
    story.append_attribute("beatBoss").set_value(scene->beatBoss);
    story.append_attribute("beatPrincess").set_value(scene->beatPrincess);
    story.append_attribute("hasAllPoems").set_value(scene->hasAllPoems);
    story.append_attribute("teleportUnlocked").set_value(scene->teleportUnlocked);
    story.append_attribute("hasTalkedWell").set_value(scene->hasTalkedWell);
    story.append_attribute("day").set_value(scene->day);
    story.append_attribute("night").set_value(scene->night);
    story.append_attribute("dusk").set_value(scene->dusk);
    story.append_attribute("dawn").set_value(scene->dawn);
    story.append_attribute("springWaterHermit").set_value(scene->springWaterHermit);
    story.append_attribute("HorsekinManureHermit").set_value(scene->HorsekinManureHermit);
    story.append_attribute("GargantuanHermit").set_value(scene->GargantuanHermit);
    story.append_attribute("finishedmissionHermit").set_value(scene->finishedmissionHermit);
    story.append_attribute("hasTalkedHermit").set_value(scene->hasTalkedHermit);
    story.append_attribute("psalm1").set_value(scene->psalm1);
    story.append_attribute("psalm2").set_value(scene->psalm2);
    story.append_attribute("psalm3").set_value(scene->psalm3);
    story.append_attribute("EmpressTrustedDialogue").set_value(scene->EmpressTrustedDialogue);
    story.append_attribute("hasTalkedSculptor").set_value(scene->hasTalkedSculptor);

    // Mejoras del jugador
    story.append_attribute("hasDamagePlus").set_value(scene->hasDamagePlus);
    story.append_attribute("extralife").set_value(scene->extralife);

    // Magician
    story.append_attribute("hasTalkedMagician").set_value(scene->hasTalkedMagician);

    // Hermit
    story.append_attribute("springWater").set_value(scene->springWater);
    story.append_attribute("HorsekinManure").set_value(scene->HorsekinManure);
    story.append_attribute("Gargantuan").set_value(scene->Gargantuan);

    // Psalms & Preacher
    story.append_attribute("hasReadAllPsalms").set_value(scene->hasReadAllPsalms);
    story.append_attribute("hasTalkedAboutPsalmsB4").set_value(scene->hasTalkedAboutPsalmsB4);
    story.append_attribute("DefeatedHighPrietest").set_value(scene->DefeatedHighPrietest);
    story.append_attribute("hasBeenWhistledblowed").set_value(scene->hasBeenWhistledblowed);
    story.append_attribute("talkedTiredPreacher").set_value(scene->talkedTiredPreacher);

    // Well 
    story.append_attribute("hasShownPoemToWell").set_value(scene->hasShownPoemToWell);

    // Little Finley 
    story.append_attribute("hasFoundTimmyThreeTimes").set_value(scene->hasFoundTimmyThreeTimes);
    story.append_attribute("hasTalkedToTimmyOnce").set_value(scene->hasTalkedToTimmyOnce);
    story.append_attribute("talkedOnceAfterDefeatBoss").set_value(scene->talkedOnceAfterDefeatBoss);
    story.append_attribute("hidingPlaceATM").set_value(scene->hidingPlaceATM);

    // Empress
    story.append_attribute("hasAllFragments").set_value(scene->hasAllFragments);
    story.append_attribute("hasSparedPrincessAndKnight").set_value(scene->hasSparedPrincessAndKnight);
    story.append_attribute("hasTalkedOnceEmpress").set_value(scene->hasTalkedOnceEmpress);
    story.append_attribute("talkedTwiceEmpress").set_value(scene->talkedTwiceEmpress);

    // Sculptor
    story.append_attribute("finishedMissionSculptor").set_value(scene->finishedMissionSculptor);

    // Rat King
    story.append_attribute("ratTalkedOnce").set_value(scene->ratTalkedOnce);
    story.append_attribute("ratmissionfinished").set_value(scene->ratmissionfinished);
    story.append_attribute("whereIsRat").set_value(scene->whereIsRat);

    // INVENTARIO Y MISIONES
    pugi::xml_node invNode = root.append_child("Inventory");
    scene->inventario.SaveState(invNode);

    pugi::xml_node missionsNode = root.append_child("Missions");
    scene->misiones.SaveState(missionsNode);

    // Guardar 
    if (doc.save_file("savegame.xml")) {
        LOG("Partida guardada");
    }
    else {
        LOG("Error al guardar la partida.");
    }
}

void GameManager::LoadGame() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("savegame.xml");

    if (result) {
        pugi::xml_node root = doc.child("SaveState");
        Scene* scene = Engine::GetInstance().scene.get();
        Player* player = scene->GetPlayer();

        // Player y mundo
        pugi::xml_node world = root.child("WorldState");
        scene->savedLevel = world.attribute("savedLevel").as_int();
        scene->lives = world.attribute("lives").as_int();
        scene->maxLives = world.attribute("maxLives").as_int();
        scene->score = world.attribute("score").as_int();

        if (player != nullptr) {
            pugi::xml_node pos = world.child("Position");
            float x = pos.attribute("x").as_float();
            float y = pos.attribute("y").as_float();
            player->SetPosition(Vector2D(x, y));
        }

        // Historia?
        pugi::xml_node story = root.child("StoryFlags");

        // Cosas de misiones y boses
        scene->beatBoss = story.attribute("beatBoss").as_bool();
        scene->beatPrincess = story.attribute("beatPrincess").as_bool();
        scene->hasAllPoems = story.attribute("hasAllPoems").as_bool();
        scene->teleportUnlocked = story.attribute("teleportUnlocked").as_bool();
        scene->hasTalkedWell = story.attribute("hasTalkedWell").as_bool();
        scene->day = story.attribute("day").as_bool();
        scene->night = story.attribute("night").as_bool();
        scene->dusk = story.attribute("dusk").as_bool();
        scene->dawn = story.attribute("dawn").as_bool();
        scene->springWaterHermit = story.attribute("springWaterHermit").as_bool();
        scene->HorsekinManureHermit = story.attribute("HorsekinManureHermit").as_bool();
        scene->GargantuanHermit = story.attribute("GargantuanHermit").as_bool();
        scene->finishedmissionHermit = story.attribute("finishedmissionHermit").as_bool();
        scene->hasTalkedHermit = story.attribute("hasTalkedHermit").as_bool();
        scene->psalm1 = story.attribute("psalm1").as_bool();
        scene->psalm2 = story.attribute("psalm2").as_bool();
        scene->psalm3 = story.attribute("psalm3").as_bool();
        scene->EmpressTrustedDialogue = story.attribute("EmpressTrustedDialogue").as_bool();
        scene->hasTalkedSculptor = story.attribute("hasTalkedSculptor").as_bool();

        // Mejoras del jugador
        scene->hasDamagePlus = story.attribute("hasDamagePlus").as_bool();
        scene->extralife = story.attribute("extralife").as_bool();

        // Magician
        scene->hasTalkedMagician = story.attribute("hasTalkedMagician").as_bool();

        // Hermit 
        scene->springWater = story.attribute("springWater").as_bool();
        scene->HorsekinManure = story.attribute("HorsekinManure").as_bool();
        scene->Gargantuan = story.attribute("Gargantuan").as_bool();

        // Psalms & Preacher
        scene->hasReadAllPsalms = story.attribute("hasReadAllPsalms").as_bool();
        scene->hasTalkedAboutPsalmsB4 = story.attribute("hasTalkedAboutPsalmsB4").as_bool();
        scene->DefeatedHighPrietest = story.attribute("DefeatedHighPrietest").as_bool();
        scene->hasBeenWhistledblowed = story.attribute("hasBeenWhistledblowed").as_bool();
        scene->talkedTiredPreacher = story.attribute("talkedTiredPreacher").as_bool();

        // Well 
        scene->hasShownPoemToWell = story.attribute("hasShownPoemToWell").as_bool();

        // Little Finley 
        scene->hasFoundTimmyThreeTimes = story.attribute("hasFoundTimmyThreeTimes").as_bool();
        scene->hasTalkedToTimmyOnce = story.attribute("hasTalkedToTimmyOnce").as_bool();
        scene->talkedOnceAfterDefeatBoss = story.attribute("talkedOnceAfterDefeatBoss").as_bool();
        scene->hidingPlaceATM = story.attribute("hidingPlaceATM").as_int();

        // Empress
        scene->hasAllFragments = story.attribute("hasAllFragments").as_bool();
        scene->hasSparedPrincessAndKnight = story.attribute("hasSparedPrincessAndKnight").as_bool();
        scene->hasTalkedOnceEmpress = story.attribute("hasTalkedOnceEmpress").as_bool();
        scene->talkedTwiceEmpress = story.attribute("talkedTwiceEmpress").as_bool();

        // Sculptor
        scene->finishedMissionSculptor = story.attribute("finishedMissionSculptor").as_bool();

        // Rat King
        scene->ratTalkedOnce = story.attribute("ratTalkedOnce").as_bool();
        scene->ratmissionfinished = story.attribute("ratmissionfinished").as_bool();
        scene->whereIsRat = story.attribute("whereIsRat").as_int();

        // INVENTARIO Y MISIONES
        scene->inventario.LoadState(root.child("Inventory"));
        scene->misiones.LoadState(root.child("Missions"));

        LOG("Partida cargada exitosamente.");
    }
    else {
        LOG("No se encontro un archivo de guardado o hubo un error: %s", result.description());
    }
}
void GameManager::StartNewGame() {
    Scene* scene = Engine::GetInstance().scene.get();
    scene->levelTimer = 0.0f;
    scene->score = 0;
    scene->lives = 4;
    scene->maxLives = 4;
    scene->extralife = false;
    scene->hasDamagePlus = false;
    scene->savedLevel = 1;

    scene->inventario.objetos.clear();
    scene->misiones.objetos.clear();
    scene->cards.cards.clear();

    scene->list = false;
    scene->cardsInventoryOn = false;
    scene->inventoryOn = false;

    scene->currentMission = 0;
    scene->currentInvPage = 0;
    scene->currentCardIndex = 0;

    scene->hasTalkedMagician = false;
    scene->beatBoss = false;
    scene->beatPrincess = false;
    scene->cheese = false;

    Player* player = scene->GetPlayer();
    if (player != nullptr) {
        player->extralife = false;
        player->isDeadDefinitive = false;
    }

}