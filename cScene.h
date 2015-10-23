#pragma once

#include "cTexture.h"

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
	int *GetMap();

private:
	int map[SCENE_WIDTH * SCENE_HEIGHT];	//scene
	int id_DL;								//actual level display list
};
