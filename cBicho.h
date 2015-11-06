#pragma once

#include "Globals.h"
#include "cTexture.h"

#define FRAME_DELAY		8
#define STEP_LENGTH		1
#define JUMP_HEIGHT		96
#define JUMP_STEP		4

#define STATE_LOOKLEFT		0
#define STATE_LOOKRIGHT		1
#define STATE_LOOKUP		2
#define STATE_LOOKDOWN		3
#define STATE_WALKLEFT		4
#define STATE_WALKRIGHT		5
#define STATE_WALKUP		6
#define STATE_WALKDOWN		7
#define STATE_DOOR			8


class cWeapon; // Forward declaration to avoid circular dependency (Weird compiler stuff)

class cRect
{
public:
	int left,top,
		right,bottom;
};

class cBicho
{
public:
	cBicho(void);
	cBicho(float life);
	cBicho(int x,int y,int w,int h);
	virtual ~cBicho(void);

	void SetPosition(int x,int y);
	void GetPosition(int *x,int *y);
	void SetTile(int tx,int ty);
	void GetTile(int *tx,int *ty);
	void SetWidthHeight(int w,int h);
	void GetWidthHeight(int *w,int *h);

	bool Collides(cRect *rc);
	bool CollidesMapWall(int *map,bool right);
	bool CollidesMapFloor(int *map, bool up);
	void GetArea(cRect *rc);
	void DrawRect(int tex_id,float xo,float yo,float xf,float yf);

	void  SetLife(float life);
	void  DecrementLife(float amount);
	float GetLife() const;

	void  SetDamage(float damage);
	float GetDamage();

	void MoveRight(int *map);
	void MoveLeft(int *map);
	void MoveUp(int* map);
	void MoveDown(int* map);
	void Stop();
	void Die(); // Used for begining the "Bicho" dying process. Once dead animation is finished isDied() will return true.
	virtual void Logic(int *map);

	void AddWeapon(int id, cWeapon* weapon);
	cWeapon* GetWeapon(int id);
	bool GetWeapon(int id, cWeapon& weapon);
	void GetActiveWeapons(std::set<cWeapon*>& weapons);
	int  GetState();
	void SetState(int s);
	virtual bool isDead();

	void NextFrame(int max);
	int  GetFrame();
	void ResetFrame();

	void SetFramesToDie(int frames);
	int GetFramesToDie();
protected:
	void ActivateWeapon(cWeapon* weapon);

private:
	int x,y;
	int w,h;
	int state;
	
	int seq,delay;

	float life; // Current amount of life the "Bicho" has.
	int frames_to_die;

	float damage; // Damage that causes the "Bicho" on colision.

	std::map<int, cWeapon*> weapons; // Available weapons for "Bicho"
	std::set<cWeapon*> active_weapons; // Weapons currently active on screen
};
