#include "wSword.h"

wSword::wSword(float damage) : cWeapon(damage) {}

wSword::~wSword() {}

void wSword::Attack(bool special, int orientation)
{
	this->special = special;

	if (isDead()) 
	{
		int x, y;
		GetPosition(&x, &y);

		switch (orientation)
		{
		case STATE_LOOKLEFT:
			SetWidthHeight(16, 8);
			x -= 16; y += 4;
			break;
		case STATE_LOOKRIGHT:
			SetWidthHeight(16, 8);
			x += 16; y += 4;
			break;
		case STATE_LOOKUP:
			SetWidthHeight(8, 16);
			x += 4;
			y += 16;
			break;
		case STATE_LOOKDOWN:
			SetWidthHeight(8, 16);
			x += 4;
			y -= 16;
			break;
		}

		SetPosition(x, y);
		SetState(orientation);
		SetLife(1);
		SetFramesToDie(3);
	}
}

void wSword::Draw()
{
	if (!isDead()) {
		float xo, yo, xf, yf;

		int x, y, frame;
		GetPosition(&x, &y);
		frame = GetFrame();

		switch(GetState())
		{
		case STATE_LOOKLEFT:
			xo = 0.125f; yo = 0.125f; xf = 0.375f; yf = 0.0f;
			x += frame;
			break;
		case STATE_LOOKRIGHT:
			xo = 0.125f; yo = 0.25f; xf = 0.375f; yf = 0.125f;
			x -= frame;
			break;
		case STATE_LOOKUP:
			xo = 0.375f; yo = 0.25f; xf = 0.5f; yf = 0.0f;
			y -= frame;
			break;
		case STATE_LOOKDOWN:
			xo = 0.0f; yo = 0.25f; xf = 0.125f; yf = 0.0f;
			y += frame;
			break;
		}
		SetPosition(x, y);
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
