#pragma once
#include "cBicho.h"
#include "cPlayer.h"

class cEnemy :
	public cBicho
{
public:
	cEnemy(int texture, float life, float damage);
	~cEnemy();

	int GetTexture() const;

	virtual void Collides(cRect& position, const int status, cRect& collision, float& damage);

	virtual void Logic(int *map, cPlayer& player);

	virtual void Draw() = 0;

private:
	int texture;
};
