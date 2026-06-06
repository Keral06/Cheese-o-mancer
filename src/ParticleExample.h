#pragma once
#include "ParticleSystem.h"
#include "SDL3_image/SDL_image.h"

class ParticleExample : public ParticleSystem
{
public:
    ParticleExample() {}
    virtual ~ParticleExample() {}

    enum PatticleStyle
    {
        NONE,
        FIRE,
        MOHO,
        /*FIRE_WORK,
        SUN,
        GALAXY,
        FLOWER,*/
        METEOR,
        SPIRAL,
        EXPLOSION,
        SMOKE,
        SNOW,
        RAIN,
    };

    PatticleStyle style_ = NONE;
    void setStyle(PatticleStyle style);

    SDL_Texture* getDefaultTexture()
    {
        static SDL_Texture* t = IMG_LoadTexture(_renderer, "assets/Textures/Particles/fire_particle.png");

        if (t == nullptr) {
            // Esto imprimirá un error en la consola si no encuentra la imagen
            printf("Error cargando textura de particulas: %s\n", SDL_GetError());
        }
        return t;
    }
};
