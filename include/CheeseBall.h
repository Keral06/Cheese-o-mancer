#pragma once

#include "Entity.h"
#include "Vector2D.h"
#include "box2d/box2d.h"
#include "Animation.h"

class CheeseBall : public Entity
{
public:
    CheeseBall();
    ~CheeseBall();

    enum class BallState { IDLE, BREAKING, EXPLODING };
    BallState currentState = BallState::IDLE;

    float lifeTimer = 0.0f;
    float maxLifeTime = 1000.0f;

    bool Awake() override;
    bool Start() override;
    bool Update(float dt) override;
    bool CleanUp() override;

    void OnCollision(PhysBody* physA, PhysBody* physB) override;
    void OnCollisionEnd(PhysBody* physA, PhysBody* physB) override;
    void StartLifespan();
    void SetPosition(const Vector2D& pos);

    void SetVelocityy(b2Vec2 vel);
    PhysBody* pbody = nullptr;
    float radius = 128.0f;
    bool ismounted = false;
    bool launch = false;
    bool canSmash = false;
    bool firstjump = true;
    bool stop = false;
    bool isOnMoho = false;
   
    AnimationSet animBreak;
    AnimationSet animExplode;
    SDL_Texture* texBreak = nullptr;
    SDL_Texture* texExplode = nullptr;
    SDL_Texture* texIdle = nullptr;

    
    AnimationSet* currentAnimation = nullptr;
    SDL_Texture* currentTexture = nullptr;

private:
    

    Vector2D position;

    

    SDL_Texture* texture = NULL;

};