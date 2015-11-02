#include "wSword.h"

wSword::wSword(float damage) : wSword(damage, false) {}

wSword::wSword(float damage, bool special) : cWeapon(damage)
{
	is_special = special;
	this->special = NULL;
}

wSword::~wSword() {}

void wSword::Attack(bool special, int orientation)
{
	if (cWeapon::isDead())
	{
		int x, y;
		GetPosition(&x, &y);
		if (!special || (this->special && this->special->GetSpecialState() != INIT)) {
			SetSpecialState(is_special ? INIT : NONE);

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
			draw_sword = true;
		} else
		{
			draw_sword = false;
			SetFramesToDie(0);
			SetSpecialState(NONE);
			wSword* sw = new wSword(GetDamage(), true);
			sw->SetPosition(x, y);
			sw->SetTexture(GetTexture());
			sw->Attack(false, orientation);
			this->special = sw;
		}

	}
}

void wSword::Draw()
{
	if (!isDead()) {
		float xo, yo, xf, yf;

		bool sp = is_special && GetSpecialState() == MOVE;

		switch (GetState())
		{
		case STATE_LOOKLEFT:
			if (sp)
			{
				xo = 0.125; yo = xf = 0.375; yf = 0.25;
			} else
			{
				xo = yo = 0.125f; xf = 0.375f; yf = 0.0f;
			}
			break;
		case STATE_LOOKRIGHT:
			if (sp)
			{
				xo = 0.125; yo = 0.5; xf = yf = 0.375;
			} else
			{
				xo = 0.125f; yo = 0.25f; xf = 0.375f; yf = 0.125f;
			}
			break;
		case STATE_LOOKUP:
			if (sp)
			{
				xo = 0.375f; yo = xf = 0.5f; yf = 0.25f;
			} else
			{
				xo = 0.375f; yo = 0.25f; xf = 0.5f; yf = 0.0f;
			}
			break;
		case STATE_LOOKDOWN:
			if (sp)
			{
				xo = 0.0f; yo = 0.5; xf = 0.125; yf = 0.25f;
			} else
			{
				xo = yf = 0.0f; yo = 0.25f; xf = 0.125f;
			}
			break;
		}

		if (draw_sword) {
			NextFrame(GetFramesToDie() + 1);
			DrawRect(GetTexture(), xo, yo, xf, yf);
		}

		if (GetSpecialState() == COLLIDE)
			NextFrame(5);
	}
	if (special)
		special->Draw();
}

SpecialState wSword::GetSpecialState()
{
	return special_state;
}

void wSword::SetSpecialState(SpecialState sp)
{
	this->special_state = sp;
}

bool wSword::LockPlayer()
{
	return !cWeapon::isDead() || (special && special->GetSpecialState() == INIT);
}

bool wSword::isDead()
{
	return !special && cWeapon::isDead();
}

bool wSword::SpecialCollidesWithBorder()
{
	int x, y;
	GetPosition(&x, &y);

	return x <= 0 || x >= 15*16 || y <= 0 || y >= 10*16; // TODO: Include Scene.h and use constants
}

void wSword::Logic(int *map)
{
	if (special) {
		if (special->isDead())
			special = NULL;
		else
			special->Logic(map);
	}
	if (!cWeapon::isDead())
	{
		if ((GetFramesToDie() - 1) <= GetFrame() && GetSpecialState() == NONE)
		{
			SetLife(0);
			draw_sword = false;
			while (GetFrame() != 0)
				NextFrame(1);
		}
		else
		{
			int x, y, sx, sy, frame;
			frame = GetFrame();
			GetPosition(&x, &y);
			bool sp = is_special && GetSpecialState() == MOVE;

			switch (GetState())
			{
			case STATE_LOOKLEFT:
				if (sp)
					x -= 2;
				else if (!is_special)
					x += frame;
				break;
			case STATE_LOOKRIGHT:
				if (sp)
					x += 2;
				else if (!is_special)
					x -= frame;
				break;
			case STATE_LOOKUP:
				if (sp)
					y += 2;
				else if (!is_special)
					y -= frame;
				break;
			case STATE_LOOKDOWN:
				if (sp)
					y -= 2;
				else if (!is_special)
					y += frame;
				break;
			}
			SetPosition(x, y);

			if (GetSpecialState() == INIT && frame == 2)
				SetSpecialState(MOVE);

			// Check special position if border
			if (sp && SpecialCollidesWithBorder())
			{
				SetSpecialState(COLLIDE);
				while (GetFrame() != 0) // TODO: Implement reset current frame function
					NextFrame(1);
				SetFramesToDie(5);
			}

			if (is_special && GetSpecialState() == COLLIDE && GetFrame() == 4)
				SetSpecialState(NONE);

			if (!is_special && frame >= 2)
				draw_sword = false;
		}
	}
	cBicho::Logic(map);
}
