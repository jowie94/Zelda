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

	virtual void Collides(const cRect& position, const int status, cRect& collision, float& damage);

	virtual void Logic(int *map, cPlayer& player);

	virtual void Draw() = 0;

	void SetAttacking(bool attacking);

	bool GetAttacking() const;

	FMOD::Sound* GetKillSound();
	FMOD::Sound* GetHitSound();

protected:
	std::set<int> weapon_ids;

private:
	int texture;

	bool attacking;
	FMOD::Sound* kill_sound, *hit_sound;
};
