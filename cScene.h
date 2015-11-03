#pragma once

#include "cTexture.h"
#include "Constants.h"

#define SCENE_Xo		0
#define SCENE_Yo		0
#define SCENE_WIDTH		16 // 16tiles
#define SCENE_HEIGHT	11 // 11tiles

#define FILENAME		"level"
#define FILENAME_EXT	".txt"

#define TILE_SIZE		16 // ==BLOCK_SIZE
#define BLOCK_SIZE		16 // 16 pixels

#define WORLD_TILE_MAP_SIZE 256

class cScene
{
public:
	cScene(void);
	virtual ~cScene(void);

	bool LoadLevel(int level);
	void Draw(int tex_id);
	bool InitTransition(int direction_transition);
	bool TransitionIsPosible(int direction_transition);
	bool TransitionFinished(int direction_transition, int transition_num);
	void UpdateMap(void);
	void DrawTransition(int direction_transition, int transition_num);
	void DrawTransitionLeft(int transition_num);
	void DrawTransitionRight(int transition_num);
	int GetNumForTransition(int direction_transition, int transition_num, int i, int j);
	int *GetMap();

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	int transition_list[6];					//The first position(0) will empty.

	int map_transition[SCENE_WIDTH * SCENE_HEIGHT];
	int id_TL;								//transition level
	int new_transition_list[6];					//The first position(0) will empty.
};
