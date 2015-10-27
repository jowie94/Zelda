
#include "cPlayer.h"

cPlayer::cPlayer()
{
	onAttack = false;
}
cPlayer::~cPlayer(){}

void cPlayer::Attack(Weapon weapon)
{
	Stop();
	switch (weapon)
	{
	case SWORD:
		onAttack = true;
		break;
	}
}

void cPlayer::Draw(int tex_id)
{	
	float xo,yo,xf,yf;

	switch(GetState())
	{
		//1
		case STATE_LOOKLEFT:    xo = yo = 0.125f;
			break;
		//4
		case STATE_LOOKRIGHT:	xo = 0.375f; yo = 0.125f;
								break;
		case STATE_LOOKUP:		xo = 0.25; yo = 0.125f;
								break;
		case STATE_LOOKDOWN:	xo = 0.0f; yo = 0.125f;
								break;
		//1..3
		case STATE_WALKLEFT:	xo = 0.125f;	yo = 0.125f + (GetFrame()*0.125f);
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
	}

	xf = xo + 0.125;
	yf = yo - 0.125;

	DrawRect(tex_id,xo,yo,xf,yf);
}
