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
int* cScene::GetMap()
{
	return map;
}