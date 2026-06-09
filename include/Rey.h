#ifndef __REY_H__
#define __REY_H__

#include "Enemy.h"

class Rey : public Enemy {
public:
    enum ReyState
    {
        IDLER,
        DEADR
    };

    enum Finales 
    {
        FINAL1,
        FINAL2,
        FINAL3,
        FINAL4
    
    };
    Rey();
    virtual ~Rey();
    bool Start() override;
    void Die(); // Lógica de muerte que spawnea al Mago
    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;
    bool Update(float dt) override;
    void Draw(float dt) override;
    void ChangeCurrentAnimation() override;

    int texW = 0;
    int texH = 0;

    float offsetX;
    float offsetY;

    AnimationSet anims;
    SDL_Texture* texture = NULL;
    SDL_Texture* textureIdle = NULL;
    SDL_Texture* textureBall = NULL;
    SDL_Texture* textureTransformation = NULL;

    ReyState stateR;
    ReyState lastStateR;

    bool playerInRange = false;
    bool magoSpawned = false;
};
#endif