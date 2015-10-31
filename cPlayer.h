#pragma once

#include "cBicho.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define INITIAL_LIFE		3.f

#define PLAYER_DOOR			4

#define TRANSITION_RIGHT    1
#define TRANSITION_LEFT		2
#define TRANSITION_TOP		3
#define TRANSITION_BOTTOM   4
#define TRANSITION_INSIDE   5


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

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void MoveUp(int* map);
	void MoveDown(int* map);

	void Logic(int* map) override;
	int getDirectionTransition();

private:
	cWeapon* aWeapon; // Weapon assigned to button A

	bool attacking;
	int direction_transition;
};
