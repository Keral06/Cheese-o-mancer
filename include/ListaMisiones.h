#pragma once
#include <vector>
#include <string>
#include "pugixml.hpp"

struct SDL_Texture;

class Mision {
public:
    std::string nombre; 
    SDL_Texture* imagen;
    SDL_Texture* imagenFinalizada;
    bool visualizada = false;
    bool completed = false;

    int i;

    Mision(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc, int pos);
};

class ListaMisiones {
public:
    std::vector<Mision> objetos;

    ListaMisiones();
    ~ListaMisiones();

    void push(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc);

    void hasBeenSeen(std::string nom);
    void Completed(std::string nom);
    bool tieneObjeto(std::string nom);
    bool Visualizada();
    void SaveState(pugi::xml_node& node);
    void LoadState(pugi::xml_node node);
    int HowManyCompleted();
};