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
			for (int i = 1; i < 7; ++i) {
				char num1, num2;
				fscanf(fd, "%c%c", &num1, &num2);

				int num = (num1 - '0') * 10 + (num2 - '0');

				transition_list[i] = num;
				if (i == TRANSITION_OUTSIDE && num > 0) {
					fscanf(fd, "%c", &num1); //pass space

					fscanf(fd, "%c%c", &num1, &num2);
					outside_pos[0] = (num1 - '0') * 10 + (num2 - '0'); // read x coord outside pos

					fscanf(fd, "%c", &num1); //pass space

					fscanf(fd, "%c%c", &num1, &num2);
					outside_pos[1] = (num1 - '0') * 10 + (num2 - '0'); // read y coord outside pos
				}

				fscanf(fd, "%c", &tile1); //pass enter
			}
		glEnd();
	glEndList();

	if (!feof(fd))
		ReadEnemies(fd);

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
		if (direction_transition == TRANSITION_BOTTOM && transition_list[TRANSITION_OUTSIDE] != 0)
			return true;
		return false;
	}
	return true;
}


int* cScene::GetTransitionOutsidePos() {
	return outside_pos;
}


bool cScene::TransitionFinished(int direction_transition, int transition_num) {
	if (transition_num == SCENE_HEIGHT*2)
		if (direction_transition == TRANSITION_BOTTOM || direction_transition == TRANSITION_TOP)
			return true;
	
	if (transition_num == SCENE_WIDTH*2)
		if (direction_transition == TRANSITION_RIGHT || direction_transition == TRANSITION_LEFT)
			return true;
	
	if (transition_num == TILE_SIZE) 
		if (direction_transition == TRANSITION_INSIDE || direction_transition == TRANSITION_OUTSIDE)
			return true;
	
	return false;
}

void cScene::UpdateMap(void) {
	std::copy(std::begin(map_transition), std::end(map_transition), std::begin(map));
	std::copy(std::begin(new_transition_list), std::end(new_transition_list), std::begin(transition_list));
	std::copy(std::begin(transition_outside_pos), std::end(transition_outside_pos), std::begin(outside_pos));
}

