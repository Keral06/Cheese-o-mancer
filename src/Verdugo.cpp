#include "Verdugo.h"
#include "Engine.h"
#include "Log.h"

Verdugo::Verdugo() : Enemy()
{
    name = "Verdugo";
}

Verdugo::~Verdugo()
{
}

bool Verdugo::Start()
{
    texW = 400;
    texH = 400;
	attackRange = 5;
    texName = "Assets/Textures/placeholder_Jester.png";
    Enemy::Start();

    
    LOG("Verdugo creado");

    return true;
}

void Verdugo::Attack()
{
	
	attackTimer = attackCooldown;
}

bool Verdugo::Update(float dt)
{
	repathTimer++;
	attackTimer--;

	GetPhysicsValues();

	distanceToPlayer = CalculateDistance();

	if (distanceToPlayer < detectionRange) {
		if (distanceToPlayer < attackRange && attackTimer <= 0.0f) {
			Attack();
		}
		else {
			PerformPathfinding();
			Move();
		}

	}
	else {

		Vector2D enemyPos = GetPosition();
		Vector2D enemyTilePos = Engine::GetInstance().map->WorldToMap(enemyPos.getX(), enemyPos.getY());
		ResetPathfinding(enemyTilePos);
	}

	ApplyPhysics();
	Draw(dt);

	return true;
}

