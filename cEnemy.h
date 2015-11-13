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

	bool CollidesMapWall(int *map, bool right) override;
	bool CollidesMapFloor(int *map, bool up) override;



	FMOD::Sound* GetKillSound();
	FMOD::Sound* GetHitSound();
private:
	int texture;

	bool attacking;
	FMOD::Sound* kill_sound, *hit_sound;
};
