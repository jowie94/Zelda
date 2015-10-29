#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define INITIAL_LIFE		3.f

enum Weapon
{
	SWORD
};

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void AAttack();

	void SetAWeapon(Weapon weapon); // Set weapon assigned to button A

	void Draw(int tex_id);

	void Logic(int* map) override;

private:
	cWeapon* aWeapon; // Weapon assigned to button A

	bool attacking;
};
