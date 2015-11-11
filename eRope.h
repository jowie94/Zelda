#pragma once
#include "cEnemy.h"
class eRope :
	public cEnemy
{
public:
	eRope(int texture, float life, float damage);
	~eRope();

	void Hurt(int* map) override;
	void Logic(int* map, cPlayer& player) override;
	void Draw() override;

private:
	bool PlayerInY(cRect &rect);
	bool PlayerInX(cRect &rect);
};

