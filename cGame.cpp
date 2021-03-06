#include "cGame.h"
#include "Globals.h"
#include <Windows.h>
#include <time.h>
#include "wSword.h"
#include "eOctorok.h"
#include "eStalfos.h"
#include "eRope.h"
#include "eAquamentus.h"
#include "wBow.h"
#include "fmod/fmod.hpp"


cGame::cGame(void)
{
}

cGame::~cGame(void)
{
}

bool cGame::Init()
{
	FMOD_RESULT       result;
	FMOD::Sound      *sound;
	int               numsubsounds;
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

	res = Data.LoadImage(IMG_INTERFACE, "res/interface_tiles.png", GL_RGBA);
	if (!res) return false;

	res = Data.LoadImage(IMG_OCTOROK, "res/octorok.png", GL_RGBA);
	if (!res) return false;

	res = Data.LoadImage(IMG_STALFOS, "res/stalfos.png", GL_RGBA);
	if (!res) return false;

	res = Data.LoadImage(IMG_ROPE, "res/rope.png", GL_RGBA);
	if (!res) return false;

	res = Data.LoadImage(IMG_AQUAMENTUS, "res/aquamentus.png", GL_RGBA);
	if (!res) return false;

	res = Data.LoadImage(IMG_DUNGEON, "res/dungeon_Tiles.png", GL_RGBA);
	if (!res) return false;

	res = Data.LoadImage(IMG_TREASURES, "res/treasures.png", GL_RGBA);
	if (!res) return false;

	result = fmod_system->createStream("sounds/overworld.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
	if (result != FMOD_OK) return false;
	sound->setLoopPoints(7000, FMOD_TIMEUNIT_MS, 40000, FMOD_TIMEUNIT_MS);

	result = sound->getNumSubSounds(&numsubsounds);
	if (result != FMOD_OK) return false;

	if (numsubsounds)
	{
		sound->getSubSound(0, &overworld_sound);
	}
	else
	{
		overworld_sound = sound;
	}

	result = fmod_system->createStream("sounds/dungeon.mp3", FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
	if (result != FMOD_OK) return false;
	sound->setLoopPoints(010, FMOD_TIMEUNIT_MS, 19500, FMOD_TIMEUNIT_MS);

	result = sound->getNumSubSounds(&numsubsounds);
	if (result != FMOD_OK) return false;

	if (numsubsounds)
	{
		sound->getSubSound(0, &dungeon_sound);
	}
	else
	{
		dungeon_sound = sound;
	}

	result = fmod_system->createSound("sounds/stairs.wav", FMOD_DEFAULT | FMOD_LOOP_OFF, 0, &stairs_sound);
	if (result != FMOD_OK) return false;

	// Prepare the channel
	result = fmod_system->playSound(overworld_sound, 0, true, &audio_channel);
	if (result != FMOD_OK) return false;

	////Show player
	
	Player.SetWidthHeight(16,16);
	Player.SetTile(6,5);
	Player.SetState(STATE_LOOKDOWN);
	cWeapon* bw = new wBow(1);
	bw->SetTexture(Data.GetID(IMG_WEAPONS));

	Player.AddWeapon(BOW, bw);
	Player.SetAWeapon(BOW);

	cWeapon* aw = new wSword(1);
	aw->SetTexture(Data.GetID(IMG_WEAPONS));

	Player.AddWeapon(SWORD, aw);
	Player.SetBWeapon(SWORD);

	fMatrix enemies;
	Scene.GetEnemies(enemies);
	LoadEnemies(enemies);

	Interface = *new cInterface();
	dungeon = false;

	return res;
}

bool cGame::Loop()
{
	time_t start = time(NULL);
	bool res=true, paused;

	audio_channel->getPaused(&paused);
	fmod_system->update();
	if (paused && state != STATE_TRANSITION)
		audio_channel->setPaused(false);

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
bool pressed_s = false;

//Input
void cGame::ReadKeyboard(unsigned char key, int x, int y, bool press)
{
	keys[key] = press;
	if (key == 'a' && !press)
		pressed_a = false;
	if (key == 's' && !press)
		pressed_s = false;
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
	if (keys['s'] && !pressed_s)
	{
		pressed_s = true;
		Player.BAttack();
	}
	else if (keys[GLUT_KEY_UP])		Player.MoveUp(Scene.GetMap());
	else if (keys[GLUT_KEY_DOWN])	Player.MoveDown(Scene.GetMap());
	else if (keys[GLUT_KEY_LEFT])	Player.MoveLeft(Scene.GetMap());
	else if (keys[GLUT_KEY_RIGHT])	Player.MoveRight(Scene.GetMap());
	else Player.Stop();


	//Game Logic
	Player.Logic(Scene.GetMap(), enemies, objects_drop);


	int transition_direction = 0;

	if (Player.GetState() != STATE_DYING && !Player.isDead())
	{
		for (auto it = enemies.begin(); it != enemies.end(); ++it)
		{
			cEnemy *enemy = *it;
			enemy->Logic(Scene.GetMap(), Player);
			if (enemy->isDead())
			{
				if (rand() % 2) {
					cDrop *obj_drop;
					if (rand() % 2)
						obj_drop = new cDrop(Data.GetID(IMG_TREASURES), RUPEE, (rand() % 5)+1);
					else
						obj_drop = new cDrop(Data.GetID(IMG_TREASURES), HEART, 1);

					int posx, posy;
					enemy->GetPosition(&posx, &posy);
					obj_drop->SetPosition(posx, posy);
					objects_drop.push_back(obj_drop);
				}
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
				transition_direction = Player.GetDirectionTransition();
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
	Interface.Process(Player.GetLife(), Player.GetHearts(), Player.GetRupees(), Player.HasSword(), Player.HasBow(), transition_direction);

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
			Scene.DrawTransition(direction_transition, transition_num, dungeon);
			Player.UpdateTransitionPos(transition_num);
			DrawScene();
			//Scene.Draw(Data.GetID(IMG_BLOCKS));
			Player.Draw(Data.GetID(IMG_PLAYER));
			if (Scene.TransitionFinished(direction_transition, transition_num)) {
				Scene.UpdateMap();
				if (Scene.GetNewLevel() == 8) {
					cDrop* tf = new cDrop(Data.GetID(IMG_TREASURES), TRIFORCE, 0);
					tf->SetTile(8, 5);
					objects_drop.push_back(tf);
				}
				fMatrix enemies;
				Scene.GetEnemies(enemies);
				LoadEnemies(enemies);
				Player.SetStateAfterTransition();
				state = STATE_PLAYING;
				if (direction_transition == TRANSITION_INSIDE || direction_transition == TRANSITION_OUTSIDE)
				{
					if (dungeon)
						fmod_system->playSound(dungeon_sound, 0, true, &audio_channel);
					else
						fmod_system->playSound(overworld_sound, 0, true, &audio_channel);
				}
			}
			else
				transition_num++;
			break;
		default:
			DrawScene();
			//Scene.Draw(Data.GetID(IMG_BLOCKS));
			if (!Player.isDead())
			{
				Player.Draw(Data.GetID(IMG_PLAYER));
				if (Player.GetState() != STATE_DYING)
					for (cEnemy* e : enemies)
						e->Draw();
				for (cDrop* obj : objects_drop)
					obj->Draw();
			}
			break;
	}
	Interface.Draw(Data.GetID(IMG_INTERFACE));
	glutSwapBuffers();
}

void cGame::DrawScene() {
	if (state == STATE_TRANSITION && direction_transition == TRANSITION_INSIDE) {
		if(transition_num == TILE_SIZE)
			Scene.Draw(Data.GetID(IMG_DUNGEON));
		else
			Scene.Draw(Data.GetID(IMG_BLOCKS));
	}
	else if (state == STATE_TRANSITION && direction_transition == TRANSITION_OUTSIDE) {
		if (transition_num == 1)
			Scene.Draw(Data.GetID(IMG_DUNGEON));
		else
			Scene.Draw(Data.GetID(IMG_BLOCKS));
	}
	else {
		if (dungeon)
			Scene.Draw(Data.GetID(IMG_DUNGEON));
		else
			Scene.Draw(Data.GetID(IMG_BLOCKS));
	}
}

bool cGame::StartTransition() {
	enemies.clear();
	objects_drop.clear();

	state = STATE_TRANSITION;
	direction_transition = Player.GetDirectionTransition();
	if (direction_transition == TRANSITION_BOTTOM && Scene.TransitionIsPosible(TRANSITION_OUTSIDE)){
		Player.SetDirectionTransition(TRANSITION_OUTSIDE);
		direction_transition = Player.GetDirectionTransition();
	}
	Player.StartTransition(Scene.GetTransitionOutsidePos());
	transition_num = 1;
	bool trans = Scene.InitTransition(direction_transition);

	if (direction_transition == TRANSITION_INSIDE || direction_transition == TRANSITION_OUTSIDE)
	{
		audio_channel->setPaused(true);
		FMOD::Channel *channel;
		fmod_system->playSound(stairs_sound, 0, false, &audio_channel);
	}

	if (direction_transition == TRANSITION_INSIDE)
		dungeon = true;
	if (direction_transition == TRANSITION_OUTSIDE)
		dungeon = false;
	if (!trans)
		state = STATE_PLAYING;

	return trans;
}

void cGame::LoadEnemies(const fMatrix& mEnemies)
{
	
	for (auto def : mEnemies)
	{

		int x = def[4];
		int y = def[5];
		for (int i = 0; i < def[1]; ++i)
		{
			cEnemy* enemy = nullptr;
			switch (int(def[0]))
			{
			case 0: // OCTOROK
				enemy = new eOctorok(Data.GetID(IMG_OCTOROK), def[2], def[3]);
				break;
			case 1:
				enemy = new eStalfos(Data.GetID(IMG_STALFOS), def[2], def[3]);
				break;
			case 2:
				enemy = new eRope(Data.GetID(IMG_ROPE), def[2], def[3]);
				break;
			case 3:
				enemy = new eAquamentus(Data.GetID(IMG_AQUAMENTUS), def[2], def[3]);
				break;
			}

			if (!enemy)
				break;

			int st = rand() % 4;
			enemy->SetState(st);
			enemy->SetTile(x, y);
			enemies.push_back(enemy);
		}
	}
}
