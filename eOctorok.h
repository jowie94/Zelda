#pragma once
#include "cEnemy.h"
class eOctorok :
	public cEnemy
{
public:
	eOctorok(int texture, float life, float damage);
	~eOctorok();

	void Collides(const cRect& position, const int status, cRect& collision, float& damage) override;
	void Logic(int *map, cPlayer& player) override;

	void Draw() override;

	void Hurt(int* map) override;
private:
	int action, steps, hurt_direction, hurt;
	FMOD::Sound* shield_sound;
};

