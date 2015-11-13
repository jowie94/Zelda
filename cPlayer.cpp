#include "cPlayer.h"
#include "cWeapon.h"
#include "cScene.h"
#include "cEnemy.h"

cPlayer::cPlayer()
{
	attacking = lock = false;
	hurt = 0;
	SetLife(INITIAL_LIFE);
	SetHearts(INITIAL_LIFE);
	SetRupees(0);
}
cPlayer::~cPlayer(){}

void cPlayer::AAttack()
{
	if (!attacking && GetState() != STATE_DYING) {
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

void cPlayer::BAttack()
{
	if (!attacking && GetState() != STATE_DYING) {
		Stop();
		attacking = true;
		ActivateWeapon(bWeapon);
		int x, y, w, h;
		GetPosition(&x, &y);
		GetWidthHeight(&w, &h);
		bWeapon->SetPosition(x, y);
		bWeapon->Attack(GetLife() == INITIAL_LIFE, GetState());
	}
}

void cPlayer::SetAWeapon(Weapon weapon)
{
	aWeapon = GetWeapon(weapon);
}

void cPlayer::SetBWeapon(Weapon weapon)
{
	bWeapon = GetWeapon(weapon);
}

void cPlayer::MoveRight(int* map)
{
	if (!lock)
		cBicho::MoveRight(map);
	int x_aux;
	int y_aux;
	GetPosition(&x_aux, &y_aux);
	if (x_aux >= 240) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_RIGHT;
	}
}

void cPlayer::MoveLeft(int* map)
{
	if (!lock)
		cBicho::MoveLeft(map);
	int x_aux;
	int y_aux;
	GetPosition(&x_aux, &y_aux);
	if (x_aux <= 0) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_LEFT;
	}
}

void cPlayer::MoveUp(int* map)
{
	if (!lock)
		cBicho::MoveUp(map);
	int x_aux;
	int y_aux;
	GetPosition(&x_aux, &y_aux);
	if (y_aux >= 160) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_TOP;
	}
	if (IsDoor(map, y_aux)) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_INSIDE;
	}
}

void cPlayer::MoveDown(int* map)
{
	if (!lock)
		cBicho::MoveDown(map);
	int x_aux;
	int y_aux;
	GetPosition(&x_aux, &y_aux);
	if (y_aux <= 0) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_BOTTOM;
	}
}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	int state = GetState();

	if (state != STATE_DYING)
	{
		std::set<cWeapon*> weapons;
		GetActiveWeapons(weapons);

		for (auto w : weapons)
		{
			w->Draw();
		}
	}

	switch(state)
	{
		//1
		case STATE_LOOKLEFT:    xo = yo = 0.125f;
								break;
		//4
		case STATE_LOOKRIGHT:	xo = 0.375f; yo = 0.25f;
								break;
		case STATE_LOOKUP:		xo = 0.25f; yo = 0.125f;
								break;
		case STATE_LOOKDOWN:	xo = 0.0f; yo = 0.125f;
								break;
		//1..3
		case STATE_WALKLEFT:	xo = 0.125f; yo = 0.125f + (GetFrame()*0.125f);
								NextFrame(2);
					
								break;
		//4..6
		case STATE_WALKRIGHT:	xo = 0.375f; yo = 0.125f + (GetFrame()*0.125f);
								NextFrame(2);
								break;
		case STATE_WALKUP:		xo = 0.25f; yo = 0.125f + (GetFrame()*0.125f);
			NextFrame(2);
			break;
		case STATE_WALKDOWN:	xo = 0.0f; yo = 0.125f + (GetFrame()*0.125f);
			NextFrame(2);
			break;

		case STATE_DYING: xo = 0.125f * (GetFrame() % 4); yo = 0.125f;
			if (GetFrame() > 7)
			{
				xo += 0.5f;
				yo += 0.125f;
			}

			NextFrame(GetFramesToDie() + 1);
			break;
	}

	if (hurt)
	{
		yo += 0.25f * (hurt % 2);
	}

	if (attacking && lock)
	{
		xo += 0.5f;
		yo = 0.125f;
	}

	xf = xo + 0.125f;
	yf = yo - 0.125f;

	if (transition && direction_transition==TRANSITION_INSIDE)
		yo -= (0.125f/TILE_SIZE)*transition_num;
	else if(transition && direction_transition == TRANSITION_OUTSIDE)
		yo = yf + (0.125f/TILE_SIZE)*transition_num;

	DrawRect(tex_id,xo,yo,xf,yf);
}


void cPlayer::UpdateTransitionPos(int transition_num) {
	this->transition_num = transition_num;
	int x, y, w, h;
	GetPosition(&x, &y);
	GetWidthHeight(&w, &h);

	switch (GetDirectionTransition())
	{
	case TRANSITION_BOTTOM:
		if(transition_num < SCENE_HEIGHT*2 - 1)
			y += TILE_SIZE/2;
		break;
	case TRANSITION_INSIDE:
		--h;
		break;
	case TRANSITION_OUTSIDE:
		++h;
		break;
	case TRANSITION_TOP:
		if (transition_num < SCENE_HEIGHT*2 - 1)
			y -= TILE_SIZE/2;
		break;
	case TRANSITION_LEFT:
		if (transition_num < SCENE_WIDTH*2 - 1)
			x += TILE_SIZE/2;
		break;
	case TRANSITION_RIGHT:
		if (transition_num < SCENE_WIDTH*2 - 1)
			x -= TILE_SIZE/2;
		break;
	}
	SetPosition(x, y);
	SetWidthHeight(w, h);
}

