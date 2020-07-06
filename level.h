#pragma once
#include <string>
#include "entity.h"
using namespace std;
class level
{
public:
	level(int width, int height);
	~level();

	bool loadBGTexture(string path);
	bool loadMusic(string path);

	void playMusic();

	void renderBG(SDL_Rect camera);
	void renderBlocks(int camerax, int cameray);

	rect** getBlocks();
	int getBlockNum();

	void close();

	int w, h;
private:
	SDL_Texture* bgTexture;
	SDL_Rect bgDest;
	Mix_Music* levelMusic;
	int numOfBlocks;
	rect** blocks;
};
