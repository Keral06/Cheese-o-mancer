#ifndef __HIGH_PRIESTESS_H__
#define __HIGH_PRIESTESS_H__

#include "Enemy.h"

class HighPriestesss : public Enemy {
public:
    HighPriestesss();
    virtual ~HighPriestesss();

    bool Start() override;
    bool Update(float dt) override;
    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void ChangeCurrentAnimation() override;

    void SpawnWave();
    void NotifyEnemyDeath();

private:
    int currentWave;
    int enemiesAlive;
    int hitsTaken;
    bool isVulnerable;
    // Podrías añadir un timer para el estado TURN si quieres que se vuelva a proteger sola
};

#endif