#pragma once
#include "cWeapon.h"
class wEnergyBall :
	public cWeapon
{
public:
	wEnergyBall(float damage, int direction[2]);
	~wEnergyBall();

	bool CollidesMapWall(int* map, bool right);
	bool CollidesMapFloor(int *map, bool up);

	void Logic(int* map) override;
	void Attack(bool special, int orientation) override;
	void Draw() override;
	bool LockPlayer() override;

private:
	int direction[2];
};

