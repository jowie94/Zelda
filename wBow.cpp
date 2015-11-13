#include "wBow.h"

wBow::wBow(float damage) : cWeapon(damage)
{
	init = false;
	fmod_system->createSound("sounds/arrpw.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &arrow_sound);
}

wBow::~wBow()
{
}

void wBow::Attack(bool special, int orientation)
{
	if (isDead())
	{
		PlaySound(arrow_sound);
		int x, y;
		GetPosition(&x, &y);
		switch (orientation)
		{
		case STATE_LOOKLEFT:
			SetWidthHeight(16, 8);
			x -= 15; y += 2;
			break;
		case STATE_LOOKRIGHT:
			SetWidthHeight(16, 8);
			x += 15; y += 2;
			break;
		case STATE_LOOKUP:
			SetWidthHeight(8, 16);
			x += 4; y += 15;
			break;
		case STATE_LOOKDOWN:
			SetWidthHeight(8, 16);
			x += 5; y -= 15;
			break;
		}
		SetPosition(x, y);
		SetState(orientation);
		SetLife(1);
		init = true;
		SetFramesToDie(1);
	}
}

void wBow::Logic(int* map)
{
	if (!cWeapon::isDead())
	{
		int x, y, offset = 2;
		GetPosition(&x, &y);

		switch (GetState())
		{
		case STATE_LOOKLEFT:
			x -= offset;
			break;
		case STATE_LOOKRIGHT:
			x += offset;
			break;
		case STATE_LOOKUP:
			y += offset;
			break;
		case STATE_LOOKDOWN:
			y -= offset;
			break;

		}
		SetPosition(x, y);

		if (init && GetFrame() >= GetFramesToDie())
			init = false;

		if (CollidesWithBorder())
			cWeapon::Finalize();
	}
	cBicho::Logic(map);
}

void wBow::Draw()
{
	if (!isDead()) {
		float xo, yo, xf, yf;

		switch (GetState())
		{
		case STATE_LOOKLEFT:
			xo = 0.625; yo = 0.125f; xf = xo + 0.25f; yf = 0.0f;
			break;
		case STATE_LOOKRIGHT:
			xo = 0.625f; yo = 0.25f; xf = xo + 0.25f; yf = 0.125f;
			break;
		case STATE_LOOKUP:
			xo = 0.875f; yo = 0.25f; xf = xo + 0.125f; yf = 0.0f;
			break;
		case STATE_LOOKDOWN:
			xo = 0.5f; yf = 0.0f; yo = 0.25f; xf = xo + 0.125f;
			break;
		}

		NextFrame(GetFramesToDie() + 1);
		DrawRect(GetTexture(), xo, yo, xf, yf);
	}
}

bool wBow::LockPlayer()
{
	return init;
}

void wBow::Finalize()
{
}
