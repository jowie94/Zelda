#pragma once

#include "cBicho.h"
#include "Constants.h"

#define PLAYER_START_CX		3
#define PLAYER_START_CY		2

#define INITIAL_LIFE		3.0f

#define PLAYER_DOOR			4


enum Weapon
{
	SWORD,
	BOW
};

class cEnemy;

class cPlayer: public cBicho
{
public:
	cPlayer();
	~cPlayer();

	void AAttack();
	void BAttack();

	void SetAWeapon(Weapon weapon); // Set weapon assigned to button A
	void SetBWeapon(Weapon weapon); // Set weapon assigned to button B
	void UpdateTransitionPos(int transition_num);

	void Collides(const cRect& position, const int status, cRect& collision, float& damage);

	void Draw(int tex_id);

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void MoveUp(int* map);
	void MoveDown(int* map);

	void Hurt(int *map) override;
	void Logic(int* map, const std::list<cEnemy*> enemies);
	int GetDirectionTransition(void);
	void StartTransition(int* outside_pos);
	void SetDirectionTransition(int direction_transition);
	void SetStateAfterTransition(void);
	void SetTransition(bool trans);

	void SetHearts(int hearts);
	int GetHearts();

	void SetRupees(int rupees);
	int GetRupees();

	bool HasSword();
	bool HasBow();

	bool IsDoor(int* map, int y_aux);

private:
	cWeapon* aWeapon; // Weapon assigned to button A
	cWeapon* bWeapon; // Weapon assigned to button B

	bool attacking, lock;
	int hurt;
	bool transition;
	int direction_transition;
	int transition_num;
	int hurt_direction;
	int hearts;
	int rupees;
	FMOD::Sound *hurt_sound;
};
