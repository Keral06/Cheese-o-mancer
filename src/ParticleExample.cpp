#include "ParticleExample.h"
#include "Engine.h"   
#include "Textures.h"

void ParticleExample::setStyle(PatticleStyle style)
{
    if (style_ == style)
    {
        return;
    }
    style_ = style;
    if (style == NONE)
    {
        stopSystem();
    }
    if (_texture == nullptr)
    {
        setTexture(getDefaultTexture());
    }
    switch (style)
    {

    case ParticleExample::FIRE:
    {
        initWithTotalParticles(5);

        // duration
        _duration = DURATION_INFINITY;

        // Gravity Mode
        this->_emitterMode = Mode::GRAVITY;

        // Gravity Mode: gravity
        this->modeA.gravity = { 0, 0 };

        // Gravity Mode: radial acceleration
        this->modeA.radialAccel = 0;
        this->modeA.radialAccelVar = 0;

        // Gravity Mode: speed of particles
        this->modeA.speed = -60;
        this->modeA.speedVar = 20;

        // starting angle
        _angle = 90;
        _angleVar = 10;

        // life of particles
        _life = 3;
        _lifeVar = 0.25f;

        // size, in pixels
        _startSize = 54.0f;
        _startSizeVar = 10.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        // emits per frame
        _emissionRate = _totalParticles / _life;

        // color of particles
        _startColor.r = 0.76f;
        _startColor.g = 0.25f;
        _startColor.b = 0.12f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.0f;
        _startColorVar.g = 0.0f;
        _startColorVar.b = 0.0f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.0f;
        _endColor.g = 0.0f;
        _endColor.b = 0.0f;
        _endColor.a = 0.0f;
        _endColorVar.r = 0.0f;
        _endColorVar.g = 0.0f;
        _endColorVar.b = 0.0f;
        _endColorVar.a = 0.0f;

        _posVar = { 40.0f, 20.0f };
        break;
    }

    case ParticleExample::SOUP:
    {
        _texture = Engine::GetInstance().textures->Load("assets/Textures/Particles/soup.png");

        initWithTotalParticles(9);

        _duration = DURATION_INFINITY;

        this->_emitterMode = Mode::GRAVITY;
        this->modeA.gravity = { 0.0f, -10.0f };
        this->modeA.speed = 10.0f;
        this->modeA.speedVar = 5.0f;

        _angle = 90.0f;
        _angleVar = 360.0f;

        _life = 3.0f;
        _lifeVar = 2.0f;

        _startSize = 30.0f;
        _startSizeVar = 15.0f;

        _endSize = 20.0f;

        _emissionRate = _totalParticles / _life;

        _startColor = { 0.2f, 0.8f, 0.2f, 1.0f };
        _startColorVar = { 0.1f, 0.1f, 0.1f, 0.2f };

        _endColor = { 0.2f, 0.2f, 0.1f, 0.5f };
        _endColorVar = { 0.1f, 0.1f, 0.1f, 0.2f };

        _posVar = { 10.0f, 20.0f };

        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
        break;
	}

    case ParticleExample::MOHO:
    {
        // 1. Elegir una de las 4 texturas de moho de forma aleatoria
        int randMoho = (rand() % 4) + 1; // Genera un número del 1 al 4
        std::string path = "assets/Textures/Particles/Moho/Moho_Particle_" + std::to_string(randMoho) + ".png";

        initWithTotalParticles(35); 

        _duration = DURATION_INFINITY;
        this->_emitterMode = Mode::GRAVITY;

        // Queremos que floten muy suavemente hacia abajo (o hacia arriba si es gravedad negativa)
        this->modeA.gravity = { 0.0f, 5.0f };
        this->modeA.speed = 10.0f; // Muy lentas
        this->modeA.speedVar = 5.0f;

        _angle = 90.0f;
        _angleVar = 360.0f; // Que salgan en todas direcciones

        _life = 3.0f; // Que duren poco
        _lifeVar = 0.5f;

        _startSize = 60.0f; // Esporas pequeñitas
        _startSizeVar = 60.0f;
        _endSize = 20.0f;

        _emissionRate = _totalParticles / _life;

        // Colores: Tonos verdosos/marrones (R, G, B, Alpha)
        _startColor = { 0.2f, 0.8f, 0.2f, 1.0f };
        _startColorVar = { 0.1f, 0.1f, 0.1f, 0.0f };
        _endColor = { 0.1f, 0.5f, 0.1f, 0.0f }; // Se desvanecen

        _posVar = { 10.0f, 30.0f }; // Dispersión a lo largo del muro

        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_ADD);

        break;
    }

    case ParticleExample::MOHO_SALTO:
    {
        // 1. Elegir una de las 4 texturas de moho de forma aleatoria
        int randMoho = (rand() % 4) + 1; // Genera un número del 1 al 4
        std::string path = "assets/Textures/Particles/Moho/Moho_Particle_" + std::to_string(randMoho) + ".png";

        // Cargamos la textura aleatoria
        _texture = Engine::GetInstance().textures->Load(path.c_str());

        // Más partículas que en el moho normal para que se note el impacto
        initWithTotalParticles(30);

        _duration = DURATION_INFINITY;
        this->_emitterMode = Mode::GRAVITY;

        // Gravedad suave para que floten, pero velocidad inicial más alta
        this->modeA.gravity = { 0.0f, 5.0f };
        this->modeA.speed = 50.0f; // Salen disparadas al aterrizar
        this->modeA.speedVar = 20.0f;

        _angle = 90.0f;
        _angleVar = 360.0f; // Explosión en todas direcciones

        _life = 0.6f;
        _lifeVar = 0.2f;

        _startSize = 120.0f; // Esporas un poco más grandes al impactar
        _startSizeVar = 120.0f;
        _endSize = 30.0f;   // Crecen un poco al disiparse

        _emissionRate = _totalParticles / 0.1f; // Explosión rápida

        _startColor = { 0.2f, 0.8f, 0.2f, 1.0f };
        _startColorVar = { 0.1f, 0.1f, 0.1f, 0.0f };
        _endColor = { 0.1f, 0.5f, 0.1f, 0.0f };

        _posVar = { 150.0f, 130.0f }; // Dispersión un poco más amplia por el impacto

        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
        break;
    }

    case ParticleExample::POLVO:
    {
        // 1. Elegir una de las 3 texturas de polvo de forma aleatoria
        int randDust = (rand() % 3) + 1; // Da 1, 2 o 3
        std::string path = "assets/Textures/Particles/Polvo/polvo_trans" + std::to_string(randDust) + ".png";

        // Sobrescribimos la textura por defecto con la aleatoria
        _texture = Engine::GetInstance().textures->Load(path.c_str());

        // 2. Comportamiento del polvo
        initWithTotalParticles(1); // Pocas partículas por "puff" para no saturar

        _duration = DURATION_INFINITY;
        this->_emitterMode = Mode::GRAVITY;

        // Queremos que el polvo se eleve un poquito y se esparza a los lados
        this->modeA.gravity = { 0.0f, -5.0f }; // Sube ligeramente
        this->modeA.speed = 15.0f;
        this->modeA.speedVar = 10.0f;

        _angle = 90.0f;
        _angleVar = 360.0f; // Que salga en todas direcciones

        _life = 0.4f; // Desaparece rápido (es un rastro de pisada)
        _lifeVar = 0.1f;

        _startSize = 70.0f;
        _startSizeVar = 5.0f;
        _endSize = 115.0f; // ¡El polvo se expande a medida que desaparece!
        _endSizeVar = 5.0f;

        //_emissionRate = _totalParticles / _life;
        _emissionRate = _totalParticles / 0.1f; // ¡Expulsa todas en 0.1 segundos!

        // Mantener el color original de tu PNG, pero lo desvanecemos al final
        _startColor = { 1.0f, 1.0f, 1.0f, 0.8f };
        _startColorVar = { 0.0f, 0.0f, 0.0f, 0.1f };
        _endColor = { 1.0f, 1.0f, 1.0f, 0.0f };

        _posVar = { 8.0f, 2.0f }; // Dispersión horizontal en la base de los pies

        // No emiten luz, usan transparencia normal
        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);

        break;
    }

    case ParticleExample::POLVO_SALTO:
    {
        int randDust = (rand() % 3) + 1;
        std::string path = "assets/Textures/Particles/Polvo/polvo_trans" + std::to_string(randDust) + ".png";
        _texture = Engine::GetInstance().textures->Load(path.c_str());

        initWithTotalParticles(14); // Más partículas para el impacto
        _duration = DURATION_INFINITY;
        this->_emitterMode = Mode::GRAVITY;

        this->modeA.gravity = { 0.0f, -10.0f }; // Sube un poco más que el polvo normal
        this->modeA.speed = 30.0f; // Salen disparadas más rápido
        this->modeA.speedVar = 15.0f;

        _angle = 90.0f;
        _angleVar = 360.0f;
        _life = 0.5f;
        _lifeVar = 0.1f;

        _startSize = 100.0f; // ¡Más grandes!
        _startSizeVar = 40.0f;
        _endSize = 40.0f;   // Se expanden mucho más al desaparecer
        _endSizeVar = 10.0f;

        _emissionRate = _totalParticles / 0.1f; // Explosión instantánea

        _startColor = { 1.0f, 1.0f, 1.0f, 0.8f };
        _startColorVar = { 0.0f, 0.0f, 0.0f, 0.1f };
        _endColor = { 1.0f, 1.0f, 1.0f, 0.0f };

        _posVar = { 15.0f, 5.0f }; // Área de dispersión más ancha
        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
        break;
    }

    case ParticleExample::HIERBAJO:
    {

        _texture = Engine::GetInstance().textures->Load("assets/Textures/Particles/hierbajo.png");

        initWithTotalParticles(1); // Con 5 hojitas por pisada es suficiente

        _duration = DURATION_INFINITY;
        this->_emitterMode = Mode::GRAVITY;

        // Queremos que las hojas salten hacia arriba y luego caigan por gravedad
        this->modeA.gravity = { 0.0f, 60.0f }; // Gravedad fuerte hacia abajo
        this->modeA.speed = -80.0f; // Impulso inicial fuerte hacia arriba
        this->modeA.speedVar = 30.0f;

        _angle = 90.0f;
        _angleVar = 45.0f; // Salen como en un cono/abanico hacia arriba

        _life = 0.5f;
        _lifeVar = 0.2f;

        _startSize = 50.0f; // Tamaño de la hojita
        _startSizeVar = 4.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE; // Las hojas no se encogen ni crecen

        // Rotación para que las hojas giren mientras saltan
        _startSpin = 0.0f;
        _startSpinVar = 360.0f;
        _endSpin = 360.0f;
        _endSpinVar = 360.0f;

        //_emissionRate = _totalParticles / _life;
        _emissionRate = _totalParticles / 0.1f; // ¡Expulsa todas en 0.1 segundos!

        _startColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        _startColorVar = { 0.0f, 0.0f, 0.0f, 0.0f };
        _endColor = { 1.0f, 1.0f, 1.0f, 0.0f }; // Se desvanecen al tocar el suelo

        _posVar = { 10.0f, 0.0f };

        // Transparencia normal
        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);

        break;
    }

    case ParticleExample::HIERBAJO_SALTO:
    {
        _texture = Engine::GetInstance().textures->Load("assets/Textures/Particles/hierbajo.png");

        initWithTotalParticles(8); // Más hojas saltando

        _duration = DURATION_INFINITY;
        this->_emitterMode = Mode::GRAVITY;

        this->modeA.gravity = { 0.0f, 80.0f }; // Gravedad fuerte
        this->modeA.speed = -100.0f; // Salto vertical más brusco
        this->modeA.speedVar = 40.0f;

        _angle = 90.0f;
        _angleVar = 60.0f; // Abanico más amplio
        _life = 0.6f;
        _lifeVar = 0.2f;

        _startSize = 50.0f; // Hojas más grandes
        _startSizeVar = 20.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;

        _startSpin = 0.0f;
        _startSpinVar = 360.0f;
        _endSpin = 360.0f;
        _endSpinVar = 360.0f;

        _emissionRate = _totalParticles / 0.1f;

        _startColor = { 1.0f, 1.0f, 1.0f, 1.0f };
        _startColorVar = { 0.0f, 0.0f, 0.0f, 0.0f };
        _endColor = { 1.0f, 1.0f, 1.0f, 0.0f };
        _posVar = { 15.0f, 0.0f };
        SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);
        break;
    }

    case ParticleExample::FLOATING_DUST:
    {
        // No cargamos textura para que use automáticamente la por defecto (fuego)

        initWithTotalParticles(120);

        _duration = DURATION_INFINITY;
        this->_emitterMode = Mode::GRAVITY;

        // Gravedad flotante: Ligerísima tendencia a subir o flotar libremente
        this->modeA.gravity = { 0.0f, -1.0f };
        this->modeA.speed = 13.0f; // Velocidad extremadamente baja
        this->modeA.speedVar = 5.0f;

        _angle = 90.0f;
        _angleVar = 400.0f; // Que cada mota flote en direcciones aleatorias

        _life = 8.0f;  // Duran mucho en el aire
        _lifeVar = 3.0f;

        _startSize = 100.0f;
        _startSizeVar = 50.0f;
        _endSize = 3.0f;   // Se van haciendo aún más pequeñas
        _endSizeVar = 2.0f;

        _emissionRate = _totalParticles / _life;

        // Color Blanco y semi-transparente
        _startColor = { 1.0f, 1.0f, 1.0f, 0.3f }; // Alpha inicial al 40%
        _startColorVar = { 0.0f, 0.0f, 0.0f, 0.2f }; // Variación de transparencia
        _endColor = { 1.0f, 1.0f, 1.0f, 0.0f }; // Terminan desvaneciéndose (Alpha 0)
        _endColorVar = { 0.0f, 0.0f, 0.0f, 0.0f };

        // Amplio rango de aparición para llenar el área del emisor (ajusta según Tiled)
        _posVar = { 60.0f, 60.0f };

        // Usamos Blend normal para que respete bien las transparencias suaves
        if (_texture) SDL_SetTextureBlendMode(_texture, SDL_BLENDMODE_BLEND);

        break;
    }

    //case ParticleExample::FIRE_WORK:
    //{
    //    initWithTotalParticles(1500);

    //    // duration
    //    _duration = DURATION_INFINITY;

    //    // Gravity Mode
    //    this->_emitterMode = Mode::GRAVITY;

    //    // Gravity Mode: gravity
    //    this->modeA.gravity = { 0.0f, 90.0f };

    //    // Gravity Mode:  radial
    //    this->modeA.radialAccel = 0.0f;
    //    this->modeA.radialAccelVar = 0.0f;

    //    //  Gravity Mode: speed of particles
    //    this->modeA.speed = -180.0f;
    //    this->modeA.speedVar = 50.0f;

    //    // angle
    //    this->_angle = 90.0f;
    //    this->_angleVar = 20.0f;

    //    // life of particles
    //    this->_life = 3.5f;
    //    this->_lifeVar = 1.0f;

    //    // emits per frame
    //    this->_emissionRate = _totalParticles / _life;

    //    // color of particles
    //    _startColor.r = 0.5f;
    //    _startColor.g = 0.5f;
    //    _startColor.b = 0.5f;
    //    _startColor.a = 1.0f;
    //    _startColorVar.r = 0.5f;
    //    _startColorVar.g = 0.5f;
    //    _startColorVar.b = 0.5f;
    //    _startColorVar.a = 0.1f;
    //    _endColor.r = 0.1f;
    //    _endColor.g = 0.1f;
    //    _endColor.b = 0.1f;
    //    _endColor.a = 0.2f;
    //    _endColorVar.r = 0.1f;
    //    _endColorVar.g = 0.1f;
    //    _endColorVar.b = 0.1f;
    //    _endColorVar.a = 0.2f;

    //    // size, in pixels
    //    _startSize = 8.0f;
    //    _startSizeVar = 2.0f;
    //    _endSize = START_SIZE_EQUAL_TO_END_SIZE;

    //    _posVar = { 0, 0 };
    //    break;
    //}
    // 
    //case ParticleExample::SUN:
    //{
    //    initWithTotalParticles(350);

    //    // additive
    //    //this->setBlendAdditive(true);

    //    // duration
    //    _duration = DURATION_INFINITY;

    //    // Gravity Mode
    //    setEmitterMode(Mode::GRAVITY);

    //    // Gravity Mode: gravity
    //    setGravity(Vec2(0, 0));

    //    // Gravity mode: radial acceleration
    //    setRadialAccel(0);
    //    setRadialAccelVar(0);

    //    // Gravity mode: speed of particles
    //    setSpeed(-20);
    //    setSpeedVar(5);

    //    // angle
    //    _angle = 90;
    //    _angleVar = 360;

    //    // life of particles
    //    _life = 1;
    //    _lifeVar = 0.5f;

    //    // size, in pixels
    //    _startSize = 30.0f;
    //    _startSizeVar = 10.0f;
    //    _endSize = START_SIZE_EQUAL_TO_END_SIZE;

    //    // emits per seconds
    //    _emissionRate = _totalParticles / _life;

    //    // color of particles
    //    _startColor.r = 0.76f;
    //    _startColor.g = 0.25f;
    //    _startColor.b = 0.12f;
    //    _startColor.a = 1.0f;
    //    _startColorVar.r = 0.0f;
    //    _startColorVar.g = 0.0f;
    //    _startColorVar.b = 0.0f;
    //    _startColorVar.a = 0.0f;
    //    _endColor.r = 0.0f;
    //    _endColor.g = 0.0f;
    //    _endColor.b = 0.0f;
    //    _endColor.a = 1.0f;
    //    _endColorVar.r = 0.0f;
    //    _endColorVar.g = 0.0f;
    //    _endColorVar.b = 0.0f;
    //    _endColorVar.a = 0.0f;

    //    _posVar = { 0, 0 };
    //    break;
    //}

    //case ParticleExample::METEOR:
    //{
    //    initWithTotalParticles(150);

    //    // duration
    //    _duration = DURATION_INFINITY;

    //    // Gravity Mode
    //    setEmitterMode(Mode::GRAVITY);

    //    // Gravity Mode: gravity
    //    setGravity(Vec2(-200, -200));

    //    // Gravity Mode: speed of particles
    //    setSpeed(-15);
    //    setSpeedVar(5);

    //    // Gravity Mode: radial
    //    setRadialAccel(0);
    //    setRadialAccelVar(0);

    //    // Gravity Mode: tangential
    //    setTangentialAccel(0);
    //    setTangentialAccelVar(0);

    //    // angle
    //    _angle = 90;
    //    _angleVar = 360;

    //    // life of particles
    //    _life = 2;
    //    _lifeVar = 1;

    //    // size, in pixels
    //    _startSize = 60.0f;
    //    _startSizeVar = 10.0f;
    //    _endSize = START_SIZE_EQUAL_TO_END_SIZE;

    //    // emits per second
    //    _emissionRate = _totalParticles / _life;

    //    // color of particles
    //    _startColor.r = 0.2f;
    //    _startColor.g = 0.4f;
    //    _startColor.b = 0.7f;
    //    _startColor.a = 1.0f;
    //    _startColorVar.r = 0.0f;
    //    _startColorVar.g = 0.0f;
    //    _startColorVar.b = 0.2f;
    //    _startColorVar.a = 0.1f;
    //    _endColor.r = 0.0f;
    //    _endColor.g = 0.0f;
    //    _endColor.b = 0.0f;
    //    _endColor.a = 1.0f;
    //    _endColorVar.r = 0.0f;
    //    _endColorVar.g = 0.0f;
    //    _endColorVar.b = 0.0f;
    //    _endColorVar.a = 0.0f;

    //    _posVar = { 0, 0 };
    //    break;
    //}
    // 
    //case ParticleExample::SPIRAL:
    //{
    //    initWithTotalParticles(500);

    //    // duration
    //    _duration = DURATION_INFINITY;

    //    // Gravity Mode
    //    setEmitterMode(Mode::GRAVITY);

    //    // Gravity Mode: gravity
    //    setGravity(Vec2(0, 0));

    //    // Gravity Mode: speed of particles
    //    setSpeed(-150);
    //    setSpeedVar(0);

    //    // Gravity Mode: radial
    //    setRadialAccel(-380);
    //    setRadialAccelVar(0);

    //    // Gravity Mode: tangential
    //    setTangentialAccel(45);
    //    setTangentialAccelVar(0);

    //    // angle
    //    _angle = 90;
    //    _angleVar = 0;

    //    // life of particles
    //    _life = 12;
    //    _lifeVar = 0;

    //    // size, in pixels
    //    _startSize = 20.0f;
    //    _startSizeVar = 0.0f;
    //    _endSize = START_SIZE_EQUAL_TO_END_SIZE;

    //    // emits per second
    //    _emissionRate = _totalParticles / _life;

    //    // color of particles
    //    _startColor.r = 0.5f;
    //    _startColor.g = 0.5f;
    //    _startColor.b = 0.5f;
    //    _startColor.a = 1.0f;
    //    _startColorVar.r = 0.5f;
    //    _startColorVar.g = 0.5f;
    //    _startColorVar.b = 0.5f;
    //    _startColorVar.a = 0.0f;
    //    _endColor.r = 0.5f;
    //    _endColor.g = 0.5f;
    //    _endColor.b = 0.5f;
    //    _endColor.a = 1.0f;
    //    _endColorVar.r = 0.5f;
    //    _endColorVar.g = 0.5f;
    //    _endColorVar.b = 0.5f;
    //    _endColorVar.a = 0.0f;

    //    _posVar = { 0, 0 };
    //    break;
    //}
    // 
    //case ParticleExample::EXPLOSION:
    //{
    //    initWithTotalParticles(700);

    //    // duration
    //    _duration = 0.1f;

    //    setEmitterMode(Mode::GRAVITY);

    //    // Gravity Mode: gravity
    //    setGravity(Vec2(0, 0));

    //    // Gravity Mode: speed of particles
    //    setSpeed(-70);
    //    setSpeedVar(40);

    //    // Gravity Mode: radial
    //    setRadialAccel(0);
    //    setRadialAccelVar(0);

    //    // Gravity Mode: tangential
    //    setTangentialAccel(0);
    //    setTangentialAccelVar(0);

    //    // angle
    //    _angle = 90;
    //    _angleVar = 360;

    //    // life of particles
    //    _life = 5.0f;
    //    _lifeVar = 2;

    //    // size, in pixels
    //    _startSize = 15.0f;
    //    _startSizeVar = 10.0f;
    //    _endSize = START_SIZE_EQUAL_TO_END_SIZE;

    //    // emits per second
    //    _emissionRate = _totalParticles / _duration;

    //    // color of particles
    //    _startColor.r = 0.7f;
    //    _startColor.g = 0.1f;
    //    _startColor.b = 0.2f;
    //    _startColor.a = 1.0f;
    //    _startColorVar.r = 0.5f;
    //    _startColorVar.g = 0.5f;
    //    _startColorVar.b = 0.5f;
    //    _startColorVar.a = 0.0f;
    //    _endColor.r = 0.5f;
    //    _endColor.g = 0.5f;
    //    _endColor.b = 0.5f;
    //    _endColor.a = 0.0f;
    //    _endColorVar.r = 0.5f;
    //    _endColorVar.g = 0.5f;
    //    _endColorVar.b = 0.5f;
    //    _endColorVar.a = 0.0f;

    //    _posVar = { 0, 0 };
    //    break;
    //}
    // 
    //case ParticleExample::SMOKE:
    //{
    //    initWithTotalParticles(200);

    //    // duration
    //    _duration = DURATION_INFINITY;

    //    // Emitter mode: Gravity Mode
    //    setEmitterMode(Mode::GRAVITY);

    //    // Gravity Mode: gravity
    //    setGravity(Vec2(0, 0));

    //    // Gravity Mode: radial acceleration
    //    setRadialAccel(0);
    //    setRadialAccelVar(0);

    //    // Gravity Mode: speed of particles
    //    setSpeed(-25);
    //    setSpeedVar(10);

    //    // angle
    //    _angle = 90;
    //    _angleVar = 5;

    //    // life of particles
    //    _life = 4;
    //    _lifeVar = 1;

    //    // size, in pixels
    //    _startSize = 60.0f;
    //    _startSizeVar = 10.0f;
    //    _endSize = START_SIZE_EQUAL_TO_END_SIZE;

    //    // emits per frame
    //    _emissionRate = _totalParticles / _life;

    //    // color of particles
    //    _startColor.r = 0.8f;
    //    _startColor.g = 0.8f;
    //    _startColor.b = 0.8f;
    //    _startColor.a = 1.0f;
    //    _startColorVar.r = 0.02f;
    //    _startColorVar.g = 0.02f;
    //    _startColorVar.b = 0.02f;
    //    _startColorVar.a = 0.0f;
    //    _endColor.r = 0.0f;
    //    _endColor.g = 0.0f;
    //    _endColor.b = 0.0f;
    //    _endColor.a = 1.0f;
    //    _endColorVar.r = 0.0f;
    //    _endColorVar.g = 0.0f;
    //    _endColorVar.b = 0.0f;
    //    _endColorVar.a = 0.0f;

    //    _posVar = { 20.0f, 0.0f };
    //    break;
    //}
    // 
    //case ParticleExample::SNOW:
    //{
    //    initWithTotalParticles(700);

    //    // duration
    //    _duration = DURATION_INFINITY;

    //    // set gravity mode.
    //    setEmitterMode(Mode::GRAVITY);

    //    // Gravity Mode: gravity
    //    setGravity(Vec2(0, 1));

    //    // Gravity Mode: speed of particles
    //    setSpeed(-5);
    //    setSpeedVar(1);

    //    // Gravity Mode: radial
    //    setRadialAccel(0);
    //    setRadialAccelVar(1);

    //    // Gravity mode: tangential
    //    setTangentialAccel(0);
    //    setTangentialAccelVar(1);

    //    // angle
    //    _angle = -90;
    //    _angleVar = 5;

    //    // life of particles
    //    _life = 45;
    //    _lifeVar = 15;

    //    // size, in pixels
    //    _startSize = 10.0f;
    //    _startSizeVar = 5.0f;
    //    _endSize = START_SIZE_EQUAL_TO_END_SIZE;

    //    // emits per second
    //    _emissionRate = 10;

    //    // color of particles
    //    _startColor.r = 1.0f;
    //    _startColor.g = 1.0f;
    //    _startColor.b = 1.0f;
    //    _startColor.a = 1.0f;
    //    _startColorVar.r = 0.0f;
    //    _startColorVar.g = 0.0f;
    //    _startColorVar.b = 0.0f;
    //    _startColorVar.a = 0.0f;
    //    _endColor.r = 1.0f;
    //    _endColor.g = 1.0f;
    //    _endColor.b = 1.0f;
    //    _endColor.a = 0.0f;
    //    _endColorVar.r = 0.0f;
    //    _endColorVar.g = 0.0f;
    //    _endColorVar.b = 0.0f;
    //    _endColorVar.a = 0.0f;

    //    _posVar = { 1.0f * x_, 0.0f };
    //    break;
    //}
    // 
    //case ParticleExample::RAIN:
    //{
    //    initWithTotalParticles(1000);

    //    // duration
    //    _duration = DURATION_INFINITY;

    //    setEmitterMode(Mode::GRAVITY);

    //    // Gravity Mode: gravity
    //    setGravity(Vec2(10, 10));

    //    // Gravity Mode: radial
    //    setRadialAccel(0);
    //    setRadialAccelVar(1);

    //    // Gravity Mode: tangential
    //    setTangentialAccel(0);
    //    setTangentialAccelVar(1);

    //    // Gravity Mode: speed of particles
    //    setSpeed(-130);
    //    setSpeedVar(30);

    //    // angle
    //    _angle = -90;
    //    _angleVar = 5;

    //    // life of particles
    //    _life = 4.5f;
    //    _lifeVar = 0;

    //    // size, in pixels
    //    _startSize = 4.0f;
    //    _startSizeVar = 2.0f;
    //    _endSize = START_SIZE_EQUAL_TO_END_SIZE;

    //    // emits per second
    //    _emissionRate = 20;

    //    // color of particles
    //    _startColor.r = 0.7f;
    //    _startColor.g = 0.8f;
    //    _startColor.b = 1.0f;
    //    _startColor.a = 1.0f;
    //    _startColorVar.r = 0.0f;
    //    _startColorVar.g = 0.0f;
    //    _startColorVar.b = 0.0f;
    //    _startColorVar.a = 0.0f;
    //    _endColor.r = 0.7f;
    //    _endColor.g = 0.8f;
    //    _endColor.b = 1.0f;
    //    _endColor.a = 0.5f;
    //    _endColorVar.r = 0.0f;
    //    _endColorVar.g = 0.0f;
    //    _endColorVar.b = 0.0f;
    //    _endColorVar.a = 0.0f;

    //    _posVar = { 1.0f * x_, 0.0f };
    //    break;
    //}
    //default:
    //    break;
    //}
    }
}
