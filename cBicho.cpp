#include "cBicho.h"
#include "cScene.h"
#include "cWeapon.h"

cBicho::cBicho(void) : cBicho(0) {}

cBicho::cBicho(float life)
{
	seq = 0;
	delay = 0;
	frames_to_die = 8; // Dummy value
	hurt = false;
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
bool cBicho::Collides(const cRect * rc) const
{
	return (((y >= rc->bottom) && (y + h <= rc->top)) || ((y<rc->top) && (y + h>rc->top)) || ((y<rc->bottom) && (y + h>rc->bottom))) &&
		   (((x >= rc->left) && (x + w <= rc->right)) || ((x<rc->right) && (x + w>rc->right)) || ((x<rc->left) && (x + w>rc->left)));
}
bool cBicho::CollidesMapWall(int *map,bool right)
{
	/*char str[128];
	sprintf(str, "x = %d \n", x);
	OutputDebugString(str);
	sprintf(str, "y = %d \n", y);
	OutputDebugString(str);
	sprintf(str, "w = %d \n", w);
	OutputDebugString(str);
	sprintf(str, "h = %d \n", h);
	OutputDebugString(str);
*/
	int tile_x,tile_y;
	int i, j;
	bool on_base;
	int width_tiles,height_tiles;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;
	width_tiles  = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;
/*
	sprintf(str, "tile_x = %d \n", tile_x);
	OutputDebugString(str);
	sprintf(str, "tile_y = %d \n", tile_y);
	OutputDebugString(str);
	sprintf(str, "w_tiles = %d \n", width_tiles);
	OutputDebugString(str);
	sprintf(str, "h_tiles = %d \n", height_tiles);
	OutputDebugString(str);
	OutputDebugString("======================================== \n");
*/

	if(right && tile_x < SCENE_WIDTH - 1)
		tile_x += width_tiles;

	i = 0;
	on_base = (x < 0) || (tile_x >= SCENE_WIDTH);
	while ((i<height_tiles) && !on_base)
	{
		if ((x % TILE_SIZE) == 0)
		{
			int back_tile = map[(tile_x - 1) + ((tile_y + i) * SCENE_WIDTH)];
			int front_tile = map[(tile_x + 1) + ((tile_y + i) * SCENE_WIDTH)];
			if (back_tile != 9 && front_tile != 9)
				on_base = true;
		}
		else
		{
			if (map[tile_x + ((tile_y + i) * SCENE_WIDTH)] != 9)
			{
				on_base = true;
			}
		}
		i++;
	}
	
	return on_base;
}

bool cBicho::CollidesMapFloor(int *map, bool up)
{
	/*char str[128];
	sprintf(str, "x = %d \n", x);
	OutputDebugString(str);
	sprintf(str, "y = %d \n", y);
	OutputDebugString(str);
	sprintf(str, "w = %d \n", w);
	OutputDebugString(str);
	sprintf(str, "h = %d \n", h);
	OutputDebugString(str);
	*/
	int tile_x,tile_y;
	int width_tiles, height_tiles;
	bool on_base;
	int i;

	tile_x = x / TILE_SIZE;
	tile_y = y / TILE_SIZE;

	width_tiles = w / TILE_SIZE;
	height_tiles = h / TILE_SIZE;
/*
	sprintf(str, "tile_x = %d \n", tile_x);
	OutputDebugString(str);
	sprintf(str, "tile_y = %d \n", tile_y);
	OutputDebugString(str);
	sprintf(str, "w_tiles = %d \n", width_tiles);
	OutputDebugString(str);
	sprintf(str, "h_tiles = %d \n", height_tiles);
	OutputDebugString(str);
	OutputDebugString("======================================== \n");*/

	if( (x % TILE_SIZE) != 0) width_tiles++;

	if (up)
		tile_y += height_tiles;
	i=0;
	on_base = y < 0 || tile_y > SCENE_HEIGHT;
	while((i<width_tiles) && !on_base)
	{
		if( (y % TILE_SIZE) == 0 )
		{
			int back_tile = map[(tile_x + i) + ((tile_y - 1) * SCENE_WIDTH)];
			int front_tile = map[(tile_x + i) + ((tile_y + 1) * SCENE_WIDTH)];
			if(back_tile != 9 && front_tile != 9 && back_tile != 23 && front_tile != 23)
				on_base = true;
		}
		else
		{
			if(map[ (tile_x + i) + (tile_y * SCENE_WIDTH) ] != 9 && map[(tile_x + i) + (tile_y * SCENE_WIDTH)] != 23 && (y % TILE_SIZE) > 10)
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

void cBicho::Move(int xoff, int yoff, int* map)
{
	int xaux = x;
	int yaux = y;

	x += xoff;
	y += yoff;

	if (map && CollidesMapWall(map, xoff > 0))
		x = xaux;

	if (map && CollidesMapFloor(map, yoff > 0))
		y = yaux;
}

void cBicho::MoveLeft(int step, int* map)
{
	state = STATE_WALKLEFT;
	Move(-step, 0, map);
}

void cBicho::MoveLeft(int *map)
{
	MoveLeft(STEP_LENGTH, map);
}

void cBicho::MoveRight(int step, int* map)
{
	state = STATE_WALKRIGHT;
	Move(step, 0, map);
}

void cBicho::MoveRight(int *map)
{
	MoveRight(STEP_LENGTH, map);
}

void cBicho::MoveUp(int step, int* map)
{
	state = STATE_WALKUP;
	Move(0, step, map);
}

void cBicho::MoveUp(int *map)
{
	MoveUp(STEP_LENGTH, map);
}

void cBicho::MoveDown(int step, int* map)
{
	state = STATE_WALKDOWN;
	Move(0, -step, map);
}

void cBicho::MoveDown(int *map)
{
	MoveDown(STEP_LENGTH, map);
}

bool cBicho::IsHurt() const
{
	return hurt;
}

void cBicho::CalculateCollisionMovement(const cRect& collision, int& hurt_direction)
{
	int xd, yd, xam = 0, yam = 0;
	if (x < collision.right && x + w > collision.right)
	{
		xd = STATE_LOOKLEFT;
		xam = collision.right - x;
	}
	else if (x < collision.left && x + w > collision.left)
	{
		xd = STATE_LOOKRIGHT;
		xam = collision.left - x;
	}
	else
		hurt_direction = rand() % 4;

	if (y < collision.top && y + h > collision.top)
	{
		yd = STATE_LOOKDOWN;
		yam = collision.top - y;
	}
	else if (y < collision.bottom && y + h > collision.bottom)
	{
		yd = STATE_LOOKUP;
		yam = collision.bottom - y;
	}
	else
		yd = hurt_direction = rand() % 4;

	if (xam > yam)
		hurt_direction = xd;
	else
		hurt_direction = yd;
}

void cBicho::ToggleHurt(bool hurt)
{
	this->hurt = hurt;
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

void cBicho::Logic(int *map)
{
	if (!isDead())
	{
		if (hurt)
			Hurt(map);
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

void cBicho::GetActiveWeapons(std::set<cWeapon*>& weapons)
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

void cBicho::ResetFrame()
{
	this->seq = 0;
	this->delay = 0;
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
	active_weapons.insert(weapon);
}

int cBicho::GetState() const
{
	return state;
}
void cBicho::SetState(int s)
{
	state = s;
}

int cBicho::GetOrientation() const
{
	int ori = GetState();
	switch (ori)
	{
	case STATE_WALKUP:
		ori = STATE_LOOKUP;
		break;
	case STATE_WALKDOWN:
		ori = STATE_LOOKDOWN;
		break;
	case STATE_WALKLEFT:
		ori = STATE_LOOKLEFT;
		break;
	case STATE_WALKRIGHT:
		ori = STATE_LOOKRIGHT;
		break;
	}
	return ori;
}

bool cBicho::isDead()
{
	return (this->life <= 0) && (this->frames_to_die <= seq);
}
