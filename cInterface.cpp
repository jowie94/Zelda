#include "cInterface.h"



cInterface::cInterface()
{
}

cInterface::~cInterface() {}

void cInterface::Process(float player_life, int player_hearts, int player_rupies, bool has_sword, bool has_arc, int transition_direction) {
	id_DL = glGenLists(1);
	glNewList(id_DL, GL_COMPILE);
	glBegin(GL_QUADS);

	float coordx_tile, coordy_tile;
	int px, py;

	px = INTERFACE_Xo;
	py = INTERFACE_Yo;
	
	for (int i = 0; i < player_hearts; i++) {
		if (player_life >= 1.) {
			coordx_tile = 0;
			coordy_tile = 0;
			player_life--;
		}
		else if (player_life == 0.5) {
			coordx_tile = 0.25f;
			coordy_tile = 0;
			player_life = 0;
		}
		else {
			coordx_tile = 0.5f;
			coordy_tile = 0;
		}

		glTexCoord2f(coordx_tile,		   coordy_tile + 0.250f);	glVertex2i(px,				py);
		glTexCoord2f(coordx_tile + 0.250f, coordy_tile + 0.250f);	glVertex2i(px + HEART_SIZE, py);
		glTexCoord2f(coordx_tile + 0.250f, coordy_tile);			glVertex2i(px + HEART_SIZE, py + HEART_SIZE);
		glTexCoord2f(coordx_tile,		   coordy_tile);			glVertex2i(px,				py + HEART_SIZE);
		
		px += HEART_SIZE;
	}

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

