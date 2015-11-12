#include "cInterface.h"
#include "cScene.h"



cInterface::cInterface()
{
	x = 1;
	y = 0;
}

cInterface::~cInterface() {}

void cInterface::Process(float player_life, int player_hearts, int player_rupees, bool has_sword, bool has_arc, int transition_direction) {
	id_DL = glGenLists(1);
	glNewList(id_DL, GL_COMPILE);
	glBegin(GL_QUADS);

	glTexCoord2f(0.5, 0.5 + 0.03125f);				glVertex2i(0., SCENE_HEIGHT*TILE_SIZE);
	glTexCoord2f(0.5 + 0.03125f, 0.5 + 0.03125f);	glVertex2i(SCENE_WIDTH*TILE_SIZE, SCENE_HEIGHT*TILE_SIZE);
	glTexCoord2f(0.5 + 0.03125f, 0.5);				glVertex2i(SCENE_WIDTH*TILE_SIZE, GAME_HEIGHT);
	glTexCoord2f(0.5, 0.5);							glVertex2i(0., GAME_HEIGHT);

	UpdateMiniMap(transition_direction);
	DrawMiniMap();
	DrawRupees(player_rupees);
	DrawWeapons(has_sword, has_arc);
	DrawLife(player_life, player_hearts);

	glEnd();
	glEndList();
}


void cInterface::Draw(int tex_id)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glCallList(id_DL);
	glDisable(GL_TEXTURE_2D);
}

void cInterface::DrawMiniMap() {
	glTexCoord2f(0.5f, 0.5f + 0.5f);			glVertex2i(INTERFACE_Xo, INTERFACE_Yo);
	glTexCoord2f(0.5f + 0.5f, 0.5f + 0.5f);		glVertex2i(INTERFACE_Xo  + 4*TILE_SIZE, INTERFACE_Yo);
	glTexCoord2f(0.5f + 0.5f, 0.5f);			glVertex2i(INTERFACE_Xo + 4*TILE_SIZE, INTERFACE_Yo + 4*TILE_SIZE);
	glTexCoord2f(0.5f, 0.5f);					glVertex2i(INTERFACE_Xo, INTERFACE_Yo + 4 * TILE_SIZE);

	glTexCoord2f(0.f,			0.1875f + 0.125f);	glVertex2i(INTERFACE_Xo + TILE_SIZE*(x + 1),	INTERFACE_Yo + TILE_SIZE / 2 + TILE_SIZE*y);
	glTexCoord2f(0.f + 0.125f,	0.1875f + 0.125f);	glVertex2i(INTERFACE_Xo + TILE_SIZE*(x + 2),	INTERFACE_Yo + TILE_SIZE / 2 + TILE_SIZE*y);
	glTexCoord2f(0.f + 0.125f,	0.1875f);			glVertex2i(INTERFACE_Xo + TILE_SIZE*(x + 2),	INTERFACE_Yo + TILE_SIZE / 2 + TILE_SIZE*(y + 1));
	glTexCoord2f(0.f,			0.1875f);			glVertex2i(INTERFACE_Xo + TILE_SIZE*(x + 1),	INTERFACE_Yo + TILE_SIZE / 2 + TILE_SIZE*(y + 1));
}

