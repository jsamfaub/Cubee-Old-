#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#undef main
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "main.h"
#include "entity.h"
#include "init.h"
#include "close.h"
#include "rect.h"
#include "player.h"
#include "checkCollision.h"
#include "background.h"
#include "ui.h"
#include "level.h"
#include "LWindow.h"
#include "gdefines.h"

using namespace std;

int gTileSize = 50;

SDL_Renderer* mRenderer = NULL;
LWindow gWindow[TOTAL_WINDOWS];
int screenwidth = 1920;//1280;
int screenheight = 1080;//720;
int levelwidth = 500;
int levelheight = 1000;
int scoreLimit = 25;
entity** entities=NULL;
int numOfEntities=0;

int main(void)
{
	if (!init())
	{

	}
	else
	{
		//gTileSize=screenwidth/38.4;
		mRenderer = gWindow[0].getRenderer();

		ui interface("data/graphics/ui/test002.png", "data/graphics/ui/blue.bmp");
		player::loadSounds();
		player::loadHealthBar();
		player::loadExplosionTexture();

		level *currentLevel = new level("data/levels/level1.lvl");
		currentLevel->loadBGTexture("data/graphics/backgrounds/test003.png");
		currentLevel->loadMusic("data/music/levelmusic.mp3");
		currentLevel->playMusic();

		entity* enemy=new entity(50,50,100,100);
		enemy->setTexturePath("data/graphics/entities/enemies/red.png");


		int numOfPlayers = 4;
		player *players[] = {new player(1, screenwidth/2, screenheight/2),
							 new player(2, screenwidth/2, screenheight/2),
							 new player(3, screenwidth/2, screenheight/2),
							 new player(4, screenwidth/2, screenheight/2) };
		for (int i = 0; i < numOfPlayers; i++)
		{
			players[i]->loadTexture("data/graphics/entities/player/");
			players[i]->setw(gTileSize);
			players[i]->seth(gTileSize);
			players[i]->setPosX(i*players[i]->getw() + players[i]->getw());
			players[i]->setPosY(0);
		}
		players[0]->setKeycodes(SDLK_a, SDLK_d, SDLK_v, SDLK_c);
		players[1]->setKeycodes(SDLK_LEFT, SDLK_RIGHT, SDLK_o, SDLK_p);
		
		bool over = 0;
		SDL_Event e;
		
		for (int i = 0; i < numOfPlayers; i++)
		{
			if(i == 0)
			players[i]->setupCamera(0, 0);
			else if (i == 1)
				players[i]->setupCamera(players[i]->getViewport().w, 0);
			else if (i == 2)
				players[i]->setupCamera(0, players[i]->getViewport().h);
			else if (i == 3)
				players[i]->setupCamera(players[i]->getViewport().w, players[i]->getViewport().h);
		}

		background bg(players[0]->getCamera().w, players[0]->getCamera().h);
		bg.loadTexture("data/graphics/backgrounds/test002.png");

		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

		for (int i = 1; i < TOTAL_WINDOWS; i++)
		{
			gWindow[i].init();
		}

		while (!over)
		{
			while (SDL_PollEvent(&e) > 0)
			{
				for (int i = 0; i < TOTAL_WINDOWS; i++)
				{
					gWindow[i].handleEvent(e);
				}

				if (e.type == SDL_QUIT)
				{
					over = 1;
				}
				if (e.type == SDL_KEYDOWN)
				{
					switch (e.key.keysym.sym)
					{
					case SDLK_q: over = 1; break;
					case SDLK_1: gWindow[0].focus(); break;
					case SDLK_2: enemy->printInfo();break;
					}
				}

				for (int i = 0; i < numOfPlayers; i++)
				{
					players[i]->handleInput(e);
				}
			}
			/*if (currentKeyStates[SDL_SCANCODE_A])
			{
				for (int i = 0; i < numOfPlayers; i++)
				{
					players[i]->turnLeft();
				}
			}
			if (currentKeyStates[SDL_SCANCODE_D])
			{
				for (int i = 0; i < numOfPlayers; i++)
				{
					players[i]->turnRight();
				}
			}*/
			for (int i = 0; i < numOfPlayers; i++)
			{
				players[i]->move(currentLevel->w, currentLevel->h, currentLevel->getBlockNum(), currentLevel->getBlocks());
			}
			for (int i = 0; i < numOfPlayers; i++)
			{
				for (int j = 0; j < numOfPlayers; j++)
				{
					if(players[j]->getArmEnt() != NULL){
					for(int k=0;k<numOfEntities;k++){
						if(entities[k]!=NULL)
						if(entities[k]->hitBy(players[j]->getArmEnt())){
						players[j]->scoreUp();
						delete entities[k];
						entities[k]=NULL;
						}
					}
					if (checkCollision(players[i]->getCollider(), players[j]->getArmEnt()->getCollider()))
						{
							if (players[i]->playerId != players[j]->playerId)
							{
								bool dead = 0;
								dead = players[i]->getHit();
								delete players[j]->getArmEnt();
								players[j]->getArmEnt() = NULL;
								if(dead)
								players[j]->scoreUp();
								interface.inputScore(players[j]->getScore());
							}
						}
					}
				}
			}


			for (int i = 0; i < numOfPlayers; i++)
			{
				for(int j=0;j<numOfEntities;j++){
					if(entities[j]!=NULL){
						entities[j]->fall();
						if(entities[j]->near(players[i]->getPosX(),players[i]->getPosY(),gTileSize*5)){
							/*delete entities[j];
							entities[j]=NULL;*/
							entities[j]->followPlayer(players[i]->getPosX(),players[i]->getPosY());
						}
						entities[j]->move(currentLevel->getBlocks(),currentLevel->getBlockNum());

						entities[j]->ground(currentLevel->getBlocks(),currentLevel->getBlockNum());
					}
				}
				players[i]->getCamera().x = (players[i]->getPosX() + (players[i]->getw() / 2)) - players[i]->getViewport().w / 2;
				players[i]->getCamera().y = (players[i]->getPosY() + (players[i]->geth() / 2)) - players[i]->getViewport().h / 2;

				if (players[i]->getCamera().x < 0)
				{
					players[i]->getCamera().x = 0;
				}
				if (players[i]->getCamera().y < 0)
				{
					players[i]->getCamera().y = 0;
				}
				if (players[i]->getCamera().x > currentLevel->w - players[i]->getCamera().w)
				{
					players[i]->getCamera().x = currentLevel->w - players[i]->getCamera().w;
				}
				if (players[i]->getCamera().y > currentLevel->h - players[i]->getCamera().h)
				{
					players[i]->getCamera().y = currentLevel->h - players[i]->getCamera().h;
				}
			}


			if (!gWindow[0].isMinimized())
			{
				SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
				SDL_RenderClear(mRenderer);
				for (int i = 0; i < numOfPlayers; i++)
				{
					players[i]->setViewport();
					currentLevel->renderBG(players[i]->getCamera());

					currentLevel->renderBlocks(players[i]->getCamera().x, players[i]->getCamera().y);

					enemy->renderEntity(400-players[i]->getCamera().x,900-players[i]->getCamera().y);
					for(int j=0;j<numOfEntities;j++){
						if(entities[j]!=NULL)entities[j]->renderEntity(entities[j]->getx()-players[i]->getCamera().x,entities[j]->gety()-players[i]->getCamera().y);
					}
					for (int j = 0; j < numOfPlayers; j++)
					{
						players[j]->render(players[j]->getPosX() - players[i]->getCamera().x, players[j]->getPosY() - players[i]->getCamera().y);
					}
					interface.render(players[i]->getScore());
				}
			}

			SDL_RenderPresent(mRenderer);

			bool allWindowsClosed = 1;
			for (int i = 0; i < TOTAL_WINDOWS; i++)
			{
				if(gWindow[i].isShown())
				{
					allWindowsClosed = 0;
					break;
				}
			}

			if (allWindowsClosed)
			{
				over = 1;
			}
		}
	}

	close();

	return 0;
}
