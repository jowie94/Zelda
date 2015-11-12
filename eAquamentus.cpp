#include "eAquamentus.h"
#include "cWeapon.h"
#include "wEnergyBall.h"

eAquamentus::eAquamentus(int texture, float life, float damage) : cEnemy(texture, life, damage)
{
	hurt = action = 0;
	SetWidthHeight(24, 32);
	steps = 16;
	SetState(STATE_WALKRIGHT);
}

eAquamentus::~eAquamentus()
{
}

void eAquamentus::Hurt(int* map)
{
	--hurt;
	ToggleHurt(hurt != 0);
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
		{
			DecrementLife(damage);
			ToggleHurt(true);
			hurt = 4 * 8;
		}

		std::set<cWeapon*> weapons;
		GetActiveWeapons(weapons);

		if (weapons.empty())
		{
			open_mouth = false;
			if (rect.left % 16 != 0)
				open_mouth = true;
			else
			{
				int dir[2] = {-1, 1};
				cWeapon* wp = new wEnergyBall(1.0f, dir);
				wp->SetTexture(GetTexture());
				wp->SetWidthHeight(8, 8);
				wp->SetPosition(rect.left, rect.bottom);
				wp->Attack(false, 0);
				ActivateWeapon(wp);
				dir[1] = 0;
				wp = new wEnergyBall(1.0f, dir);
				wp->SetTexture(GetTexture());
				wp->SetWidthHeight(8, 8);
				wp->SetPosition(rect.left, rect.bottom);
				wp->Attack(false, 0);
				ActivateWeapon(wp);
				dir[1] = -1;
				wp = new wEnergyBall(1.0f, dir);
				wp->SetTexture(GetTexture());
				wp->SetWidthHeight(8, 8);
				wp->SetPosition(rect.left, rect.bottom);
				wp->Attack(false, 0);
				ActivateWeapon(wp);
			}
		}
		
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

		if (GetLife() <= 0 && GetState() != STATE_DYING)
		{
			SetState(STATE_DYING);
			SetFramesToDie(2);
			ResetFrame();
		}
	}
}

void eAquamentus::Draw()
{
	float xo, yo, xf, yf;
	std::set<cWeapon*> weapons;
	GetActiveWeapons(weapons);

	switch(GetState())
	{
	case STATE_DYING:
		xo = 0.375f;
		yo = 0.5f;
		xf = xo + 0.25f;
		NextFrame(3);
		break;
	default:
		for (auto w : weapons)
		{
			w->Draw();
		}

		xo = 0.1875f * (GetFrame() > 1);
		yo = 0.25f + (0.25f * open_mouth); // TODO: Open mouth

		if (hurt)
			yo += 0.5f * (hurt % 2);

		xf = xo + 0.1875f;

		NextFrame(4);
		break;
	}

	yf = yo - 0.25f;

	DrawRect(GetTexture(), xo, yo, xf, yf);
}

bool eAquamentus::CollidesWithHead(cRect& rect)
{
	cRect area;
	GetArea(&area);
	return rect.bottom >= area.top - 12 || rect.top >= area.top - 6;
}