#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "rect.h"
#include "level.h"
#include "entity.h"
#include <iostream>

extern SDL_Renderer* mRenderer;
extern int gTileSize;
extern int levelwidth;

level::level(int width, int height)
{
	numOfBlocks = 100;
	blocks = (rect * *)malloc(sizeof(rect*) * numOfBlocks);
	for (int i = 0; i < numOfBlocks; i++)
	{
		blocks[i] = new rect;
	}
	blocks[0]->setw(gTileSize * 7);
	blocks[0]->seth(gTileSize);
	blocks[0]->setCol(0xDD,0xDD,0xDD);
	blocks[0]->setx(gTileSize);
	blocks[0]->sety(height - blocks[0]->geth());
	blocks[0]->fill();

	blocks[1]->setw(gTileSize * 13);
	blocks[1]->seth(gTileSize * 2);
	blocks[1]->setCol(0xDD,0xDD,0xDD);
	blocks[1]->setx(blocks[0]->getw());
	blocks[1]->sety(height - blocks[1]->geth());
	blocks[1]->fill();

	blocks[2]->setw(gTileSize * 4);
	blocks[2]->seth(gTileSize*2);
	blocks[2]->setCol(0xDD,0xDD,0xDD);
	blocks[2]->setx(blocks[1]->getx() + (gTileSize * 3) );
	blocks[2]->sety(height - (blocks[1]->geth()+blocks[2]->geth() ) );
	blocks[2]->fill();

	blocks[3]->setw(gTileSize * 3);
	blocks[3]->seth(gTileSize * 1);
	blocks[3]->setCol(0xDD,0xDD,0xDD);
	blocks[3]->setx(blocks[2]->getx() + blocks[2]->getw() + gTileSize );
	blocks[3]->sety(blocks[2]->gety());
	blocks[3]->fill();
	
	blocks[4]->setw(gTileSize * 1);
	blocks[4]->seth(gTileSize * 2);
	blocks[4]->setCol(0xDD,0xDD,0xDD);
	blocks[4]->setx(blocks[3]->getx() + blocks[3]->getw() + gTileSize);
	blocks[4]->sety(blocks[2]->gety() );
	blocks[4]->fill();

	blocks[5]->setw(gTileSize * 1);
	blocks[5]->seth(gTileSize * 3);
	blocks[5]->setCol(0xDD,0xDD,0xDD);
	blocks[5]->setx(blocks[3]->getx()+gTileSize);
	blocks[5]->sety(blocks[3]->gety()-blocks[5]->geth());
	blocks[5]->fill();

	blocks[6]->setw(gTileSize * 4);
	blocks[6]->seth(gTileSize * 1);
	blocks[6]->setCol(0xDD,0xDD,0xDD);
	blocks[6]->setx(blocks[2]->getx());
	blocks[6]->sety(blocks[5]->gety() - gTileSize);
	blocks[6]->fill();
	bgTexture = NULL;
	w = width, h = height;
	levelMusic = NULL;

	SDL_Rect bgDest = { 0,0,0,0 };
}

level::~level()
{
	close();
}

void level::close()
{
	Mix_HaltMusic();

	SDL_DestroyTexture(bgTexture);
	Mix_FreeMusic(levelMusic);
	for (int i = 0; i < numOfBlocks; i++)
	{
		delete blocks[i];
	}
	free(blocks);
}

bool level::loadBGTexture(string path)
{
	bool success = 1;

	SDL_Surface* surface;
	surface = IMG_Load(path.c_str());
	if (surface == NULL)
	{
		cout << IMG_GetError() << endl;
		success = 0;
	}
	else
	{
		bgTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (bgTexture == NULL)
		{
			cout << SDL_GetError() << endl;
			success = 0;
		}
		SDL_FreeSurface(surface);
	}

	return success;
}

bool level::loadMusic(string path)
{
	bool success = 1;

	levelMusic = Mix_LoadMUS(path.c_str());
	if (levelMusic == NULL)
	{
		cout << Mix_GetError() << endl;
		success = 0;
	}

	return success;
}

void level::playMusic()
{
	Mix_PlayMusic(levelMusic, -1);
}

void level::renderBG(SDL_Rect camera)
{
	bgDest.w = camera.w;
	bgDest.h = camera.h;

	while (camera.x > bgDest.w)
		camera.x -= bgDest.w;
	while (camera.y > bgDest.h)
		camera.y -= bgDest.h;
	bgDest.x = -camera.x;
	bgDest.y = -camera.y;
	bgDest.w = bgDest.w;
	bgDest.h = bgDest.h;
	SDL_RenderCopy(mRenderer, bgTexture, NULL, &bgDest);
	bgDest.x += bgDest.w;
	SDL_RenderCopy(mRenderer, bgTexture, NULL, &bgDest);
	bgDest.y += bgDest.h;
	SDL_RenderCopy(mRenderer, bgTexture, NULL, &bgDest);
	bgDest.x -= bgDest.w;
	SDL_RenderCopy(mRenderer, bgTexture, NULL, &bgDest);
}

void level::renderBlocks(int camerax, int cameray)
{
	for (int i = 0; i < numOfBlocks; i++)
	{
		blocks[i]->render(blocks[i]->getx() - camerax, blocks[i]->gety() - cameray);
	}
}

rect** level::getBlocks()
{
	return blocks;
}

int level::getBlockNum()
{
	return numOfBlocks;
}
