#include "cDrop.h"

cDrop::cDrop(int texture, DROP_TYPE drop, float amount)
{
	this->texture = texture;
	this->drop = drop;
	this->amount = amount;
	SetWidthHeight(16, 16);
	SetFramesToDie(3);
	SetLife(0);
}

cDrop::~cDrop()
{
}

void cDrop::Hurt(int *map) {};

DROP_TYPE cDrop::GetType() const
{
	return drop;
}

float cDrop::GetAmount()
{
	if (!isDead())
		return amount;
	return 0;
}

void cDrop::Draw()
{
	if (!isDead())
	{
		float xo, yo, xf, yf;

		xo = 0.25 * GetType();
		yo = 0.25 + 0.25 * GetFrame();

		xf = xo + 0.25;
		yf = yo - 0.25;

		NextFrame(2);

		DrawRect(texture, xo, yo, xf, yf);
	}
}
