#include "eStalfos.h"

eStalfos::eStalfos(int texture, float life, float damage) : cEnemy(texture, life, damage)
{
	steps = action = 0;
}

eStalfos::~eStalfos()
{
}

void eStalfos::Logic(int* map, cPlayer& player)
{
	cEnemy::Logic(map, player);

	if (!isDead() && GetState() != STATE_DYING)
	{
		cRect collision;
		float damage = 0;
		cRect rect;
		GetArea(&rect);
		player.Collides(rect, 0, collision, damage);

		if (damage)
			DecrementLife(damage);
		else
		{
			int x, y;
			GetPosition(&x, &y);
			int offset = STEP_LENGTH - 1;

			if (!steps)
			{
				action = rand() % 4;
				switch (action)
				{
				case 0:
					MoveUp(offset, map);
					break;
				case 1:
					MoveLeft(offset, map);
					break;
				case 2:
					MoveDown(offset, map);
					break;
				case 3:
					MoveRight(offset, map);
					break;
				}
				steps = (rand() % 16) * 16;
			}
			else
			{
				int nx, ny;
				switch (action)
				{
				case 0:
					MoveUp(offset, map);
					break;
				case 1:
					MoveLeft(offset, map);
					break;
				case 2:
					MoveDown(offset, map);
					break;
				case 3:
					MoveRight(offset, map);
					break;
				}
				--steps;

				GetPosition(&nx, &ny);
				if (x == nx && y == ny)
					steps = 0;
			}
		}
		
		if (GetLife() <= 0 && GetState() != STATE_DYING)
		{
			SetState(STATE_DYING);
			SetFramesToDie(2);
			ResetFrame();
		}
	}
}

void eStalfos::Draw()
{
	if (!isDead())
	{
		float xo, yo, xf, yf;
		xo = 0.25*(GetFrame() < 2);

		switch (GetState())
		{
		case STATE_DYING:
			yo = 0.5f;
			NextFrame(3);
			break;
		default:
			yo = 0.25f;
			NextFrame(4);
			break;
		}

		xf = xo + 0.25f;
		yf = yo - 0.25f;

		DrawRect(GetTexture(), xo, yo, xf, yf);
	}
}

void eStalfos::Hurt(int* map)
{
}
