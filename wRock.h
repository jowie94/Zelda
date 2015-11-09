#pragma once
#include "cWeapon.h"
class wRock :
	public cWeapon
{
public:
	wRock(float damage);
	~wRock();

	void Attack(bool special, int orientation) override;

	void Logic(int* map) override;
	void Draw() override;
};