bool cScene::InitTransition(int direction_transition) {
	char tile1, tile2;
	int level = transition_list[direction_transition];
	char file[16];
	FILE *fd;

	if (level<10) sprintf(file, "%s0%d%s", (char *)FILENAME, level, (char *)FILENAME_EXT);
	else		  sprintf(file, "%s%d%s", (char *)FILENAME, level, (char *)FILENAME_EXT);

	fd = fopen(file, "r");
	if (fd == NULL){
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

	for (int i = 1; i < 7; ++i) {
		char num1, num2;
		fscanf(fd, "%c%c", &num1, &num2);

		int num = (num1 - '0') * 10 + (num2 - '0');

		new_transition_list[i] = num;

		if (i == TRANSITION_OUTSIDE && num > 0) {
			fscanf(fd, "%c", &num1); //pass space

			fscanf(fd, "%c%c", &num1, &num2);
			transition_outside_pos[0] = (num1 - '0') * 10 + (num2 - '0'); // read x coord outside pos

			fscanf(fd, "%c", &num1); //pass space

			fscanf(fd, "%c%c", &num1, &num2);
			transition_outside_pos[1] = (num1 - '0') * 10 + (num2 - '0'); // read y coord outside pos
		}

		fscanf(fd, "%c", &tile1); //pass enter


	}
	
	enemies.clear();
	if (!feof(fd))
		ReadEnemies(fd);

	fclose(fd);
	return true;
}

void cScene::DrawTransition(int direction_transition, int transition_num, bool dungeon) {
	if (direction_transition == TRANSITION_INSIDE || direction_transition == TRANSITION_OUTSIDE) {
		if (direction_transition == TRANSITION_INSIDE && transition_num == TILE_SIZE)
			DrawTransitionDungeon(direction_transition, transition_num);
		else if (direction_transition == TRANSITION_OUTSIDE && transition_num == 1)
			DrawTransitionEdge(direction_transition, transition_num);
	}
	else {
		if(dungeon)
			DrawTransitionDungeon(direction_transition, transition_num);
		else
			DrawTransitionEdge(direction_transition, transition_num);
	}
}

void cScene::DrawTransitionEdge(int direction_transition, int transition_num) {
	int offsetx, offsety;
	offsetx = offsety = 0;
	if (transition_num % 2 != 0) {
		transition_num -= 1;
		switch (direction_transition)
		{
		case TRANSITION_BOTTOM:
			offsety = TILE_SIZE / 2;
			break;
		case TRANSITION_TOP:
			offsety = -TILE_SIZE / 2;
			break;
		case TRANSITION_RIGHT:
			offsetx = -TILE_SIZE / 2;
			break;
		case TRANSITION_LEFT:
			offsetx = TILE_SIZE / 2;
			break;
		}
	}
	transition_num /= 2;
	char str[128];
	int px, py;
	float coordx_tile, coordy_tile;

	id_DL = glGenLists(1);
	glNewList(id_DL, GL_COMPILE);
	glBegin(GL_QUADS);

	for (int j = SCENE_HEIGHT - 1;j >= 0;j--)
	{
		px = SCENE_Xo;
		py = SCENE_Yo + (j*TILE_SIZE);

		for (int i = 0;i < SCENE_WIDTH;i++)
		{

			int num = GetNumForTransition(direction_transition, transition_num, i, j);

			int columna = (num - 1) % 12;
			int fila = (num - 1) / 12;

			int num_pixel_x = columna*TILE_SIZE;
			int num_pixel_y = fila*TILE_SIZE;

			coordx_tile = float(num_pixel_x) / WORLD_TILE_MAP_SIZE;
			coordy_tile = float(num_pixel_y) / WORLD_TILE_MAP_SIZE;

			coordx_tile += offsetx;
			coordy_tile += offsety;

			//BLOCK_SIZE = 16, FILE_SIZE = 256
			// 16 / 256 = 0.0625
			glTexCoord2f(coordx_tile, coordy_tile + 0.0625f);			glVertex2i(px, py);
			glTexCoord2f(coordx_tile + 0.0625f, coordy_tile + 0.0625f);	glVertex2i(px + BLOCK_SIZE, py);
			glTexCoord2f(coordx_tile + 0.0625f, coordy_tile);			glVertex2i(px + BLOCK_SIZE, py + BLOCK_SIZE);
			glTexCoord2f(coordx_tile, coordy_tile);						glVertex2i(px, py + BLOCK_SIZE);

			px += TILE_SIZE;
		}

	}
	glEnd();
	glEndList();
}

void cScene::DrawTransitionDungeon(int direction_transition, int transition_num) {
	int offsetx, offsety, old_scene_posx, old_scene_posy, new_scene_posx, new_scene_posy;
	offsetx = offsety = old_scene_posx = old_scene_posy = new_scene_posx = new_scene_posy = 0;
	switch (direction_transition)
	{
	case TRANSITION_BOTTOM:
		if (transition_num % 2 != 0)
			offsety = TILE_SIZE / 2;
		old_scene_posy = SCENE_Yo + transition_num*TILE_SIZE / 2;
		new_scene_posy = -SCENE_HEIGHT*TILE_SIZE + transition_num*TILE_SIZE / 2;
		break;
	case TRANSITION_TOP:
		if (transition_num % 2 != 0)
			offsety = -TILE_SIZE / 2;
		old_scene_posy = SCENE_Yo - transition_num*TILE_SIZE / 2;
		new_scene_posy = SCENE_HEIGHT*TILE_SIZE - transition_num*TILE_SIZE / 2;
		break;
	case TRANSITION_RIGHT:
		if (transition_num % 2 != 0)
			offsetx = -TILE_SIZE / 2;
		old_scene_posx = SCENE_Xo - transition_num*TILE_SIZE / 2;
		new_scene_posx = SCENE_WIDTH*TILE_SIZE - transition_num*TILE_SIZE / 2;
		break;
	case TRANSITION_LEFT:
		if (transition_num % 2 != 0)
			offsetx = TILE_SIZE / 2;
		old_scene_posx = SCENE_Xo + transition_num*TILE_SIZE / 2;
		new_scene_posx = -SCENE_WIDTH*TILE_SIZE + transition_num*TILE_SIZE / 2;
		break;
	}
	transition_num /= 2;
	char str[128];
	int px, py;
	float coordx_tile, coordy_tile;

	id_DL = glGenLists(1);
	glNewList(id_DL, GL_COMPILE);
	glBegin(GL_QUADS);

	//Printed old map
	DrawDungeonMap(old_scene_posx, old_scene_posy, transition_list);

	//Printed new map
	DrawDungeonMap(new_scene_posx, new_scene_posy, new_transition_list);

	//if (new_transition_list[TRANSITION_BOTTOM] != 0 || new_transition_list[TRANSITION_OUTSIDE] != 0) {
	//	glTexCoord2f(0.5F, 0.125f);		glVertex2i(SCENE_Xo,							SCENE_Yo);
	//	glTexCoord2f(1.0f, 0.125f);		glVertex2i(SCENE_Xo + BLOCK_SIZE*SCENE_WIDTH,	SCENE_Yo);
	//	glTexCoord2f(1.0f, 0.0625f);	glVertex2i(SCENE_Xo + BLOCK_SIZE*SCENE_WIDTH,	SCENE_Yo + BLOCK_SIZE*2);
	//	glTexCoord2f(0.5F, 0.0625f);	glVertex2i(SCENE_Xo,							SCENE_Yo + BLOCK_SIZE*2);
	//}

	//if (new_transition_list[TRANSITION_RIGHT] != 0) {
	//	glTexCoord2f(0.0625f,	0.84375f);	glVertex2i(BLOCK_SIZE*SCENE_WIDTH - BLOCK_SIZE*2,	SCENE_Yo);
	//	glTexCoord2f(0.125f,	0.84375f);	glVertex2i(BLOCK_SIZE*SCENE_WIDTH,					SCENE_Yo);
	//	glTexCoord2f(0.125f,	0.5f);		glVertex2i(BLOCK_SIZE*SCENE_WIDTH,					BLOCK_SIZE*SCENE_HEIGHT);
	//	glTexCoord2f(0.0625f,	0.5f);		glVertex2i(BLOCK_SIZE*SCENE_WIDTH - BLOCK_SIZE*2,	BLOCK_SIZE*SCENE_HEIGHT);
	//}
	//if (new_transition_list[TRANSITION_LEFT] != 0) {
	//	glTexCoord2f(0.f,		0.84375f);	glVertex2i(SCENE_Xo,				SCENE_Yo);
	//	glTexCoord2f(0.0625f,	0.84375f);	glVertex2i(SCENE_Xo + BLOCK_SIZE*2, SCENE_Yo);
	//	glTexCoord2f(0.0625f,	0.5f);		glVertex2i(SCENE_Xo + BLOCK_SIZE*2, BLOCK_SIZE*SCENE_HEIGHT);
	//	glTexCoord2f(0.f,		0.5f);		glVertex2i(SCENE_Xo,				BLOCK_SIZE*SCENE_HEIGHT);
	//}
	//if (transition_list[TRANSITION_TOP] != 0) {
	//	glTexCoord2f(0.5f, 0.0625f);	glVertex2i(old_scene_posx, old_scene_posy + BLOCK_SIZE*SCENE_HEIGHT - BLOCK_SIZE * 2);
	//	glTexCoord2f(1.0f, 0.0625f);	glVertex2i(old_scene_posx + BLOCK_SIZE*SCENE_WIDTH, old_scene_posy + BLOCK_SIZE*SCENE_HEIGHT - BLOCK_SIZE * 2);
	//	glTexCoord2f(1.0f, 0.f);		glVertex2i(old_scene_posx + BLOCK_SIZE*SCENE_WIDTH, old_scene_posy + BLOCK_SIZE*SCENE_HEIGHT);
	//	glTexCoord2f(0.5f, 0.f);		glVertex2i(old_scene_posx, old_scene_posy + BLOCK_SIZE*SCENE_HEIGHT);
	//}
	//if (new_transition_list[TRANSITION_TOP] != 0) {
	//	glTexCoord2f(0.5f, 0.0625f);	glVertex2i(new_scene_posx, new_scene_posy + BLOCK_SIZE*SCENE_HEIGHT - BLOCK_SIZE * 2);
	//	glTexCoord2f(1.0f, 0.0625f);	glVertex2i(new_scene_posx + BLOCK_SIZE*SCENE_WIDTH, new_scene_posy + BLOCK_SIZE*SCENE_HEIGHT - BLOCK_SIZE * 2);
	//	glTexCoord2f(1.0f, 0.f);		glVertex2i(new_scene_posx + BLOCK_SIZE*SCENE_WIDTH, new_scene_posy + BLOCK_SIZE*SCENE_HEIGHT);
	//	glTexCoord2f(0.5f, 0.f);		glVertex2i(new_scene_posx, new_scene_posy + BLOCK_SIZE*SCENE_HEIGHT);
	//}
	//if (new_transition_list[TRANSITION_TOP] != 0) {
	//	glTexCoord2f(0.5f, 0.0625f);	glVertex2i(SCENE_Xo, BLOCK_SIZE*SCENE_HEIGHT - BLOCK_SIZE * 2);
	//	glTexCoord2f(1.0f, 0.0625f);	glVertex2i(SCENE_Xo + BLOCK_SIZE*SCENE_WIDTH, BLOCK_SIZE*SCENE_HEIGHT - BLOCK_SIZE * 2);
	//	glTexCoord2f(1.0f, 0.f);		glVertex2i(SCENE_Xo + BLOCK_SIZE*SCENE_WIDTH, BLOCK_SIZE*SCENE_HEIGHT);
	//	glTexCoord2f(0.5f, 0.f);		glVertex2i(SCENE_Xo, BLOCK_SIZE*SCENE_HEIGHT);
	//}

	for (int j = SCENE_HEIGHT - 1;j >= 0;j--)
	{
		px = SCENE_Xo;
		py = SCENE_Yo + (j*TILE_SIZE);

		for (int i = 0;i < SCENE_WIDTH;i++)
		{

			int num = GetNumForTransition(direction_transition, transition_num, i, j);

			if (num != 0 && num != 9) {
				int columna = (num - 1) % 4;
				int fila = (num - 1) / 4;

				int num_pixel_x = columna*TILE_SIZE;
				int num_pixel_y = fila*TILE_SIZE;

				coordx_tile = float(num_pixel_x) / DUNGEON_TILE_MAP_SIZE;
				coordy_tile = float(num_pixel_y) / DUNGEON_TILE_MAP_SIZE;

				coordx_tile += offsetx + 0.5;
				coordy_tile += offsety + 0.5;

				//BLOCK_SIZE = 16, FILE_SIZE = 256
				// 16 / 256 = 0.0625
				glTexCoord2f(coordx_tile, coordy_tile + 0.03125f);			glVertex2i(px, py);
				glTexCoord2f(coordx_tile + 0.03125f, coordy_tile + 0.03125f);	glVertex2i(px + BLOCK_SIZE, py);
				glTexCoord2f(coordx_tile + 0.03125f, coordy_tile);			glVertex2i(px + BLOCK_SIZE, py + BLOCK_SIZE);
				glTexCoord2f(coordx_tile, coordy_tile);						glVertex2i(px, py + BLOCK_SIZE);
			}
			px += TILE_SIZE;
		}

	}
	glEnd();
	glEndList();
}

void cScene::DrawDungeonMap(int pos_x, int pos_y, int *trans_list) {

	//Draw map
	glTexCoord2f(0., 0. + 0.34375f);
	glVertex2i(pos_x, pos_y);
	glTexCoord2f(0. + 0.5f, 0. + 0.34375f);
	glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH, pos_y);
	glTexCoord2f(0. + 0.5f, 0.);
	glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH, pos_y + BLOCK_SIZE*SCENE_HEIGHT);
	glTexCoord2f(0., 0.);
	glVertex2i(pos_x, pos_y + BLOCK_SIZE*SCENE_HEIGHT);

	if (trans_list[TRANSITION_TOP] != 0) {
		glTexCoord2f(0.5f, 0.0625f);	glVertex2i(pos_x, pos_y + BLOCK_SIZE*SCENE_HEIGHT - BLOCK_SIZE * 2);
		glTexCoord2f(1.0f, 0.0625f);	glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH, pos_y + BLOCK_SIZE*SCENE_HEIGHT - BLOCK_SIZE * 2);
		glTexCoord2f(1.0f, 0.f);		glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH, pos_y + BLOCK_SIZE*SCENE_HEIGHT);
		glTexCoord2f(0.5f, 0.f);		glVertex2i(pos_x, pos_y + BLOCK_SIZE*SCENE_HEIGHT);
	}
	if (trans_list[TRANSITION_BOTTOM] != 0 || trans_list[TRANSITION_OUTSIDE] != 0) {
		glTexCoord2f(0.5F, 0.125f);		glVertex2i(pos_x, pos_y);
		glTexCoord2f(1.0f, 0.125f);		glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH, pos_y);
		glTexCoord2f(1.0f, 0.0625f);	glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH, pos_y + BLOCK_SIZE * 2);
		glTexCoord2f(0.5F, 0.0625f);	glVertex2i(pos_x, pos_y + BLOCK_SIZE * 2);
	}
	if (trans_list[TRANSITION_RIGHT] != 0) {
		glTexCoord2f(0.0625f, 0.84375f);	glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH - BLOCK_SIZE * 2, pos_y);
		glTexCoord2f(0.125f, 0.84375f);	glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH, pos_y);
		glTexCoord2f(0.125f, 0.5f);		glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH, pos_y + BLOCK_SIZE*SCENE_HEIGHT);
		glTexCoord2f(0.0625f, 0.5f);		glVertex2i(pos_x + BLOCK_SIZE*SCENE_WIDTH - BLOCK_SIZE * 2, pos_y + BLOCK_SIZE*SCENE_HEIGHT);
	}
	if (trans_list[TRANSITION_LEFT] != 0) {
		glTexCoord2f(0.f, 0.84375f);	glVertex2i(pos_x, pos_y);
		glTexCoord2f(0.0625f, 0.84375f);	glVertex2i(pos_x + BLOCK_SIZE * 2, pos_y);
		glTexCoord2f(0.0625f, 0.5f);		glVertex2i(pos_x + BLOCK_SIZE * 2, pos_y + BLOCK_SIZE*SCENE_HEIGHT);
		glTexCoord2f(0.f, 0.5f);		glVertex2i(pos_x, pos_y + BLOCK_SIZE*SCENE_HEIGHT);
	}
}


