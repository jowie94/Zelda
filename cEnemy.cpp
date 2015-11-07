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

void cEnemy::Collides(cRect& position, const int orientation, cRect& collision, float& damage)
{
	if(cBicho::Collides(&position))
	{
		GetArea(&collision);
		damage = GetDamage();
	}
}

void cEnemy::Logic(int* map, cPlayer& player)
{
	cBicho::Logic(map);
}
