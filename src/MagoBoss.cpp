#include "MagoBoss.h"
#include "Engine.h"

MagoBoss::MagoBoss() : Enemy(), state(MagoState::INTRO), floatTimer(0.0f) {}
MagoBoss::~MagoBoss() {}

bool MagoBoss::Start() {
    
    return true;
}

bool MagoBoss::Update(float dt) {
    switch (state) {
    case MagoState::INTRO:
        // Esperar golpe del jugador para cambiar a TRANSFORMING
        break;
    case MagoState::IDLE:
        HandleFloating(dt);
        break;
        // ... resto de estados
    }
    return true;
}

void MagoBoss::HandleFloating(float dt) {
    
}