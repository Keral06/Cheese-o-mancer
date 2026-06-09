#include "Uva.h"
#include "Engine.h"
#include "Textures.h"
#include "Physics.h"
#include "Render.h"

Uva::Uva() : Entity()
{
    name = "Uva";
}

Uva::~Uva() {}

void Uva::SetConfiguration(UvaSize size, UvaType type, float directionX)
{
    sizeConfig = size;
    typeConfig = type;
    dirX = directionX;
}

bool Uva::Start()
{
    // 1. Asignar dimensiones según el tamaño (Recuerda pre-cargarlas en el Rey/Scene para evitar lag)
    switch (sizeConfig)
    {
    case UvaSize::PEQUEÑA:
        texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/big_grape_meteorite.png");
        radius = 16;
        texW = 32;
        texH = 32;
        speed = 500.0f;
        break;

    case UvaSize::MEDIANA:
        texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/mid_grape_meteorite.png");
        radius = 24;
        texW = 48;
        texH = 48;
        speed = 400.0f;
        break;

    case UvaSize::GRANDE:
        texture = Engine::GetInstance().textures->Load("assets/Textures/Spritesheets/Wizard Cheese/sma_grape_meteorite.png");
        radius = 40;
        texW = 80;
        texH = 80;
        speed = 250.0f;
        break;
    }

    // 2. Crear cuerpo DYNAMIC (Tu wrapper lo pide: CreateCircle(x, y, radius, bodyType))
    // Usamos el tipo DYNAMIC para que responda a fuerzas y rote de manera natural
    pbody = Engine::GetInstance().physics->CreateCircle(position.getX(), position.getY(), radius, bodyType::DYNAMIC);
    pbody->listener = this;

    // Asignamos uno de tus tipos existentes en el enum
    pbody->ctype = ColliderType::ENEMY_ATTACK;

    // 3. Aplicar velocidad inicial usando tus métodos thin wrappers de Physics
    if (typeConfig == UvaType::METEORO)
    {
        // Movimiento puramente vertical hacia abajo
        Engine::GetInstance().physics->SetLinearVelocity(pbody, 0.0f, speed);
    }
    else if (typeConfig == UvaType::LINEAL)
    {
        // Movimiento horizontal con un leve salto hacia arriba en el eje Y
        Engine::GetInstance().physics->SetLinearVelocity(pbody, dirX * speed, -150.0f);
    }

    return true;
}

bool Uva::Update(float dt)
{
    // Asegurar que mantengan velocidades constantes frente al rozamiento
    if (typeConfig == UvaType::METEORO)
    {
        Engine::GetInstance().physics->SetLinearVelocity(pbody, 0.0f, speed);
    }
    else
    {
        // Mantiene la velocidad X constante pero respetando la caída de la gravedad en Y
        float currentVy = Engine::GetInstance().physics->GetYVelocity(pbody);
        Engine::GetInstance().physics->SetLinearVelocity(pbody, dirX * speed, currentVy);
    }

    // Dibujar la uva
    Draw(dt);

    return true;
}

void Uva::Draw(float dt)
{
    if (texture == nullptr || pbody == nullptr) return;

    // 1. Obtener la posición síncrona con el PhysBody de tu motor
    int x, y;
    pbody->GetPosition(x, y);
    position.setX((float)x);
    position.setY((float)y);

    // 2. ¡ROTACIÓN FÍSICA PERFECTA!
    // Tu pbody devuelve radianes en GetRotation(). 
    // Usamos tu constante RADTODEG definida en tu Physics.h para pasarlo a grados dobles para SDL
    float radians = pbody->GetRotation();
    double degrees = (double)(radians * RADTODEG);

    // 3. Posición en la pantalla (Centrado de cámara en el origen del círculo)
    int drawX = x - texW / 2;
    int drawY = y - texH / 2;

    // 4. Renderizado con rotación integrada
    Engine::GetInstance().render->DrawTexture(
        texture,
        drawX,
        drawY,
        nullptr,
        1.0f,
        degrees, // <-- Le inyectamos los grados calculados
        INT_MAX,
        INT_MAX,
        SDL_FLIP_NONE
    );
}

void Uva::OnCollision(PhysBody* physA, PhysBody* physB)
{
    // Si golpea al jugador
    if (physB->ctype == ColliderType::PLAYER)
    {
        // Código para restar vida al player...

        // Marcamos para destruir (Tu clase Physics tiene métodos para colas de borrado si hiciese falta)
       toDelete = true;
    }

    // Si los meteoros impactan con cualquier superficie sólida del entorno
    if (typeConfig == UvaType::METEORO && (physB->ctype == ColliderType::PLATFORM || physB->ctype == ColliderType::PARED))
    {
        toDelete = true;
    }
}