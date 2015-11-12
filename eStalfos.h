#pragma once
#include "cEnemy.h"
class eStalfos :
	public cEnemy
{
public:
	eStalfos(int texture, float life, float damage);
	~eStalfos();

	void Logic(int *map, cPlayer& player) override;

	void Draw() override;

	void Hurt(int* map) override;

private:
	int action, steps, hurt, hurt_direction;
};