int cScene::GetNumForTransition(int direction_transition, int transition_num, int i, int j) {
	int num = 0;
	int lim_i = SCENE_WIDTH - transition_num;
	int lim_j = SCENE_HEIGHT - transition_num;
	switch (direction_transition)
	{
	case TRANSITION_BOTTOM:
		if (j < transition_num)
			num = map_transition[((lim_j + j)*SCENE_WIDTH) + i];
		else
			num = map[((j - transition_num)*SCENE_WIDTH) + i];
		break;
	case TRANSITION_TOP:
		if (j < lim_j)
			num = map[((j + transition_num)*SCENE_WIDTH) + i];
		else
			num = map_transition[((j - lim_j)*SCENE_WIDTH) + i];
		break;
	case TRANSITION_RIGHT:
		if (i < lim_i)
			num = map[(j*SCENE_WIDTH) + i + transition_num];
		else
			num = map_transition[(j*SCENE_WIDTH) + i - lim_i];
		break;
	case TRANSITION_LEFT:
		if (i < transition_num)
			num = map_transition[(j*SCENE_WIDTH) + lim_i + i];
		else
			num = map[(j*SCENE_WIDTH) + i - transition_num];
		break;
	default:
		num = map_transition[(j*SCENE_WIDTH) + i];
		break;
	}
	return num;
}

