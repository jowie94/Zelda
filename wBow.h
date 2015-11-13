#pragma once
#include "cWeapon.h"
class wBow :
	public cWeapon
{
public:
	wBow(float damage);
	~wBow();

	void Logic(int* map) override;
	void Attack(bool special, int orientation) override;
	void Draw() override;
	bool LockPlayer() override;

	void Finalize() override;
private:
	int bow_x, bow_y;

	bool init;
	FMOD::Sound* arrow_sound;
};