void cInterface::DrawRupees(int player_rupees) {
	glTexCoord2f(0.f, 0.3125f + 0.125f);			glVertex2i(INTERFACE_Xo + TILE_SIZE*3, INTERFACE_Yo + TILE_SIZE*1);
	glTexCoord2f(0.f + 0.125f, 0.3125f + 0.125f);	glVertex2i(INTERFACE_Xo + TILE_SIZE*4, INTERFACE_Yo + TILE_SIZE*1);
	glTexCoord2f(0.f + 0.125f, 0.3125f);			glVertex2i(INTERFACE_Xo + TILE_SIZE*4, INTERFACE_Yo + TILE_SIZE*2);
	glTexCoord2f(0.f, 0.3125f);						glVertex2i(INTERFACE_Xo + TILE_SIZE*3, INTERFACE_Yo + TILE_SIZE*2);

	int num = player_rupees / 100;
	player_rupees -= 100 * num;
	if (num == 0)
		num = 10;

	glTexCoord2f(0.25f + (num%4)*0.0625f,		0.f + (num/4 + 1)*0.0625f);	glVertex2i(INTERFACE_Xo + TILE_SIZE * 4,				 INTERFACE_Yo + TILE_SIZE * 1);
	glTexCoord2f(0.25f + (num%4 + 1)*0.0625f,	0.f + (num/4 + 1)*0.0625f);	glVertex2i(INTERFACE_Xo + TILE_SIZE * 4 + TILE_SIZE / 2, INTERFACE_Yo + TILE_SIZE * 1);
	glTexCoord2f(0.25f + (num%4 + 1)*0.0625f,	0.f + (num/4)*0.0625f);		glVertex2i(INTERFACE_Xo + TILE_SIZE * 4 + TILE_SIZE / 2, INTERFACE_Yo + TILE_SIZE * 1 + TILE_SIZE / 2);
	glTexCoord2f(0.25f + (num%4)*0.0625f,		0.f + (num/4)*0.0625f);		glVertex2i(INTERFACE_Xo + TILE_SIZE * 4,				 INTERFACE_Yo + TILE_SIZE * 1 + TILE_SIZE / 2);

	num = player_rupees / 10;
	player_rupees -= 10 * num;

	glTexCoord2f(0.25f + (num % 4)*0.0625f,		0.f + (num / 4 + 1)*0.0625f);	glVertex2i(INTERFACE_Xo + TILE_SIZE * 4 + TILE_SIZE / 2,INTERFACE_Yo + TILE_SIZE * 1);
	glTexCoord2f(0.25f + (num % 4 + 1)*0.0625f, 0.f + (num / 4 + 1)*0.0625f);	glVertex2i(INTERFACE_Xo + TILE_SIZE * 5,				INTERFACE_Yo + TILE_SIZE * 1);
	glTexCoord2f(0.25f + (num % 4 + 1)*0.0625f, 0.f + (num / 4)*0.0625f);		glVertex2i(INTERFACE_Xo + TILE_SIZE * 5,				INTERFACE_Yo + TILE_SIZE * 1 + TILE_SIZE / 2);
	glTexCoord2f(0.25f + (num % 4)*0.0625f,		0.f + (num / 4)*0.0625f);		glVertex2i(INTERFACE_Xo + TILE_SIZE * 4 + TILE_SIZE / 2,INTERFACE_Yo + TILE_SIZE * 1 + TILE_SIZE / 2);

	num = player_rupees;

	glTexCoord2f(0.25f + (num % 4)*0.0625f,		0.f + (num / 4 + 1)*0.0625f);	glVertex2i(INTERFACE_Xo + TILE_SIZE * 5,				INTERFACE_Yo + TILE_SIZE * 1);
	glTexCoord2f(0.25f + (num % 4 + 1)*0.0625f, 0.f + (num / 4 + 1)*0.0625f);	glVertex2i(INTERFACE_Xo + TILE_SIZE * 5 + TILE_SIZE / 2,INTERFACE_Yo + TILE_SIZE * 1);
	glTexCoord2f(0.25f + (num % 4 + 1)*0.0625f, 0.f + (num / 4)*0.0625f);		glVertex2i(INTERFACE_Xo + TILE_SIZE * 5 + TILE_SIZE / 2,INTERFACE_Yo + TILE_SIZE * 1 + TILE_SIZE / 2);
	glTexCoord2f(0.25f + (num % 4)*0.0625f,		0.f + (num / 4)*0.0625f);		glVertex2i(INTERFACE_Xo + TILE_SIZE * 5,				INTERFACE_Yo + TILE_SIZE * 1 + TILE_SIZE / 2);
}

