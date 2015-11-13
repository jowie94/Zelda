#include "eOctorok.h"
#include "cWeapon.h"
#include "cScene.h"
#include "wRock.h"

eOctorok::eOctorok(int texture, float life, float damage) : cEnemy(texture, life, damage)
{
	steps = action = 0;
	SetWidthHeight(16, 16);

	fmod_system->createSound("sounds/shield.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &shield_sound);
}

eOctorok::~eOctorok() {}

void eOctorok::Collides(const cRect& position, const int orientation, cRect& collision, float& damage)
{
	std::set<cWeapon*> active_wp;
	GetActiveWeapons(active_wp);

	bool collides = false;

	int diff = abs(orientation - GetOrientation()), cori = GetOrientation() % 4;
	bool coll = (orientation < 1 && cori > 1) || (orientation > 1 && cori < 1) || diff != 1;

	for (cWeapon* wp : active_wp)
	{
		wp->Collides(position, orientation, collision, damage);
		collides = damage > 0;
		if (collides)
		{
			if (coll)
			{
				wp->GetArea(&collision);
				damage = wp->GetDamage();
			}
			else
			{
				PlaySound(shield_sound);
				damage = 0;
			}
			wp->SetLife(0);
			wp->SetFramesToDie(0);
			break;
		}

	}

	if (!collides)
	{
		cEnemy::Collides(position, orientation, collision, damage);
	}
}

void eOctorok::Logic(int* map, cPlayer& player)
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
			PlaySound(GetHitSound());
			DecrementLife(damage);
			ToggleHurt(true);
			hurt = 4 * 8;
			Stop();
			CalculateCollisionMovement(collision, hurt_direction);
		}
		else if (!IsHurt())
		{
			int x, y;
			GetPosition(&x, &y);
			int offset = STEP_LENGTH - 1;

			if (!steps)
			{
				action = rand() % 4;
				bool attacking = rand() % 2;
				//bool attacking = true;
				SetAttacking(attacking);
				switch (action)
				{
				case 0:
					if (attacking)
					{
						SetState(STATE_LOOKUP);
					}
					else
						MoveUp(offset, map);
					break;
				case 1:
					if (attacking)
					{
						SetState(STATE_LOOKLEFT);
					}
					else
						MoveLeft(offset, map);
					break;
				case 2:
					if (attacking)
					{
						SetState(STATE_LOOKDOWN);
					}
					else
						MoveDown(offset, map);
					break;
				case 3:
					if (attacking)
					{
						SetState(STATE_LOOKRIGHT);
					}
					else
						MoveRight(offset, map);
					break;
				}

				if (attacking)
				{
					cWeapon* rock = new wRock(GetDamage());
					rock->SetWidthHeight(16, 16);
					rock->SetTexture(GetTexture());
					rock->SetPosition(x, y);
					rock->Attack(false, GetState());
					ActivateWeapon(rock);
				}
				steps = (rand() % 16) * 16;
			}
			else
			{
				int nx, ny;
				if (!GetAttacking())
				{
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
				else
				{
					std::set<cWeapon*> weapons;
					GetActiveWeapons(weapons);
					SetAttacking(weapons.size() != 0);
				}
			}
		}

		if (GetLife() <= 0 && GetState() != STATE_DYING)
		{
			SetState(STATE_DYING);
			hurt = 0;
			ToggleHurt(false);
			SetFramesToDie(2);
			ResetFrame();
			PlaySound(GetKillSound());
		}
	}
}

void eOctorok::Draw()
{
	if (!isDead())
	{
		float xo, yo, xf, yf;

		std::set<cWeapon*> weapons;
		GetActiveWeapons(weapons);

		for (auto w : weapons)
		{
			w->Draw();
		}

		int state = GetState();

		switch (state)
		{
			//1
		case STATE_LOOKLEFT:    xo = yo = 0.25f;
			break;
			//4
		case STATE_LOOKRIGHT:	xo = 0.75f; yo = 0.5f;
			break;
		case STATE_LOOKUP:		xo = 0.5f; yo = 0.25f;
			break;
		case STATE_LOOKDOWN:	xo = 0.0f; yo = 0.25f;
			break;
			//1..3
		case STATE_WALKLEFT:	xo = 0.25f;	yo = 0.25f + (GetFrame()*0.25f);
			NextFrame(2);

			break;
			//4..6
		case STATE_WALKRIGHT:	xo = 0.75f; yo = 0.25f + (GetFrame()*0.25f);
			NextFrame(2);
			break;
		case STATE_WALKUP:		xo = 0.5f; yo = 0.25f + (GetFrame()*0.25f);
			NextFrame(2);
			break;
		case STATE_WALKDOWN:	xo = 0.0f; yo = 0.25f + (GetFrame()*0.25f);
			NextFrame(2);
			break;
		case STATE_DYING:		xo = 0.0f + (GetFrame()*0.25f); yo = 0.75f;
			NextFrame(3);
			break;

		}

		/*if (attacking)
			yo = 0.75f;*/

		xf = xo + 0.25f;
		yf = yo - 0.25f;

		DrawRect(GetTexture(), xo, yo, xf, yf);
	}
}

void eOctorok::Hurt(int* map)
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
