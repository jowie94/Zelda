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

#define WORLD_TILE_MAP_SIZE		256
#define DUNGEON_TILE_MAP_SIZE	512

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
	void DrawTransition(int direction_transition, int transition_num, bool dungeon);
	void DrawTransitionEdge(int direction_transition, int transition_num);
	void DrawTransitionDungeon(int direction_transition, int transition_num);
	void DrawDungeonMap(int pos_x, int pos_y, int *trans_list);;
	int GetNumForTransition(int direction_transition, int transition_num, int i, int j);
	int *GetTransitionOutsidePos();
	int *GetMap();

	void GetEnemies(fMatrix& enemies) const;

private:
	void ReadEnemies(FILE* fd);

	fMatrix enemies;

	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
	int transition_list[7];					//The first position(0) will empty.
	int outside_pos[2];

	int map_transition[SCENE_WIDTH * SCENE_HEIGHT];
	int id_TL;								//transition level
	int new_transition_list[7];				//The first position(0) will empty.
	int transition_outside_pos[2];
};