void cPlayer::Collides(const cRect& position, const int status, cRect& collision, float& damage)
{
	std::set<cWeapon*> active_wp;
	GetActiveWeapons(active_wp);

	for (cWeapon* wp : active_wp)
	{
		wp->Collides(position, status, collision, damage);
		if (damage)
			break;
	}
}

void cPlayer::Hurt(int *map)
{
	--hurt;
	ToggleHurt(hurt != 0);
	int xoff = 0, yoff = 0, x, y, nx, ny;
	if (lock)
	{
		std::set<cWeapon*> weapons;
		GetActiveWeapons(weapons);
		for (cWeapon* w : weapons)
			w->Finalize();
		GetPosition(&x, &y);
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
		GetPosition(&nx, &ny);

		if (!hurt || x == nx && y == ny || hurt < 6*8)
			lock = false;
	}
}

void cPlayer::Logic(int* map, const std::list<cEnemy*> enemies)
{
	cBicho::Logic(map);
	if (!isDead() && GetState() != STATE_DYING)
	{
		std::set<cWeapon*> weapons;
		GetActiveWeapons(weapons);
		attacking = false;
		for (cWeapon* w : weapons)
			attacking = attacking || !w->isDead();

		if (!IsHurt())
		{
			lock = false;
			for (cWeapon* w : weapons)
				lock = lock || w->LockPlayer() || IsHurt();
			cRect coll, area;
			float damage = 0;
			int state = GetState() % 4;
			GetArea(&area);
			for (cEnemy* en : enemies)
			{
				float tmpd = 0;
				en->Collides(area, state, coll, tmpd);
				damage += tmpd;
			}

			if (damage != 0)
			{
				if (!hurt_sound)
					FMOD_RESULT res = fmod_system->createSound("sounds/link-hurt.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &hurt_sound);
				PlaySound(hurt_sound);
				DecrementLife(damage);
				for (cWeapon* w : weapons)
					w->Finalize();
				lock = true;
				if (GetLife())
					hurt = 8 * 8;
				else
					hurt = 2 * 8;
				ToggleHurt(true);
				Stop();
				CalculateCollisionMovement(coll, hurt_direction);
			}
		}

		if (GetLife() <= 0)
		{
			if (hurt)
				lock = true;
			else
			{
				hurt = 0;
				ToggleHurt(false);
				ResetFrame();
				SetFramesToDie(11);
				lock = true;
				SetState(STATE_DYING);
			}
		}
	}
}

int cPlayer::GetDirectionTransition(void) {
	return direction_transition;
}

void cPlayer::SetDirectionTransition(int new_direction_transition) {
	direction_transition = new_direction_transition;
}

void cPlayer::SetStateAfterTransition(void) {
	SetTransition(false);
	transition_num = 1;
	SetWidthHeight(TILE_SIZE, TILE_SIZE);
	if(direction_transition==TRANSITION_INSIDE)
		SetPosition(112, 0);
	int dir = GetDirectionTransition();
	switch (dir)
	{
	case TRANSITION_BOTTOM:
		SetState(STATE_LOOKDOWN);
		break;
	case TRANSITION_TOP:
		SetState(STATE_LOOKUP);
		break;
	case TRANSITION_LEFT:
		SetState(STATE_LOOKLEFT);
		break;
	case TRANSITION_RIGHT:
		SetState(STATE_LOOKRIGHT);
		break;
	case TRANSITION_INSIDE:
		SetState(STATE_LOOKUP);
		break;
	case TRANSITION_OUTSIDE:
		SetState(STATE_LOOKDOWN);
		break;
	}
}

void cPlayer::StartTransition(int* outside_pos) {
	transition = true;
	if (direction_transition == TRANSITION_OUTSIDE) {
		int w, h;
		GetWidthHeight(&w, &h);
		h = 0;
		SetWidthHeight(w, h);
		SetPosition(outside_pos[0]*TILE_SIZE, outside_pos[1]*TILE_SIZE);
	}
}


void cPlayer::SetTransition(bool trans) {
	transition = trans;
}

bool cPlayer::IsDoor(int* map, int y_coord) {
	int x_tile;
	int y_tile;
	GetTile(&x_tile, &y_tile);
	if (map[(y_tile * SCENE_WIDTH) + x_tile] == 23 && y_coord%TILE_SIZE < 2)
		return true;
	return false;
}

void cPlayer::SetHearts(int hearts) {
	this->hearts = hearts;
}

int cPlayer::GetHearts() {
	return hearts;
}

void cPlayer::SetRupees(int rupees) {
	if (rupees > 999)
		rupees = 999;
	this->rupees = rupees;
}

int cPlayer::GetRupees() {
	return rupees;
}

bool cPlayer::HasSword() {
	return bWeapon;
}

bool cPlayer::HasBow() {
	return aWeapon;
}
