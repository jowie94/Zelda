#pragma once
#include "cEnemy.h"
class eOctorok :
	public cEnemy
{
public:
	eOctorok(int texture, float life, float damage);
	~eOctorok();

	void Collides(cRect& position, const int status, cRect& collision, float& damage) override;
	void Logic(int *map, cPlayer& player) override;

	void Draw() override;
};

