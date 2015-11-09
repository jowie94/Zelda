#pragma once
#include "cWeapon.h"

enum SpecialState
{
	NONE,
	INIT,
	MOVE,
	COLLIDE
};

class wSword :
	public cWeapon
{
public:
	wSword(float damage);
	~wSword();


	void Collides(const cRect& position, const int status, cRect& collision, float& damage) override;
	void Logic(int* map) override;
	void Attack(bool special, int orientation) override;

	void Draw() override;

	SpecialState GetSpecialState();
	void SetSpecialState(SpecialState sp);

	bool LockPlayer() override;

	bool isDead() override;

	void Hurt(int *map) override;
private:
	wSword(float damage, bool special);

	bool draw_sword, is_special;

	SpecialState special_state;

	bool SpecialCollidesWithBorder();

	wSword *special;
};