void cInterface::DrawWeapons(bool has_sword, bool has_arc) {
	glTexCoord2f(0.f, 0.5f + 0.5f);			glVertex2i(INTERFACE_Xo + 6 * TILE_SIZE, INTERFACE_Yo);
	glTexCoord2f(0.f + 0.5f, 0.5f + 0.5f);	glVertex2i(INTERFACE_Xo + 10 * TILE_SIZE, INTERFACE_Yo);
	glTexCoord2f(0.f + 0.5f, 0.5f);			glVertex2i(INTERFACE_Xo + 10 * TILE_SIZE, INTERFACE_Yo + 4 * TILE_SIZE);
	glTexCoord2f(0.f, 0.5f);				glVertex2i(INTERFACE_Xo + 6 * TILE_SIZE, INTERFACE_Yo + 4 * TILE_SIZE);
	if (has_arc) {
		glTexCoord2f(0.125f, 0.0625f + 0.125f);			glVertex2i(INTERFACE_Xo + 6 * TILE_SIZE + 12,		INTERFACE_Yo + TILE_SIZE);
		glTexCoord2f(0.125f + 0.125f, 0.0625f + 0.125f);	glVertex2i(INTERFACE_Xo + 7 * TILE_SIZE + 12,	INTERFACE_Yo + TILE_SIZE);
		glTexCoord2f(0.125f + 0.125f, 0.0625f);			glVertex2i(INTERFACE_Xo + 7 * TILE_SIZE + 12,		INTERFACE_Yo + 2*TILE_SIZE);
		glTexCoord2f(0.125f, 0.0625f);				glVertex2i(INTERFACE_Xo + 6 * TILE_SIZE + 12,			INTERFACE_Yo + 2*TILE_SIZE);
	}
	if (has_sword) {
		glTexCoord2f(0.f, 0.0625f + 0.125f);			glVertex2i(INTERFACE_Xo + 8 * TILE_SIZE + 4, INTERFACE_Yo + TILE_SIZE);
		glTexCoord2f(0.f + 0.125f, 0.0625f + 0.125f);	glVertex2i(INTERFACE_Xo + 9 * TILE_SIZE + 4, INTERFACE_Yo + TILE_SIZE);
		glTexCoord2f(0.f + 0.125f, 0.0625f);			glVertex2i(INTERFACE_Xo + 9 * TILE_SIZE + 4, INTERFACE_Yo + 2 * TILE_SIZE);
		glTexCoord2f(0.f, 0.0625f);						glVertex2i(INTERFACE_Xo + 8 * TILE_SIZE + 4, INTERFACE_Yo + 2 * TILE_SIZE);
	}
}

void cInterface::DrawLife(float player_life, int player_hearts) {
	float coordx_tile, coordy_tile;
	int px, py;

	glTexCoord2f(0.5f, 0.f + 0.5f);			glVertex2i(INTERFACE_Xo + 10 * TILE_SIZE, INTERFACE_Yo);
	glTexCoord2f(0.5f + 0.5f, 0.f + 0.5f);	glVertex2i(INTERFACE_Xo + 14 * TILE_SIZE, INTERFACE_Yo);
	glTexCoord2f(0.5f + 0.5f, 0.f);			glVertex2i(INTERFACE_Xo + 14 * TILE_SIZE, INTERFACE_Yo + 4 * TILE_SIZE);
	glTexCoord2f(0.5f, 0.f);				glVertex2i(INTERFACE_Xo + 10 * TILE_SIZE, INTERFACE_Yo + 4 * TILE_SIZE);

	for (int i = 0; i < player_hearts; i++) {
		px = INTERFACE_Xo + TILE_SIZE * 10 + HEART_SIZE*(i%10);
		py = INTERFACE_Yo + TILE_SIZE - HEART_SIZE*(i/10);

		if (player_life >= 1.) {
			coordx_tile = 0;
			coordy_tile = 0;
			player_life--;
		}
		else if (player_life == 0.5) {
			coordx_tile = 0.0625f;
			coordy_tile = 0;
			player_life = 0;
		}
		else {
			coordx_tile = 0.125f;
			coordy_tile = 0;
		}

		glTexCoord2f(coordx_tile, coordy_tile + 0.0625f);			glVertex2i(px, py);
		glTexCoord2f(coordx_tile + 0.0625f, coordy_tile + 0.0625f);	glVertex2i(px + HEART_SIZE, py);
		glTexCoord2f(coordx_tile + 0.0625f, coordy_tile);			glVertex2i(px + HEART_SIZE, py + HEART_SIZE);
		glTexCoord2f(coordx_tile, coordy_tile);						glVertex2i(px, py + HEART_SIZE);
	}
}

void cInterface::UpdateMiniMap(int transition_direction) {
	if (transition_direction != 0) {
		switch (transition_direction)
		{
		case TRANSITION_BOTTOM:
			--y;
			break;
		case TRANSITION_TOP:
			++y;
			break;
		case TRANSITION_LEFT:
			--x;
			break;
		case TRANSITION_RIGHT:
			++x;
			break;
		}
	}
}