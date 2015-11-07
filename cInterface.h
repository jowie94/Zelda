#pragma once

#include "cTexture.h"

#define INTERFACE_Xo	0
#define INTERFACE_Yo	180
#define HEART_SIZE		8

class cInterface
{

public:
	cInterface();
	cInterface(int hearts);
	~cInterface();

	void SetHearts(int hearts);
	int GetHearts();

	void Process(float player_life);
	void Draw(int tex_id);

private:
	int id_DL;								//actual level display list
	float life;

	int hearts;

};

