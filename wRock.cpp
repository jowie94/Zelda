#include "wRock.h"



wRock::wRock(float damage) : cWeapon(damage) {}

wRock::~wRock()
{
}

void wRock::Logic(int* map)
{
	int x, y, nx, ny, offset = STEP_LENGTH + 1;
	GetPosition(&x, &y);

	switch (GetState())
	{
	case STATE_WALKUP:
		MoveUp(offset, map);
		break;
	case STATE_WALKLEFT:
		MoveLeft(offset, map);
		break;
	case STATE_WALKDOWN:
		MoveDown(offset, map);
		break;
	case STATE_WALKRIGHT:
		MoveRight(offset, map);
		break;
	}

	GetPosition(&nx, &ny);

	if (x == nx && y == ny)
	{
		SetLife(0);
		ResetFrame();
		SetFramesToDie(0);
	}
}

void wRock::Draw()
{
	if (!isDead())
	{
		DrawRect(GetTexture(), 0, 1, 0.25f, 0.75f);
		NextFrame(2);
	}
}

void wRock::Attack(bool special, int orientation)
{
	SetLife(1);
	SetState(orientation % 4 + 4);
}
