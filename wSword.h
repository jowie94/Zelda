#pragma once
#include "cWeapon.h"

class wSword :
	public cWeapon
{
public:
	wSword(float damage);
	~wSword();

	void Logic(int* map) override;
	void Attack(bool special, int orientation) override;

	void Draw() override;

private:
	bool special;
};

