#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "rect.h"
#include "player.h"
#include <iostream>
#include "checkCollision.h"
#include "debug.h"
#define MAXHP 20
using namespace std;

extern int gTileSize;

extern SDL_Renderer* mRenderer;

SDL_Rect player::healthBarRedClip;
SDL_Rect player::healthBarRedDest;
SDL_Rect player::healthBarGreenClip;
SDL_Rect player::healthBarGreenDest;

#define NUMOFEXPLOSFRAMES 4
SDL_Texture* player::explosionTexture = NULL;
Uint32 player::explosionFrameTime = 100;
SDL_Rect player::explosionDest = {0,0,gTileSize*2, gTileSize*2};
SDL_Rect player::explosionClip[NUMOFEXPLOSFRAMES] = { {0,0,48,48},{48,0,48,48},{96,0,48,48},{144,0,48,48} };
int player::explosionIndex = 0;

player::player(int playerNum, int vpw, int vph)
{
	explosed = 0;
	explosedStartFrame = 0;

	hp = MAXHP;

	armEnt = NULL;

	initXFlag = 0;
	initYFlag = 0;
	dest = { 0,0,0,0 };
	deadTexture = NULL;
	defaultTexture = NULL;
	currentTexture = NULL;
	flipped = SDL_FLIP_NONE;
	walkingVel = 10;
	fallingVel = 10;
	jumpVel = 10;
	jumped = 0;
	jumpTime = 400;
	punched = 0;
	punchTime = 50;
	velx = 0;
	vely = 0;
	grounded = 0;
	dead = 0;
	viewport.w = vpw;
	viewport.h = vph;
	playerId = playerNum;
	switch (playerNum)
	{
	case 1: viewport.x = 0; viewport.y = 0; break;
	case 2: viewport.x = vpw; viewport.y = 0; break;
	case 3: viewport.x = 0; viewport.y = vph; break;
	case 4: viewport.x = vpw; viewport.y = vph; break;
	}

	leftKey = rightKey = punchKey = jumpKey = -1;
}
Uint32 player::respawnTime = 400;

player::~player()
{
	close();
}

bool player::loadTexture(std::string textureDirectory)
{
	close();
	const char* playerNum = "1";
	switch (playerId)
	{
	case 1: playerNum = "1"; break;
	case 2: playerNum = "2"; break;
	case 3: playerNum = "3"; break;
	case 4: playerNum = "4"; break;
	}
	string defaultPlayer = textureDirectory;
	defaultPlayer += "player";
	defaultPlayer += playerNum;
	defaultPlayer += ".png";
#if DEBUG
	cout << defaultPlayer << endl;
#endif
	string dead = textureDirectory;
	dead += "deadplayer";
	dead += playerNum;
	dead += ".png";
#if DEBUG
	cout << dead << endl;
#endif
	string arm = textureDirectory;
	arm += "armplayer";
	arm += playerNum;
	arm += ".png";
#if DEBUG
	cout << arm << endl;
#endif

	bool success = 1;

	SDL_Surface* surface = NULL;
	surface = IMG_Load(defaultPlayer.c_str());
	if (surface == NULL)
	{
		cout << "Error13" << endl;
		success = 0;
	}
	else
	{
		defaultTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (defaultTexture == NULL)
		{
			cout << "Error14" << endl;
			success = 0;
		}

		dest.w = surface->w;
		dest.h = surface->h;
		SDL_FreeSurface(surface);
	}

	surface = IMG_Load(dead.c_str());
	if (surface == NULL)
	{
		cout << "Error13" << endl;
		success = 0;
	}
	else
	{
		deadTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (deadTexture == NULL)
		{
			cout << "Error14" << endl;
			success = 0;
		}
		SDL_FreeSurface(surface);
	}

	surface = IMG_Load(arm.c_str());
	if (surface == NULL)
	{
		cout << "Error13" << endl;
		success = 0;
	}
	else
	{
		armTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (armTexture == NULL)
		{
			cout << "Error14" << endl;
			success = 0;
		}
		SDL_FreeSurface(surface);
	}

	currentTexture = defaultTexture;

	return success;
}

