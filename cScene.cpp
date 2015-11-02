#include "cScene.h"
#include "Globals.h"

cScene::cScene(void)
{
}

cScene::~cScene(void)
{
}

bool cScene::LoadLevel(int level)
{
	bool res;
	FILE *fd;
	char file[16];
	int i,j,px,py;
	char tile1, tile2;
	float coordx_tile, coordy_tile;

	res=true;

	if(level<10) sprintf(file,"%s0%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);
	else		 sprintf(file,"%s%d%s",(char *)FILENAME,level,(char *)FILENAME_EXT);

	fd=fopen(file,"r");
	if(fd==NULL) return false;

	id_DL=glGenLists(1);
	glNewList(id_DL,GL_COMPILE);
		glBegin(GL_QUADS);
	
			for(j=SCENE_HEIGHT-1;j>=0;j--)
			{
				px=SCENE_Xo;
				py=SCENE_Yo+(j*TILE_SIZE);

				for(i=0;i<SCENE_WIDTH;i++)
				{
					fscanf(fd,"%c%c",&tile1, &tile2);

					int num = (tile1 - '0') * 10 + (tile2 - '0');
					
					map[(j*SCENE_WIDTH)+i] = (tile1-'0')*10 + (tile2 - '0');

					int columna = (num-1)%12;
					int fila = (num-1)/12;

					int num_pixel_x = columna*TILE_SIZE;
					int num_pixel_y = fila*TILE_SIZE;

					coordx_tile = float(num_pixel_x) / WORLD_TILE_MAP_SIZE;
					coordy_tile = float(num_pixel_y) / WORLD_TILE_MAP_SIZE;
					
					//BLOCK_SIZE = 16, FILE_SIZE = 256
					// 16 / 256 = 0.0625
					glTexCoord2f(coordx_tile		,coordy_tile+0.0625f);	glVertex2i(px           ,py           );
					glTexCoord2f(coordx_tile+0.0625f,coordy_tile+0.0625f);	glVertex2i(px+BLOCK_SIZE,py           );
					glTexCoord2f(coordx_tile+0.0625f,coordy_tile        );	glVertex2i(px+BLOCK_SIZE,py+BLOCK_SIZE);
					glTexCoord2f(coordx_tile        ,coordy_tile        );	glVertex2i(px           ,py+BLOCK_SIZE);
				
					px+=TILE_SIZE;
				}
				fscanf(fd,"%c",&tile1); //pass enter
			}
			for (int i = 1; i < 6; ++i) {
				char num1, num2;
				fscanf(fd, "%c%c", &num1, &num2);

				int num = (num1 - '0') * 10 + (num2 - '0');

				transition_list[i] = num;

				fscanf(fd, "%c", &tile1); //pass enter
			}
		glEnd();
	glEndList();

	fclose(fd);

	return res;
}

void cScene::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}


bool cScene::TransitionIsPosible(int direction_transition) {
	int level = transition_list[direction_transition];
	if (level == 0) {
		return false;
	}
	return true;
}

bool cScene::TransitionFinished(bool vertical, int transition_num) {
	if (vertical && transition_num == SCENE_HEIGHT)
		return true;
	else if(!vertical && transition_num == SCENE_WIDTH)
		return true;
	else
		return false;
}

void cScene::UpdateMap(void) {
	std::copy(std::begin(map_transition), std::end(map_transition), std::begin(map));
	std::copy(std::begin(new_transition_list), std::end(new_transition_list), std::begin(transition_list));
}

bool cScene::InitTransition(int direction_transition) {
	char tile1, tile2;
	int level = transition_list[direction_transition];
	char file[16];
	FILE *fd;

	if (level<10) sprintf(file, "%s0%d%s", (char *)FILENAME, level, (char *)FILENAME_EXT);
	else		  sprintf(file, "%s%d%s", (char *)FILENAME, level, (char *)FILENAME_EXT);

	char str[128];
	sprintf(str, "FILE = %s \n", file);
	OutputDebugString(str);

	fd = fopen(file, "r");
	if (fd == NULL){
		OutputDebugString("fd is NULL \n");
		return false;
	}
		


	for (int j = SCENE_HEIGHT - 1; j >= 0; j--)
	{
		
		for (int i = 0; i < SCENE_WIDTH; i++)
		{
			fscanf(fd, "%c%c", &tile1, &tile2);
			int num = (tile1 - '0') * 10 + (tile2 - '0');
			map_transition[(j*SCENE_WIDTH) + i] = (tile1 - '0') * 10 + (tile2 - '0');
		}

		fscanf(fd, "%c", &tile1); //pass enter
	}

	for (int i = 1; i < 6; ++i) {
		char num1, num2;
		fscanf(fd, "%c%c", &num1, &num2);

		int num = (num1 - '0') * 10 + (num2 - '0');

		new_transition_list[i] = num;

		fscanf(fd, "%c", &tile1); //pass enter
	}
	fclose(fd);
	return true;
}

void cScene::DrawTransition(int direction_transition, int transition_num) {
	int px, py;
	float coordx_tile, coordy_tile;
	
	id_DL = glGenLists(1);
	glNewList(id_DL, GL_COMPILE);
	glBegin(GL_QUADS);

	for (int j = SCENE_HEIGHT - 1;j >= 0;j--)
	{
		px = SCENE_Xo;
		py = SCENE_Yo + (j*TILE_SIZE);

		for (int i = 0;i<SCENE_WIDTH;i++)
		{

			int num = map_transition[(j*SCENE_WIDTH) + i];

			int columna = (num - 1) % 12;
			int fila = (num - 1) / 12;

			int num_pixel_x = columna*TILE_SIZE;
			int num_pixel_y = fila*TILE_SIZE;

			coordx_tile = float(num_pixel_x) / WORLD_TILE_MAP_SIZE;
			coordy_tile = float(num_pixel_y) / WORLD_TILE_MAP_SIZE;

			//BLOCK_SIZE = 16, FILE_SIZE = 256
			// 16 / 256 = 0.0625
			glTexCoord2f(coordx_tile, coordy_tile + 0.0625f);	glVertex2i(px, py);
			glTexCoord2f(coordx_tile + 0.0625f, coordy_tile + 0.0625f);	glVertex2i(px + BLOCK_SIZE, py);
			glTexCoord2f(coordx_tile + 0.0625f, coordy_tile);	glVertex2i(px + BLOCK_SIZE, py + BLOCK_SIZE);
			glTexCoord2f(coordx_tile, coordy_tile);	glVertex2i(px, py + BLOCK_SIZE);

			px += TILE_SIZE;
		}
	}
	glEnd();
	glEndList();
}

int* cScene::GetMap()
{
	return map;
}