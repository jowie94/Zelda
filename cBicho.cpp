#include "cBicho.h"
#include "cScene.h"
#include "cWeapon.h"

cBicho::cBicho(void) : cBicho(0) {}

cBicho::cBicho(float life)
{
	seq = 0;
	delay = 0;
	frames_to_die = 8; // Dummy value
	this->life = life;
}

cBicho::~cBicho(void){}

cBicho::cBicho(int posx,int posy,int width,int height)
{
	x = posx;
	y = posy;
	w = width;
	h = height;
}
void cBicho::SetPosition(int posx,int posy)
{
	x = posx;
	y = posy;
}
void cBicho::GetPosition(int *posx,int *posy)
{
	*posx = x;
	*posy = y;
}
void cBicho::SetTile(int tx,int ty)
{
	x = tx * TILE_SIZE;
	y = ty * TILE_SIZE;
}
void cBicho::GetTile(int *tx,int *ty)
{
	*tx = x / TILE_SIZE;
	*ty = y / TILE_SIZE;
}
void cBicho::SetWidthHeight(int width,int height)
{
	w = width;
	h = height;
}
void cBicho::GetWidthHeight(int *width,int *height)
{
	*width = w;
	*height = h;
}
bool cBicho::Collides(cRect *rc)
{
	return ((x>rc->left) && (x+w<rc->right) && (y>rc->bottom) && (y+h<rc->top));
}
bool cBicho::CollidesMapWall(int *map,bool right)
{
	int tile_x,tile_y;
	int j;
	int width_tiles,height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles  = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;

	if(right)	tile_x += width_tiles;
	
	for(j=0;j<height_tiles;j++)
	{
		if(map[ tile_x + ((tile_y+j)*SCENE_WIDTH) ] != 9)	return true;
	}
	
	return false;
}

bool cBicho::CollidesMapFloor(int *map, bool up)
{
	int tile_x,tile_y;
	int width_tiles, height_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;
	if( (x % TILE_SIZE) != 0) width_tiles++;

	if (up)
		tile_y += height_tiles;
	i=0;
	on_base = false;
	while((i<width_tiles) && !on_base)
	{
		if( (y % TILE_SIZE) == 0 )
		{
			int back_tile = map[(tile_x + i) + ((tile_y - 1) * SCENE_WIDTH)];
			int front_tile = map[(tile_x + i) + ((tile_y + 1) * SCENE_WIDTH)];
				if(back_tile != 9 && front_tile != 9)
				on_base = true;
		}
		else
		{
			if(map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ] != 9 && (y % TILE_SIZE) > 10)
			{
				on_base = true;
			}
		}
		i++;
	}
	return on_base;
}

void cBicho::GetArea(cRect *rc)
{
	rc->left   = x;
	rc->right  = x+w;
	rc->bottom = y;
	rc->top    = y+h;
}
void cBicho::DrawRect(int tex_id,float xo,float yo,float xf,float yf)
{
	int screen_x,screen_y;

	screen_x = x + SCENE_Xo;
	screen_y = y + SCENE_Yo + (BLOCK_SIZE - TILE_SIZE);

	glEnable(GL_TEXTURE_2D);
	
	glBindTexture(GL_TEXTURE_2D,tex_id);
	glBegin(GL_QUADS);	
		glTexCoord2f(xo,yo);	glVertex2i(screen_x  ,screen_y);
		glTexCoord2f(xf,yo);	glVertex2i(screen_x+w,screen_y);
		glTexCoord2f(xf,yf);	glVertex2i(screen_x+w,screen_y+h);
		glTexCoord2f(xo,yf);	glVertex2i(screen_x  ,screen_y+h);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void cBicho::SetLife(float life)
{
	this->life = life;
}

void cBicho::DecrementLife(float amount)
{
	this->life -= amount;
}

float cBicho::GetLife() const
{
	return this->life;
}

void cBicho::SetDamage(float damage)
{
	this->damage = damage;
}

float cBicho::GetDamage()
{
	return this->damage;
}

void cBicho::MoveLeft(int *map)
{
	int xaux = x;
	x -= STEP_LENGTH;
	state = STATE_WALKLEFT;
	if (CollidesMapWall(map, false))
		x = xaux;
}

void cBicho::MoveRight(int *map)
{
	int xaux = x;
	x += STEP_LENGTH;
	state = STATE_WALKRIGHT;
	if (CollidesMapWall(map, true))
		x = xaux;
}

void cBicho::MoveUp(int *map)
{
	int yaux = y;
	y += STEP_LENGTH;
	state = STATE_WALKUP;
	if (CollidesMapFloor(map, true))
		y = yaux;
}

void cBicho::MoveDown(int *map)
{
	int yaux = y;
	y -= STEP_LENGTH;
	state = STATE_WALKDOWN;
	if (CollidesMapFloor(map, false))
		y = yaux;
}

void cBicho::Stop()
{
	switch (state)
	{
	case STATE_WALKLEFT:	state = STATE_LOOKLEFT;		break;
	case STATE_WALKRIGHT:	state = STATE_LOOKRIGHT;	break;
	case STATE_WALKUP:		state = STATE_LOOKUP;		break;
	case STATE_WALKDOWN:	state = STATE_LOOKDOWN;		break;
	}
}

void cBicho::Die() {}

void cBicho::Logic(int *map)
{
	if (life <= 0 && !isDead())
		Die();
	else if (!isDead())
	{
		// Execute logic for all weapons
		for (auto it = active_weapons.begin(); it != active_weapons.end(); ++it)
		{
			cWeapon* w = *it;
			w->Logic(map);

			// If the weapon is "dead" remove from the active weapons list
			if (w->isDead()) {
				it = active_weapons.erase(it);
				if (it == active_weapons.end())
					break;
			}
		}
	} else
	{
		seq = 0;
		delay = 0;
	}
}

void cBicho::AddWeapon(int id, cWeapon* weapon)
{
	weapon->SetLife(0);
	weapons.insert_or_assign(id, weapon);
}

bool cBicho::GetWeapon(int id, cWeapon& weapon)
{
	auto it = weapons.find(id);
	if (it == weapons.end())
		return false;
	weapon = *it->second;
	return true;
}

void cBicho::GetActiveWeapons(std::list<cWeapon*>& weapons)
{
	weapons = this->active_weapons;
}

cWeapon* cBicho::GetWeapon(int id)
{
	auto it = weapons.find(id);
	if (it == weapons.end())
		return nullptr;
	return it->second;
}

void cBicho::NextFrame(int max)
{
	delay++;
	if(delay == FRAME_DELAY)
	{
		seq++;
		seq%=max;
		delay = 0;
	}
}
int cBicho::GetFrame()
{
	return seq;
}

void cBicho::SetFramesToDie(int frames)
{
	frames_to_die = frames;
}

int cBicho::GetFramesToDie()
{
	return frames_to_die;
}

void cBicho::ActivateWeapon(cWeapon* weapon)
{
	active_weapons.push_back(weapon);
}

int cBicho::GetState()
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}

bool cBicho::isDead()
{
	return (this->life <= 0) && (this->frames_to_die >= seq);
}
