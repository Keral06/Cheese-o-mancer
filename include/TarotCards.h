#pragma once
#include <vector>
#include <string>

struct SDL_Texture;

class Card {
public:
    std::string nombre; 
    SDL_Texture* imagen;
    SDL_Texture* imagenGirada;
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
};