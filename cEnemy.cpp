#include "cEnemy.h"
#include "cScene.h"


cEnemy::cEnemy(int texture, float life, float damage) : cBicho(life)
{
	SetDamage(damage);
	this->texture = texture;

	fmod_system->createSound("sounds/enemy-kill.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &kill_sound);
	fmod_system->createSound("sounds/enemy-hit.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &hit_sound);
}

cEnemy::~cEnemy()
{
}

int cEnemy::GetTexture() const
{
	return texture;
}

void cEnemy::Collides(const cRect& position, const int orientation, cRect& collision, float& damage)
{
	if(GetState() != STATE_DYING && cBicho::Collides(&position))
	{
		GetArea(&collision);
		damage = GetDamage();
	}
}

void cEnemy::SetAttacking(bool attacking)
{
	this->attacking = attacking;
}

bool cEnemy::GetAttacking() const
{
	return attacking;
}

FMOD::Sound* cEnemy::GetKillSound()
{
	return kill_sound;
}

FMOD::Sound* cEnemy::GetHitSound()
{
	return hit_sound;
}

void cEnemy::Logic(int* map, cPlayer& player)
{
	cBicho::Logic(map);
}


bool cEnemy::CollidesMapWall(int *map, bool right)
{
	int x, y;
	GetPosition(&x, &y);
	if (x < TILE_SIZE || x >= TILE_SIZE*(SCENE_WIDTH-1))
		return true;
	else
		return cBicho::CollidesMapWall(map, right);
}

bool cEnemy::CollidesMapFloor(int *map, bool up)
{
	int x, y;
	GetPosition(&x, &y);
	if (y < TILE_SIZE || y >= TILE_SIZE*(SCENE_HEIGHT - 1))
		return true;
	else cBicho::CollidesMapFloor(map, up);
}