int* cScene::GetMap()
{
	return map;
}

void cScene::GetEnemies(fMatrix& enemies) const
{
	enemies = this->enemies;
}

void cScene::ReadEnemies(FILE* fd)
{
	enemies.clear();
	char c;
	fscanf(fd, "%c", &c);
	if (c == '-')
	{
		fscanf(fd, "%c", &c); // Ignore enter
		fscanf(fd, "%c", &c); // Get enemy ID or '-'
		while (c != '-')
		{
			float id, am, life, damage, posx, posy;
			char c2;

			id = c - '0';

			fscanf(fd, " "); // Ignore space
			fscanf(fd, "%c%c", &c, &c2); // Get amount of enemies
			am = (c - '0') * 10 + (c2 - '0');

			fscanf(fd, " "); // Ignore space
			fscanf(fd, "%c.%c", &c, &c2); // Get life of enemies
			life = (c - '0') + (c2 - '0') / 10.0f;

			fscanf(fd, " ");
			fscanf(fd, "%c.%c", &c, &c2); // Get damage of enemies
			damage = (c - '0') + (c2 - '0') / 10.0f;

			fscanf(fd, " ");
			fscanf(fd, "%c%c", &c, &c2); // Get pos x
			posx = (c - '0') * 10 + (c2 - '0');

			fscanf(fd, " ");
			fscanf(fd, "%c%c", &c, &c2); // Get pos y
			posy = (c - '0') * 10 + (c2 - '0');

			enemies.push_back({ id, am, life, damage, posx, posy });

			fscanf(fd, "\n");
			fscanf(fd, "%c", &c);
		}
	}
}
