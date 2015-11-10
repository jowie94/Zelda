#include "cGame.h"
#include "Globals.h"
#include <Windows.h>
#include <time.h>
#include "wSword.h"
#include "eOctorok.h"


cGame::cGame(void)
{
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	bool res=true;

	//Graphics initialization
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,GAME_WIDTH/2.5,0,GAME_HEIGHT/2.5,0,1);
	glMatrixMode(GL_MODELVIEW);
	
	glAlphaFunc(GL_GREATER, 0.05f);
	glEnable(GL_ALPHA_TEST);

	//Scene initialization
	res = Data.LoadImage(IMG_BLOCKS,"res/WorldTiles.png",GL_RGBA);
	if(!res) return false;
	res = Scene.LoadLevel(1);
	if(!res) return false;

	////Player initialization
	res = Data.LoadImage(IMG_PLAYER,"res/link.png",GL_RGBA);
	if(!res) return false;

	res = Data.LoadImage(IMG_WEAPONS, "res/weapons.png", GL_RGBA);
	if (!res) return false;

	res = Data.LoadImage(IMG_INTERFACE, "res/link_life.png", GL_RGBA);
	if (!res) return false;

	res = Data.LoadImage(IMG_OCTOROK, "res/octorok.png", GL_RGBA);
	if (!res) return false;

	////Show player
	
	Player.SetWidthHeight(16,16);
	Player.SetTile(6,5);
	Player.SetState(STATE_LOOKDOWN);
	wSword* sw = new wSword(1);
	sw->SetTexture(Data.GetID(IMG_WEAPONS));

	Player.AddWeapon(SWORD, sw);
	Player.SetAWeapon(SWORD);
	
	fMatrix enemies;
	Scene.GetEnemies(enemies);
	LoadEnemies(enemies);

	Interface = *new cInterface(Player.GetLife());

	return res;
}

bool cGame::Loop()
{
	time_t start = time(NULL);
	bool res=true;

	if (Player.isDead())
	{
		this->enemies.clear();
		fMatrix enemies;
		Scene.GetEnemies(enemies);
		LoadEnemies(enemies);

		Player.SetLife(INITIAL_LIFE);
		Player.SetTile(6, 5);
		Player.SetState(STATE_LOOKDOWN);
		Player.ResetFrame();
		Player.SetFramesToDie(5);
	}

	if(state != STATE_TRANSITION)
		res = Process();
	if(res) Render();

	time_t end = time(NULL);
	Sleep(15 - (end - start));
	return res;
}

void cGame::Finalize()
{
}

bool pressed_a = false;

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
	if (key == 'a' && !press)
		pressed_a = false;
}

void cGame::ReadMouse(int button, int state, int x, int y)
{
}

//Process
bool cGame::Process()
{
	bool res=true;
	
	//Process Input
	if(keys[27])	res=false;
	if (keys['a'] && !pressed_a)
	{
		pressed_a = true;
		Player.AAttack();
	}
	else if (keys[GLUT_KEY_UP])		Player.MoveUp(Scene.GetMap());
	else if (keys[GLUT_KEY_DOWN])	Player.MoveDown(Scene.GetMap());
	else if (keys[GLUT_KEY_LEFT])	Player.MoveLeft(Scene.GetMap());
	else if (keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap());
	else Player.Stop();


	//Game Logic
	Player.Logic(Scene.GetMap(), enemies);

	if (Player.GetState() != STATE_DYING && !Player.isDead())
	{
		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			cEnemy *enemy = *it;
			enemy->Logic(Scene.GetMap(), Player);
			if (enemy->isDead())
			{
				it = enemies.erase(it);
				if (it == enemies.end())
					break;
			}
		}

		int player_state = Player.GetState();

		if (player_state == STATE_DOOR) {
			int dir = Player.GetDirectionTransition();
			if (Scene.TransitionIsPosible(dir)) {
				res = cGame::StartTransition();
			}
			switch (dir)
			{
			case TRANSITION_BOTTOM:
				Player.SetState(STATE_WALKDOWN);
				break;
			case TRANSITION_TOP:
				Player.SetState(STATE_WALKUP);
				break;
			case TRANSITION_INSIDE:
				Player.SetState(STATE_WALKUP);
				break;
			case TRANSITION_OUTSIDE:
				Player.SetState(STATE_WALKDOWN);
				break;
			case TRANSITION_RIGHT:
				Player.SetState(STATE_WALKRIGHT);
				break;
			case TRANSITION_LEFT:
				Player.SetState(STATE_WALKLEFT);
				break;
			}
		}
	}
	Interface.Process(Player.GetLife());

	return res;
}

//Output
void cGame::Render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();

	switch (state)
	{
		case STATE_TRANSITION:
			Scene.DrawTransition(direction_transition, transition_num);
			Player.UpdateTransitionPos(transition_num);
			Scene.Draw(Data.GetID(IMG_BLOCKS));
			Player.Draw(Data.GetID(IMG_PLAYER));
			if (Scene.TransitionFinished(direction_transition, transition_num)) {
				Scene.UpdateMap();
				fMatrix enemies;
				Scene.GetEnemies(enemies);
				LoadEnemies(enemies);
				Player.SetStateAfterTransition();
				state = STATE_PLAYING;
			}
			else
				transition_num++;
			break;
		default:
			Scene.Draw(Data.GetID(IMG_BLOCKS));
			if (!Player.isDead())
			{
				Player.Draw(Data.GetID(IMG_PLAYER));
				if (Player.GetState() != STATE_DYING)
				for (cEnemy* e : enemies)
					e->Draw();
			}
			break;
	}
	Interface.Draw(Data.GetID(IMG_INTERFACE));
	glutSwapBuffers();
}

bool cGame::StartTransition() {
	enemies.clear();

	state = STATE_TRANSITION;
	direction_transition = Player.GetDirectionTransition();
	if (direction_transition == TRANSITION_BOTTOM && Scene.TransitionIsPosible(TRANSITION_OUTSIDE)){
		Player.SetDirectionTransition(TRANSITION_OUTSIDE);
		direction_transition = Player.GetDirectionTransition();
	}
	Player.StartTransition(Scene.GetTransitionOutsidePos());
	transition_num = 1;
	bool trans = Scene.InitTransition(direction_transition);
	if (!trans)
		state = STATE_PLAYING;

	return trans;
}

void cGame::LoadEnemies(const fMatrix& mEnemies)
{
	std::vector<int> usable_positions;

	for (int i = 0; i < SCENE_WIDTH * SCENE_HEIGHT; ++i)
	{
		if (Scene.GetMap()[i] == 9)
			usable_positions.push_back(i);
	}

	for (auto def : mEnemies)
	{
		for (int i = 0; i < def[1]; ++i)
		{
			cEnemy* enemy;
			switch (int(def[0]))
			{
			case 0: // OCTOROK
				enemy = new eOctorok(Data.GetID(IMG_OCTOROK), def[2], def[3]);
				break;
			}

			enemy->SetWidthHeight(16, 16);
			int st = rand() % 4, pos;
			enemy->SetState(st);
			pos = rand() % usable_positions.size();
			pos = usable_positions[pos];
			enemy->SetTile(pos / SCENE_WIDTH, pos / SCENE_HEIGHT);
			enemies.push_back(enemy);
		}
	}
}
