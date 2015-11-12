#pragma once
#include "cEnemy.h"
class eAquamentus :
	public cEnemy
{
public:
	eAquamentus(int texture, float life, float damage);
	~eAquamentus();

	void Hurt(int* map) override;
	void Collides(const cRect& position, const int status, cRect& collision, float& damage) override;
	void Logic(int* map, cPlayer& player) override;
	void Draw() override;

private:
	int action, steps;
	bool open_mouth;

	// Checks if the collision is in the head
	bool CollidesWithHead(cRect& rect);
};

