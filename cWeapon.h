#pragma once
#include "cBicho.h"

class cWeapon : public cBicho
{
public:
	cWeapon(float damage);
	~cWeapon();

	virtual void Attack(bool special, int orientation);

	void SetTexture(int id);
	int GetTexture();
	virtual void Draw();

	virtual bool LockPlayer();

	virtual void Collides(const cRect& position, const int status, cRect& collision, float& damage);
private:
	int texture_id;
};

