#include "cPlayer.h"
#include "cWeapon.h"

cPlayer::cPlayer()
{
	attacking = false;
	SetLife(INITIAL_LIFE);
}
cPlayer::~cPlayer(){}

void cPlayer::AAttack()
{
	if (!attacking) {
		Stop();
		attacking = true;
		ActivateWeapon(aWeapon);
		int x, y, w, h;
		GetPosition(&x, &y);
		GetWidthHeight(&w, &h);
		aWeapon->SetPosition(x, y);
		aWeapon->Attack(GetLife() == INITIAL_LIFE, GetState());
	}
}

void cPlayer::SetAWeapon(Weapon weapon)
{
	aWeapon = GetWeapon(weapon);
}


void cPlayer::MoveRight(int* map)
{
	if (!attacking)
		cBicho::MoveRight(map);
	int x_aux;
	int y_aux;
	GetPosition(&x_aux, &y_aux);
	if (x_aux == 240) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_RIGHT;
	}
}

void cPlayer::MoveLeft(int* map)
{
	if (!attacking)
		cBicho::MoveLeft(map);
	int x_aux;
	int y_aux;
	GetPosition(&x_aux, &y_aux);
	if (x_aux == 0) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_LEFT;
	}
}

void cPlayer::MoveUp(int* map)
{
	if (!attacking)
		cBicho::MoveUp(map);
	int x_aux;
	int y_aux;
	GetPosition(&x_aux, &y_aux);
	if (y_aux == 160) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_TOP;
	}
}

void cPlayer::MoveDown(int* map)
{
	if (!attacking)
		cBicho::MoveDown(map);
	int x_aux;
	int y_aux;
	GetPosition(&x_aux, &y_aux);
	if (y_aux == 0) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_BOTTOM;
	}
}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	std::list<cWeapon*> weapons;
	GetActiveWeapons(weapons);

	for (auto w : weapons)
	{
		w->Draw();
	}

	switch(GetState())
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

	if (attacking)
		yo = 0.75f;

	xf = xo + 0.25f;
	yf = yo - 0.25f;

	DrawRect(tex_id,xo,yo,xf,yf);
}

void cPlayer::Logic(int* map)
{
	cBicho::Logic(map);
	if (!isDead())
		attacking = !aWeapon->isDead();
}

int cPlayer::getDirectionTransition() {
	return direction_transition;
}