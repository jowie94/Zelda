#include "cWeapon.h"

cWeapon::cWeapon(float damage)
{
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
