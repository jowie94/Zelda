#include "eRope.h"

eRope::eRope(int texture, float life, float damage) : cEnemy(texture, life, damage)
{
//	steps = action = 0;
}

eRope::~eRope()
{
}

void eRope::Hurt(int* map)
{
}

void eRope::Logic(int* map, cPlayer& player)
{
	cBicho::Logic(map);
}

void eRope::Draw()
{
}

bool eRope::PlayerInY(cRect& rect)
{
}

bool eRope::PlayerInX(cRect& rect)
{
}
