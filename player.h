#pragma once
#include <string>
#include "rect.h"
#include "LTimer.h"
#include "entity.h"
#include "gdefines.h"
class player
{
public:
	player(int playerNum, int vpw, int vph); //viewPort width, and viewPort height
	~player();

	bool loadTexture(std::string textureDirectory);
	void render(int x, int y);
	void close(void);

	bool isFlipped();

	void turnLeft();
	void turnRight();

	void setPosX(int x);
	void setPosY(int y);

	void setw(int w);
	void seth(int h);

	int getw();
	int geth();
	int getPosX();
	int getPosY();

	void move(int levelwidth, int levelheight, int numOfBlocks, rect** blocks);
	void handleInput(SDL_Event& e);

	SDL_Rect& getCollider();

	void jump();
	bool isJumped();
	void punch();

	void die();
	bool isDead();

	void setPlayerViewport(int x, int y, int w, int h);
	SDL_Rect getViewport();

	void setupCamera(int x, int y);
	SDL_Rect& getCamera();

	static void loadSounds();
	static Mix_Chunk* punchSound;
	static Mix_Chunk* explodeSound;

	void setViewport();

	//keys:
	void setKeycodes(SDL_Keycode left, SDL_Keycode right, SDL_Keycode punch, SDL_Keycode jump);
	SDL_Keycode leftKey, rightKey, punchKey, jumpKey;

	int hp;

	entity*& getArmEnt();
	bool getHit();
	
	int playerId;

	static SDL_Texture* healthBarTexture;
	static bool loadHealthBar();
	static SDL_Rect healthBarRedClip;
	static SDL_Rect healthBarRedDest;
	static SDL_Rect healthBarGreenClip;
	static SDL_Rect healthBarGreenDest;

	static bool loadExplosionTexture();
	static SDL_Texture* explosionTexture;
	static Uint32 explosionFrameTime;
	static SDL_Rect explosionDest;
	static SDL_Rect explosionClip[4];
	static int explosionIndex;
	LTimer explosionTimer;
	bool explosed;
	Uint32 explosedStartFrame;
	int getScore();
	void scoreUp();
private:
		int score=0;

	bool dead;
	bool grounded;
	Uint32 jumpTime;
	int jumpVel;
	bool jumped;
	LTimer jumpTimer;
	SDL_Rect collider;
	SDL_Rect dest;
	int posx, posy;
	int velx, vely, walkingVel, fallingVel;
	SDL_Texture* defaultTexture, *deadTexture, *currentTexture;
	SDL_Texture* armTexture;
	SDL_RendererFlip flipped;
	int w, h;

	SDL_Rect viewport;
	SDL_Rect camera;

	Uint32 punchTime;
	bool punched;
	LTimer punchTimer;
	SDL_Rect punchDest;
	entity* armEnt;

	int initPosX;
	bool initXFlag;
	int initPosY;
	bool initYFlag;

	LTimer respawnTimer;
	static Uint32 respawnTime;
	void respawn();
};
