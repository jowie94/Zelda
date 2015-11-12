#pragma once

#include "cTexture.h"

#define INTERFACE_Xo	0
#define INTERFACE_Yo	176
#define HEART_SIZE		8
#define GAME_HEIGHT		568

class cInterface
{

public:
	cInterface();
	~cInterface();

	void Process(float player_life, int player_hearts, int player_rupees, bool has_sword, bool has_arc, int transition_direction);
	void Draw(int tex_id);

	void DrawMiniMap();
	void DrawWeapons(bool has_sword, bool has_arc);
	void DrawRupees(int player_rupees);
	void DrawLife(float player_life, int player_hearts);

	void UpdateMiniMap(int transition_direction);

private:
	int id_DL;								//actual level display list
	int x, y;
	//float life;
};

