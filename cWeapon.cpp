#include "cWeapon.h"

cWeapon::cWeapon(float damage)
{
	SetWidthHeight(0, 0);
	SetLife(0);
	SetFramesToDie(0);
	SetDamage(damage);
}

cWeapon::~cWeapon() {}

void cWeapon::Attack(bool special, int orientation) {}

void cWeapon::SetTexture(int id)
{
	texture_id = id;
}

int cWeapon::GetTexture()
{
	return texture_id;
}

void cWeapon::Draw() {}

bool cWeapon::LockPlayer()
{
	return false;
}

void cWeapon::Collides(cRect& position, const int status, cRect& collision, float& damage)
{
	if (cBicho::Collides(&position))
	{
		GetArea(&collision);
		damage = GetDamage();
	}
}
