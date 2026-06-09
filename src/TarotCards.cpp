#include "TarotCards.h"

Card::Card(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc, int pos) {
    nombre = nom;
    imagen = img;
    imagenGirada = imgDesc;
    i = pos;
}

TarotCards::TarotCards() {}
TarotCards::~TarotCards() { cards.clear(); }

void TarotCards::push(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc) {
    int nuevaPosicion = cards.size();
    cards.push_back(Card(nom, img, imgDesc, nuevaPosicion));
}

void TarotCards::usarObjeto(int pos) {
    if (pos >= 0 && pos < cards.size()) {
        cards[pos] = cards.back();
        cards[pos].i = pos;
        cards.pop_back();
    }
}

bool TarotCards::tieneObjeto(std::string nom) {
    for (int j = 0; j < cards.size(); j++) {
        if (cards[j].nombre == nom) {
            return true;
        }
    }
    return false;
}

void TarotCards::GirarCarta(std::string nom) {
    for (int j = 0; j < cards.size(); j++) {
        if (cards[j].nombre == nom) {
            cards[j].imagen = cards[j].imagenGirada; 
            break; 
        }
    }
}

void TarotCards::SaveState(pugi::xml_node& node) {
    for (int j = 0; j < cards.size(); j++) {
        pugi::xml_node cardNode = node.append_child("Card");
        cardNode.append_attribute("nombre").set_value(cards[j].nombre.c_str());
    }
}

void TarotCards::LoadState(pugi::xml_node& node) {
    cards.clear(); 
    for (pugi::xml_node cardNode = node.child("Card"); cardNode; cardNode = cardNode.next_sibling("Card")) {
        std::string nom = cardNode.attribute("nombre").as_string();

        push(nom, nullptr, nullptr);
    }
}