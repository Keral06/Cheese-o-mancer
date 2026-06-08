#include "Inventario.h"

ObjetoInventario::ObjetoInventario(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc, int pos) {
    nombre = nom;
    imagen = img;
    imagenDescripcion = imgDesc;
    i = pos;
}

Inventario::Inventario() {}
Inventario::~Inventario() { objetos.clear(); }

void Inventario::push(std::string nom, SDL_Texture* img, SDL_Texture* imgDesc) {
    int nuevaPosicion = objetos.size();
    objetos.push_back(ObjetoInventario(nom, img, imgDesc, nuevaPosicion));
}

void Inventario::usarObjeto(int pos) {
    if (pos >= 0 && pos < objetos.size()) {
        objetos[pos] = objetos.back();
        objetos[pos].i = pos;
        objetos.pop_back();
    }
}

bool Inventario::tieneObjeto(std::string nom) {
    for (int j = 0; j < objetos.size(); j++) {
        if (objetos[j].nombre == nom) {
            return true;
        }
    }
    return false;
}

void Inventario::eliminarObjeto(std::string nom) {
    for (int j = 0; j < objetos.size(); j++) {
        if (objetos[j].nombre == nom) {
            usarObjeto(j);
            break; 
        }
    }
}

void Inventario::SaveState(pugi::xml_node& node) {
    for (int j = 0; j < objetos.size(); j++) {
        pugi::xml_node itemNode = node.append_child("item");
        itemNode.append_attribute("nombre") = objetos[j].nombre.c_str();
    }
}

void Inventario::LoadState(pugi::xml_node node) {
    objetos.clear();
    for (pugi::xml_node itemNode = node.child("item"); itemNode; itemNode = itemNode.next_sibling("item")) {
        std::string nom = itemNode.attribute("nombre").as_string();
    }
}