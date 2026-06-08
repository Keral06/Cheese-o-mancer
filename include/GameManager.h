#pragma once
#include "Module.h"
#include "pugixml.hpp"

class GameManager : public Module
{
public:
    GameManager();
    virtual ~GameManager();

    bool Start() override;
    bool CleanUp() override;
    void StartNewGame();
    void SaveGame();
    void LoadGame();
};