#pragma once

#include "cTexture.h"

#define INTERFACE_Xo	0
#define INTERFACE_Yo	180
#define HEART_SIZE		8
#define GAME_HEIGHT		568

class cInterface
{

public:
	cInterface();
	~cInterface();

	void Process(float player_life, int player_hearts, int player_rupies, bool has_sword, bool has_arc, int transition_direction);
	void Draw(int tex_id);

private:
	int id_DL;								//actual level display list
	//float life;
};