void player::render(int x, int y)
{
	dest.x = x;
	dest.y = y;
	dest.w = w;
	dest.h = h;
	//SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
	//SDL_RenderFillRect(renderer, &dest);

	SDL_RenderCopyEx(mRenderer, currentTexture, NULL, &dest, 0.0, NULL, flipped);

	if (punched)
	{
		punchDest.w = w;
		punchDest.h = h;
		switch (flipped)
		{
		case SDL_FLIP_NONE:
			punchDest.x = dest.x + dest.w;
			punchDest.y = dest.y;
			SDL_RenderCopyEx(mRenderer, armTexture, NULL, &punchDest, 0.0, NULL, flipped);
			break;
		case SDL_FLIP_HORIZONTAL:
			punchDest.x = dest.x - dest.w;
			punchDest.y = dest.y;
			SDL_RenderCopyEx(mRenderer, armTexture, NULL, &punchDest, 0.0, NULL, flipped);
			break;
		}
	}
	healthBarGreenDest.w = (int)((float)healthBarRedDest.w * ((float)hp / (float)MAXHP));
	healthBarGreenDest.x = dest.x;
	healthBarGreenDest.y = dest.y - gTileSize/3;
	healthBarRedDest.x = dest.x;
	healthBarRedDest.y = dest.y - gTileSize / 3;
	SDL_RenderCopy(mRenderer, healthBarTexture, &healthBarRedClip, &healthBarRedDest);
	SDL_RenderCopy(mRenderer, healthBarTexture, &healthBarGreenClip, &healthBarGreenDest);

	if (explosed)
	{
		if (explosionTimer.getTicks() - explosedStartFrame >= explosionFrameTime)
		{
			explosionIndex++;
			explosedStartFrame = explosionTimer.getTicks();
		}
#if DEBUG
		cout << explosionTimer.getTicks() << endl;
#endif
		explosionDest.x = dest.x - (explosionDest.w / 4);
		explosionDest.y = dest.y - (explosionDest.h / 4);
		SDL_RenderCopy(mRenderer, explosionTexture, &explosionClip[explosionIndex], &explosionDest);
		if (explosionTimer.getTicks() >= explosionFrameTime * NUMOFEXPLOSFRAMES)
		{
				explosionTimer.stop();
				explosed = 0;
				explosionIndex = 0;
				explosedStartFrame = 0;
		}
	}
}

void player::close()
{
	if (defaultTexture != NULL)
	{
		SDL_DestroyTexture(defaultTexture);
	}
}

bool player::isFlipped()
{
	return (flipped == SDL_FLIP_NONE) ? 0 : 1;
}

void player::turnLeft()
{
	flipped = SDL_FLIP_HORIZONTAL;
}

void player::turnRight()
{
	flipped = SDL_FLIP_NONE;
}

void player::setPosX(int x)
{
	if (!initXFlag)
	{
		initPosX = x;
		initXFlag = 1;
	}
	posx = x;
}

void player::setPosY(int y)
{
	if (!initYFlag)
	{
		initPosY = y;
		initYFlag = 1;
	}
	posy = y;
}

void player::setw(int width)
{
	w = width;
}

void player::seth(int height)
{
	h = height;
}

int player::getw()
{
	return w;
}

int player::geth()
{
	return h;
}

int player::getPosX()
{
	return posx;
}

int player::getPosY()
{
	return posy;
}

void player::move(int levelwidth, int levelheight, int numOfBlocks, rect**blocks)
{

	if (dead)
	{
		if (respawnTimer.getTicks() >= respawnTime)
		{
			respawn();
		}
	}
	else
	{

		bool collided = 0;

		posx += velx;
		for (int i = 0; i < numOfBlocks; i++)
		{
			collided = checkCollision(getCollider(), blocks[i]->getCollider());
			if (collided)
				break;
		}
		if (posx < 0 || posx + w > levelwidth || collided)
		{
			posx -= velx;
		}

		if (jumped)
		{
			if (jumpTimer.getTicks() >= jumpTime)
			{
				jumpTimer.stop();
				jumped = 0;
			}
			else
			{
				//posy -= jumpVel*(((jumpTimer.getTicks()-jumpTime)*(jumpTimer.getTicks()-jumpTime))/((jumpTime*jumpTime)/10));
				posy-=jumpVel;
			}
		}
		else
		{
			posy += fallingVel;
		}

		for (int i = 0; i < numOfBlocks; i++)
		{
			collided = checkCollision(getCollider(), blocks[i]->getCollider());
			if (collided)
				break;
		}

		if (posy < 0 || posy + h > levelheight || collided)
		{

			if (jumped)
			{
				posy += jumpVel;
			}
			else
			{
				posy -= fallingVel;
				grounded = 1;
			}
		}

		if (posy + w == levelheight)
		{
			die();
		}

		if (punched)
		{
			if (punchTimer.getTicks() >= punchTime)
			{
				punched = 0;
				punchTimer.stop();
			}
			else
			{
				if (armEnt != NULL)
				{
					if (flipped == SDL_FLIP_NONE)
						armEnt->setx(posx + w);
					else
						armEnt->setx(posx - w);
					armEnt->sety(posy);
				}
			}
		}
	}
}

void player::die()
{
	if (!dead)
	{
#if DEBUG
		cout << "Player " << playerId << " died!" << endl;
#endif
		dead = 1;
		currentTexture = deadTexture;
		respawnTimer.start();
		explosionTimer.start();
		explosed = 1;
		Mix_PlayChannel(-1, explodeSound, 0);
	}
}

bool player::isDead()
{
	return dead;
}

void player::jump()
{
	jumpTimer.start();
	jumped = 1;
	grounded = 0;
}

bool player::isJumped()
{
	return jumped;
}

