#pragma once
#include "cBicho.h"

enum DROP_TYPE
{
	HEART,
	RUPEE,
	TRIFORCE
};

class cDrop :
	public cBicho
{
public:
	cDrop(int texture, DROP_TYPE drop, float amount);
	~cDrop();

	void Hurt(int *map) override;

	bool Collides(const cRect* rc);
	DROP_TYPE GetType() const;

	float GetAmount();


	void Draw();

private:
	int texture;
	DROP_TYPE drop;
	float amount;
};

