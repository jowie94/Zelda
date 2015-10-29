#include "wSword.h"

wSword::wSword(float damage) : cWeapon(damage) {}

wSword::~wSword() {}

void wSword::Attack(bool special, int orientation)
{
	this->special = special;
	if (isDead()) 
	{
		switch (orientation)
		{
		case STATE_LOOKLEFT: case STATE_LOOKRIGHT:
			SetWidthHeight(16, 8);
			break;
		case STATE_LOOKUP: case STATE_LOOKDOWN:
			SetWidthHeight(8, 16);
			break;
		}
		SetState(orientation);
		SetLife(1);
		SetFramesToDie(3);
	}
}

void wSword::Draw()
{
	if (!isDead()) {
		float xo, yo, xf, yf;
		switch(GetState())
		{
		case STATE_LOOKLEFT:
			xo = 0.125f; yo = 0.125f; xf = 0.375f; yf = 0.0f;
			break;
		case STATE_LOOKRIGHT:
			xo = 0.125f; yo = 0.25f; xf = 0.375f; yf = 0.125f;
			break;
		case STATE_LOOKUP:
			xo = 0.375f; yo = 0.25f; xf = 0.5f; yf = 0.0f;
			break;
		case STATE_LOOKDOWN:
			xo = 0.0f; yo = 0.25f; xf = 0.125f; yf = 0.0f;
			break;
		}
		DrawRect(GetTexture(), xo, yo, xf, yf);
		NextFrame(GetFramesToDie() + 1);
	}
}

void wSword::Logic(int *map)
{
	if (!isDead())
	{
		if ((GetFramesToDie() - 1) == GetFrame())
			SetLife(0);
	}
	cBicho::Logic(map);
}
