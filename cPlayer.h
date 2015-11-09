#pragma once

#include "cBicho.h"
#include "Constants.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define INITIAL_LIFE		3.0f

#define PLAYER_DOOR			4


enum Weapon
{
	SWORD
};

class cEnemy;

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void AAttack();

	void SetAWeapon(Weapon weapon); // Set weapon assigned to button A
	void UpdateTransitionPos(int transition_num);

	void Collides(const cRect& position, const int status, cRect& collision, float& damage);

	void Draw(int tex_id);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void MoveUp(int* map);
	void MoveDown(int* map);

	void Logic(int* map, const std::list<cEnemy*> enemies);
	int GetDirectionTransition(void);
	void StartTransition(int* outside_pos);
	void SetDirectionTransition(int direction_transition);
	void SetStateAfterTransition(void);
	void SetTransition(bool trans);

	bool IsDoor(int* map, int y_aux);

private:
	cWeapon* aWeapon; // Weapon assigned to button A

	bool attacking, lock;
	int hurt;
	bool transition;
	int direction_transition;
	int transition_num;
	int hurt_direction;
};