void player::handleInput(SDL_Event& e)
{
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		if (e.key.keysym.sym == SDLK_i)
		{
			if (playerId == 1)
			{
				cout << "x" << dest.x << endl;
				cout << "w" << dest.w << endl;
				cout << "y" << dest.y << endl;
				cout << "h" << dest.h << endl;
				cout << "x" << posx << endl;
				cout << "y" << posy << endl;
			}
		}
		if (e.key.keysym.sym == leftKey)
		{
			velx -= walkingVel;
			turnLeft();
		}
		else if (e.key.keysym.sym == rightKey)
		{
			velx += walkingVel;
			turnRight();
		}
		else if (e.key.keysym.sym == jumpKey)
		{
			if (grounded)
				if (!jumped)
					jump();
		}
		else if (e.key.keysym.sym == punchKey)
		{
			if (!dead)
				if (!punched){
					punch();
					cout<<"x:"<<getPosX()<<endl;
					cout<<"y:"<<getPosY()<<endl;
				}
		}
		//case SDLK_w: vely -= walkingVel; break;
		//case SDLK_s: vely += walkingVel; break;
	}
	if (e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		//case SDLK_w: vely += walkingVel; break;
		//case SDLK_s: vely -= walkingVel; break;
		if (e.key.keysym.sym == leftKey)
		{
			velx += walkingVel;
		}
		else if (e.key.keysym.sym == rightKey)
		{
			velx -= walkingVel;
		}
	}
}

SDL_Rect& player::getCollider()
{
	collider.w = w;
	collider.h = h;
	collider.x = posx;
	collider.y = posy;

	return collider;
}

void player::setPlayerViewport(int x, int y, int w, int h)
{
	viewport.x = x;
	viewport.y = y;
	viewport.w = w;
	viewport.h = h;
}

SDL_Rect player::getViewport()
{
	return viewport;
}

void player::setupCamera(int x, int y)
{
	camera.x = x;
	camera.y = y;
	camera.w = viewport.w;
	camera.h = viewport.h;
}

SDL_Rect& player::getCamera()
{
	return camera;
}

void player::setViewport()
{
	SDL_RenderSetViewport(mRenderer, &viewport);
}

void player::punch()
{
	Mix_PlayChannel(-1, punchSound, 0);
	punched = 1;
	punchTimer.start();

	armEnt = new entity(w, h);
}

void player::loadSounds()
{
	punchSound = Mix_LoadWAV("data/sounds/punch.mp3");
	explodeSound = Mix_LoadWAV("data/sounds/explode.mp3");
}

Mix_Chunk *player::punchSound = NULL;
Mix_Chunk* player::explodeSound = NULL;


void player::setKeycodes(SDL_Keycode left, SDL_Keycode right, SDL_Keycode punch, SDL_Keycode jump)
{
	leftKey = left;
	rightKey = right;
	jumpKey = jump;
	punchKey = punch;
}


void player::respawn()
{
	hp = MAXHP;
	dead = 0;
	setPosX(initPosX);
	setPosY(initPosY);
	respawnTimer.stop();

	currentTexture = defaultTexture;
}

entity*& player::getArmEnt()
{
	return armEnt;
}

bool player::getHit()
{
	if (!dead)
	{
		hp-=4;
		if (hp <= 0)
		{
			die();
			return 1;
		}
#if DEBUG
		cout << "Player " << playerId << " hp: " << hp << endl;
#endif
		return 0;
	}
}

SDL_Texture* player::healthBarTexture = NULL;

bool player::loadHealthBar()
{
	bool success = 1;

	SDL_Surface* surface = IMG_Load("data/graphics/ui/healthBar.png");
	if (surface == NULL)
	{
		cout << IMG_GetError() << endl;
		success = 0;
	}
	else
	{
		healthBarTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (healthBarTexture == NULL)
		{
			cout << SDL_GetError() << endl;
			success = 0;
		}
		int w = surface->w, h = surface->h;
		healthBarRedClip.x = 0;
		healthBarRedClip.y = 0;
		healthBarRedClip.w = w;
		healthBarRedClip.h = h/2;
		healthBarRedDest.x = 0;
		healthBarRedDest.y = 0;
		healthBarRedDest.w = gTileSize;
		healthBarRedDest.h = gTileSize / 7;

		healthBarGreenClip.x = 0;
		healthBarGreenClip.y = h / 2;
		healthBarGreenClip.w = w;
		healthBarGreenClip.h = h / 2;
		healthBarGreenDest.x = 0;
		healthBarGreenDest.y = 0;
		healthBarGreenDest.w = gTileSize;
		healthBarGreenDest.h = gTileSize / 7;

		SDL_FreeSurface(surface);
	}

	return success;
}

bool player::loadExplosionTexture()
{
	bool success = 1;

	SDL_Surface* surface = IMG_Load("data/graphics/entities/effects/explosion.png");
	if (surface == NULL)
	{
		cout << IMG_GetError() << endl;
		success = 0;
	}
	else
	{
		explosionTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (explosionTexture == NULL)
		{
			cout << SDL_GetError() << endl;
			success = 0;
		}

		SDL_FreeSurface(surface);
	}

	return success;
}

void player::scoreUp() {
	/*if(score == 9)
		score = 0;
	else*/
		score++;
}
int player::getScore() {
	return score;
}
