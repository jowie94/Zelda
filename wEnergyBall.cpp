#include "wEnergyBall.h"

wEnergyBall::wEnergyBall(float damage, int direction[2]) : cWeapon(damage)
{
	this->direction[0] = direction[0];
	this->direction[1] = direction[1];
}

wEnergyBall::~wEnergyBall()
{
}

bool wEnergyBall::CollidesMapWall(int* map, bool right)
{
	int x, y;
	GetPosition(&x, &y);

	return x <= 0 || x >= 15 * 16;
}

bool wEnergyBall::CollidesMapFloor(int* map, bool up)
{
	int x, y;
	GetPosition(&x, &y);

	return y <= 0 || y >= 10 * 16;
}

void wEnergyBall::Logic(int* map)
{
	int x, y, nx, ny, offset = STEP_LENGTH + 1;
	GetPosition(&x, &y);

	Move(direction[0], direction[1], map);

	GetPosition(&nx, &ny);

	if (direction[0] && x == nx || direction[1] && y == ny)
	{
		SetLife(0);
		ResetFrame();
		SetFramesToDie(0);
	}
}

void wEnergyBall::Attack(bool special, int orientation)
{
	SetLife(1);
}

void wEnergyBall::Draw()
{
	float xo, yo, xf, yf;

	xo = 0.75f + 0.125f * (GetFrame() % 2);
	yo = 0.15625f + 0.15625f * (GetFrame() > 2);

	xf = xo + 0.125f;
	yf = yo - 0.15625f;

	NextFrame(4);

	DrawRect(GetTexture(), xo, yo, xf, yf);
}

bool wEnergyBall::LockPlayer()
{
	return false;
}
