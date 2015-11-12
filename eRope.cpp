#include "eRope.h"

eRope::eRope(int texture, float life, float damage) : cEnemy(texture, life, damage)
{
	steps = action = 0;
	orientation = STATE_LOOKLEFT;
	SetWidthHeight(16, 16);
}

eRope::~eRope()
{
}

void eRope::Hurt(int* map)
{
	int xoff = 0, yoff = 0;
	switch (hurt_direction)
	{
	case STATE_LOOKUP:
		yoff -= 2;
		break;
	case STATE_LOOKDOWN:
		yoff += 2;
		break;
	case STATE_LOOKLEFT:
		xoff += 2;
		break;
	case STATE_LOOKRIGHT:
		xoff -= 2;
		break;
	}
	Move(xoff, yoff, map);
	--hurt;
	ToggleHurt(hurt != 0);
}

void eRope::Logic(int* map, cPlayer& player)
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
		{
			DecrementLife(damage);
			ToggleHurt(true);
			hurt = 4 * 8;
			Stop();
			CalculateCollisionMovement(collision, hurt_direction);
		} 
		else if (!IsHurt())
		{
			cRect p;
			player.GetArea(&p);
			int state = PlayerInDirection(p);

			int x, y;
			GetPosition(&x, &y);
			int offset = STEP_LENGTH - 1;

			if (state != -1 && !player.IsHurt())
			{
				if ((state > 1 && x % 16 == 0) || (state < 2 && y % 16 == 0))
				{
					SetState(state);
					action = state;
					steps = 256;
					offset = STEP_LENGTH + 1;
				}
			}
			
			if (!steps)
			{
				action = rand() % 4;
				switch (action)
				{
				case STATE_LOOKLEFT:
					MoveLeft(offset, map);
					break;
				case STATE_LOOKRIGHT:
					MoveRight(offset, map);
					break;
				case STATE_LOOKUP:
					MoveUp(offset, map);
					break;
				case STATE_LOOKDOWN:
					MoveDown(offset, map);
					break;
				}
				steps = (rand() % 256);
				steps -= steps % 16;
			}
			else
			{
				int nx, ny;
				switch (action)
				{
				case STATE_LOOKLEFT:
					MoveLeft(offset, map);
					break;
				case STATE_LOOKRIGHT:
					MoveRight(offset, map);
					break;
				case STATE_LOOKUP:
					MoveUp(offset, map);
					break;
				case STATE_LOOKDOWN:
					MoveDown(offset, map);
					break;
				}
				--steps;

				GetPosition(&nx, &ny);
				if (x == nx && y == ny)
				{
					steps = 0;
					action = -1;
				}
			}

			if (GetState() % 4 < 2)
				orientation = GetState();
		}

		if (GetLife() <= 0 && GetState() != STATE_DYING)
		{
			SetState(STATE_DYING);
			SetFramesToDie(2);
			ResetFrame();
		}
	}
}

void eRope::Draw()
{
	if (!isDead())
	{
		float xo, yo, xf, yf;

		switch (GetState())
		{
		case STATE_DYING:
			yo = 0.75f;
			xo = 0.25f*GetFrame();
			NextFrame(3);
			break;
		default:
			xo = 0.25f*orientation;
			yo = 0.25f + 0.25f * GetFrame();
			if (IsHurt())
				xo += 0.55f * (hurt % 2);
			NextFrame(2);
			break;
		}

		xf = xo + 0.25f;
		yf = yo - 0.25f;

		DrawRect(GetTexture(), xo, yo, xf, yf);
	}
}

int eRope::PlayerInDirection(cRect& rect)
{
	int state = -1;
	cRect area;
	GetArea(&area);

	bool up =   (area.top >= rect.bottom && area.top <= rect.top),
		down =  (area.bottom >= rect.bottom && area.bottom <=rect.top),
		left =  (area.left >= rect.left && area.left <= rect.right),
		right = (area.right >= rect.left && area.right <= rect.right);


	if ((down && area.bottom - rect.bottom <= 8) || (up && rect.top - area.top <= 8))
	{
		if (area.left >= rect.left && area.left >= rect.right)
			state = STATE_LOOKLEFT;
		else if (area.right <= rect.left && area.right <= rect.right)
			state = STATE_LOOKRIGHT;
	}
	else if ((left && area.left - rect.left <= 8) || (right && rect.right - area.right <= 8))
	{
		if (area.top <= rect.bottom && area.top <= rect.top)
			state = STATE_LOOKUP;
		else if (area.bottom >= rect.bottom && area.bottom >= rect.top)
			state = STATE_LOOKDOWN;
	}

	return state;
}
