#pragma once
#include <vector>
#include "pugixml.hpp"
#include <string>

struct SDL_Texture;

class ObjetoInventario {
public:
    std::string nombre; 
    SDL_Texture* imagen;
    SDL_Texture* imagenDescripcion;
    int i;

    ObjetoInventario(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc, int pos);
};

class Inventario {
public:
    std::vector<ObjetoInventario> objetos;

    Inventario();
    ~Inventario();

    void push(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc);
    void usarObjeto(int pos);
    void eliminarObjeto(std::string nom);
    bool tieneObjeto(std::string nom); 
    void SaveState(pugi::xml_node& node);
    void LoadState(pugi::xml_node node);
};