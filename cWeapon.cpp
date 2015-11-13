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

void cWeapon::Collides(const cRect& position, const int status, cRect& collision, float& damage)
{
	if (cBicho::Collides(&position))
	{
		GetArea(&collision);
		damage = GetDamage();
	}
}

bool cWeapon::CollidesWithBorder()
{
	int x, y;
	GetPosition(&x, &y);

	return x <= 0 || x >= 15 * 16 || y <= 0 || y >= 10 * 16; // TODO: Include Scene.h and use constants
}

void cWeapon::Hurt(int* map) {}

void cWeapon::Finalize()
{
	SetLife(0);
	ResetFrame();
	SetFramesToDie(0);
}

int cWeapon::GetId()
{
	return id;
}
