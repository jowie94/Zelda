#include "eAquamentus.h"
#include "cWeapon.h"

eAquamentus::eAquamentus(int texture, float life, float damage) : cEnemy(texture, life, damage)
{
	steps = action = 0;
	SetWidthHeight(24, 32);
	steps = 16;
	SetState(STATE_WALKRIGHT);
}

eAquamentus::~eAquamentus()
{
}

void eAquamentus::Hurt(int* map)
{
}

void eAquamentus::Collides(const cRect& position, const int status, cRect& collision, float& damage)
{
	bool collides = false;
	std::set<cWeapon*> active_wp;
	GetActiveWeapons(active_wp);

	for (cWeapon* wp : active_wp)
	{
		wp->Collides(position, status, collision, damage);
		collides = damage > 0;
		if (collides)
		{
			wp->GetArea(&collision);
			damage = wp->GetDamage();
			wp->SetLife(0);
			wp->SetFramesToDie(0);
			break;
		}
	}

	if (!collides)
	{
		cEnemy::Collides(position, status, collision, damage);
	}
}

void eAquamentus::Logic(int* map, cPlayer& player)
{
	cEnemy::Logic(map, player);

	if (!isDead() && GetState() != STATE_DYING)
	{
		cRect collision;
		float damage = 0;
		cRect rect;
		GetArea(&rect);
		player.Collides(rect, 0, collision, damage);

		if (damage && CollidesWithHead(collision)) // Aquamentus only gets damaged if it is hit in the head
			DecrementLife(damage);
		
		if (!steps)
		{
			switch (GetState())
			{
			case STATE_WALKLEFT:
				SetState(STATE_WALKRIGHT);
				break;
			case STATE_WALKRIGHT:
				SetState(STATE_WALKLEFT);
				break;
			default:
				SetState(STATE_WALKRIGHT);
				break;
			}

			steps = 3 * 16 * 5;
		}
		else
		{
			--steps;
			if (steps % 5 == 0)
			{
				switch (GetState())
				{
				case STATE_WALKLEFT:
					MoveLeft(1, map);
					break;
				case STATE_WALKRIGHT:
					MoveRight(1, map);
					break;
				}
			}
		}
	}
}

void eAquamentus::Draw()
{
	float xo, yo, xf, yf;

	xo = 0.376f * (GetFrame() > 1);
	yo = 0.5f /*+ (0.5f * float(rand() % 10 == 0))*/; // TODO: Open mouth

	xf = xo + 0.375f;
	yf = yo - 0.5f;

	NextFrame(4);

	DrawRect(GetTexture(), xo, yo, xf, yf);
}

bool eAquamentus::CollidesWithHead(cRect& rect)
{
	cRect area;
	GetArea(&area);
	return rect.bottom >= area.top - 12 || rect.top >= area.top - 6;
}