#pragma once

#include "cScene.h"
#include "cPlayer.h"
#include "cInterface.h"
#include "cData.h"
#include "cEnemy.h"

#define GAME_WIDTH	640
#define GAME_HEIGHT 568

#define STATE_PLAYING	 2

class cGame
{
public:
	cGame(void);
	virtual ~cGame(void);

	bool Init();
	bool Loop();
	void Finalize();

	//Input
	void ReadKeyboard(unsigned char key, int x, int y, bool press);
	void ReadMouse(int button, int state, int x, int y);
	//Process
	bool Process();
	//Output
	void Render();

	bool StartTransition();
private:
	unsigned char keys[256];
	cScene Scene;
	cPlayer Player;
	cData Data;
	cInterface Interface;
	std::list<cEnemy*> enemies;
	int state = STATE_PLAYING;
	int transition_num;
	int direction_transition;

	void LoadEnemies(const fMatrix& mEnemies);
};
