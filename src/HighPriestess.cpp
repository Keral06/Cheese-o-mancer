#include "HighPriestess.h"
#include "Engine.h"
#include "EntityManager.h"
#include "Physics.h"

HighPriestesss::HighPriestesss() : Enemy() {
    name = "HighPriestess";
    currentWave = 1;
    enemiesAlive = 0;
    hitsTaken = 0;
    isVulnerable = false;
}

HighPriestesss::~HighPriestesss() {}

bool HighPriestesss::Start() {
    // Llamar al Start base para inicializar física y texturas
    Enemy::Start();

    // Configuración específica
    health = 3;
    SetState(EnemyState::IDLE);

    SpawnWave();
    return true;
}

bool HighPriestesss::Update(float dt) {
    // Si no hay enemigos y no hemos terminado, el boss se expone
    if (enemiesAlive <= 0 && currentWave <= 3 && !isVulnerable && hitsTaken < 3) {
        isVulnerable = true;
        SetState(EnemyState::WALKING); // Usamos WALKING como alias de "TURN" (vulnerable)
        //LOG("HighPriestess expuesta!");
    }

    // El Update base se encarga de llamar a ApplyPhysics y Draw
    return Enemy::Update(dt);
}

void HighPriestesss::OnCollision(PhysBody* physA, PhysBody* physB) {
    // Si el jugador ataca y el boss es vulnerable
    if (isVulnerable && physB->ctype == ColliderType::PLAYERATTACK) {
        hitsTaken++;
        isVulnerable = false;

        if (hitsTaken >= 3) {
            SetState(EnemyState::DYING);
            Die();
        }
        else {
            currentWave++;
            SetState(EnemyState::IDLE);
            SpawnWave();
        }
    }
}

void HighPriestesss::SpawnWave() {
    // Lógica para crear enemigos (ej: 2 enemigos por oleada)
    int count = 2;
    enemiesAlive = count;
    for (int i = 0; i < count; ++i) {
        Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY);
        // Nota: Asegúrate de que los enemigos nuevos tengan una referencia a 'this'
        // para llamar a NotifyEnemyDeath() cuando mueran.
    }
    //LOG("Oleada %d iniciada", currentWave);
}

void HighPriestesss::NotifyEnemyDeath() {
    enemiesAlive--;
}

void HighPriestesss::ChangeCurrentAnimation() {
    // Sobrescribimos para definir qué animaciones usa el boss
    // IDLE = Protegida, WALKING = Vulnerable (TURN)
    switch (state) {
    case EnemyState::IDLE: anims.SetCurrent("idle"); break;
    case EnemyState::WALKING: anims.SetCurrent("turn"); break; // Usamos el alias de turn
    case EnemyState::DYING: anims.SetCurrent("death"); break;
    }
}