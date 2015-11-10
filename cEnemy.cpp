#include "cEnemy.h"

cEnemy::cEnemy(int texture, float life, float damage) : cBicho(life)
{
	SetDamage(damage);
	this->texture = texture;
}

cEnemy::~cEnemy()
{
}

int cEnemy::GetTexture() const
{
	return texture;
}

void cEnemy::Collides(const cRect& position, const int orientation, cRect& collision, float& damage)
{
	if(GetState() != STATE_DYING && cBicho::Collides(&position))
	{
		GetArea(&collision);
		damage = GetDamage();
	}
}

void cEnemy::SetAttacking(bool attacking)
{
	this->attacking = attacking;
}

bool cEnemy::GetAttacking() const
{
	return attacking;
}

void cEnemy::Logic(int* map, cPlayer& player)
{
	cBicho::Logic(map);
}

