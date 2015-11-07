#include "cPlayer.h"
#include "cWeapon.h"
#include "cScene.h"

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
	if (IsDoor(map, y_aux)) {
		SetState(STATE_DOOR);
		direction_transition = TRANSITION_INSIDE;
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

	std::set<cWeapon*> weapons;
	GetActiveWeapons(weapons);

	for (auto w : weapons)
	{
		w->Draw();
	}

	int state = GetState();

	switch(state)
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

	if (transition && direction_transition==TRANSITION_INSIDE)
		yo -= (0.25f/TILE_SIZE)*transition_num;
	else if(transition && direction_transition == TRANSITION_OUTSIDE)
		yo = yf + (0.25f/TILE_SIZE)*transition_num;

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


void cPlayer::Logic(int* map)
{
	cBicho::Logic(map);
	if (!isDead())
		attacking = aWeapon->LockPlayer();
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
	if (map[(y_tile * SCENE_WIDTH) + x_tile] == 23 && y_coord%TILE_SIZE == 0)
		return true;
	return false;
}