#ifndef __MAGOBOSS_H__
#define __MAGOBOSS_H__

#include "Enemy.h"

enum class MagoState { INTRO, TRANSFORMING, IDLE, ATTACK_BALL, ATTACK_RAIN };

class MagoBoss : public Enemy {
public:
    MagoBoss();
    virtual ~MagoBoss();
    bool Start() override;
    bool Update(float dt) override;

private:
    MagoState state;
    float floatTimer;
    void HandleFloating(float dt);
};
#endif