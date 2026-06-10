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

bool ListaMisiones::Visualizada() {

    for (int j = 0; j < objetos.size(); j++) {
        if (objetos[j].visualizada == false) {
            return false;
        }
    }
    return true;



}
void ListaMisiones::SaveState(pugi::xml_node& node) {
    for (int j = 0; j < objetos.size(); j++) {
        pugi::xml_node misionNode = node.append_child("mision");
        misionNode.append_attribute("nombre") = objetos[j].nombre.c_str();
        misionNode.append_attribute("visualizada") = objetos[j].visualizada;
        misionNode.append_attribute("completed") = objetos[j].completed;
    }
}

void ListaMisiones::LoadState(pugi::xml_node node) {
    objetos.clear();
    for (pugi::xml_node misionNode = node.child("mision"); misionNode; misionNode = misionNode.next_sibling("mision")) {
        std::string nom = misionNode.attribute("nombre").as_string();

        push(nom, nullptr, nullptr);

        objetos.back().visualizada = misionNode.attribute("visualizada").as_bool();
        objetos.back().completed = misionNode.attribute("completed").as_bool();
    }
}


int ListaMisiones::HowManyCompleted() {

    int completed = 0;
    for (int j = 0; j < objetos.size(); j++) {
        if (objetos[j].visualizada) {
        
            completed++;
        }
    }
    return completed;

}