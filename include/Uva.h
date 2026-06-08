#ifndef __UVA_H__
#define __UVA_H__

#include "Entity.h"

class Uva : public Entity {
public:
    Uva();
    virtual ~Uva();
    bool Start() override;
    bool Update(float dt) override;
};
#endif