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

    void SaveGame();
    void LoadGame();
};