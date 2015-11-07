#include "eOctorok.h"
#include "cWeapon.h"
#include "cScene.h"

eOctorok::eOctorok(int texture, float life, float damage) : cEnemy(texture, life, damage) {}

eOctorok::~eOctorok() {}

void eOctorok::Collides(cRect& position, const int orientation, cRect& collision, float& damage)
{
	std::set<cWeapon*> active_wp;
	GetActiveWeapons(active_wp);

	bool collides = false;

	if (GetOrientation() != orientation)
		for (cWeapon* wp : active_wp)
		{
			collides = wp->Collides(&position);
			if (collides)
			{
				wp->GetArea(&collision);
				damage = wp->GetDamage();
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
}

void eOctorok::Draw()
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
	}

	/*if (attacking)
		yo = 0.75f;*/

	xf = xo + 0.25f;
	yf = yo - 0.25f;

	DrawRect(GetTexture(), xo, yo, xf, yf);
}
