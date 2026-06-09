#pragma once
#include <vector>
#include <string>
#include "pugixml.hpp"
struct SDL_Texture;

class Card {
public:
    std::string nombre; 
    SDL_Texture* imagen;
    SDL_Texture* imagenGirada;
    bool visualizada = false;

    int i;

    Card(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc, int pos);
};

class TarotCards {
public:
    std::vector<Card> cards;

    TarotCards();
    ~TarotCards();

    void push(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc);
    void usarObjeto(int pos);
    void GirarCarta(std::string nom);
    bool tieneObjeto(std::string nom);
    void SaveState(pugi::xml_node& node);
    void LoadState(pugi::xml_node& node); void hasBeenSeen(std::string nom);
    bool Visualizada();


};