#include "ListaMisiones.h"

Mision::Mision(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc, int pos) {
    nombre = nom;
    imagen = img;
    imagenFinalizada = imgDesc;
    i = pos;
}

ListaMisiones::ListaMisiones() {}
ListaMisiones::~ListaMisiones() { objetos.clear(); }

void ListaMisiones::push(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc) {
    int nuevaPosicion = objetos.size();
    objetos.push_back(Mision(nom, img, imgDesc, nuevaPosicion));
}

void ListaMisiones::Completed(std::string nom) { // para cuando se ha completado la mision
    
    for (int j = 0; j < objetos.size(); j++) {
        if (objetos[j].nombre == nom) {
            objetos[j].completed = true;
            objetos[j].imagen = objetos[j].imagenFinalizada;
            break;
        }
    }
   
}

bool ListaMisiones::tieneObjeto(std::string nom) {
    for (int j = 0; j < objetos.size(); j++) {
        if (objetos[j].nombre == nom) {
            return true;
        }
    }
    return false;
}

void ListaMisiones::hasBeenSeen(std::string nom) {// para cuando se ha visualizado el objeto
    for (int j = 0; j < objetos.size(); j++) {
        if (objetos[j].nombre == nom) {
            objetos[j].visualizada = true;
            break; 
        }
    }
}