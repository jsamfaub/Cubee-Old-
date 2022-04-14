#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "rect.h"
#include "level.h"
#include "entity.h"
#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

extern SDL_Renderer* mRenderer;
extern int gTileSize;
extern int levelwidth;
extern int numOfEntities;
extern entity** entities;
struct entityInfo{
	int x;
	int y;
	int w;
	int h;
	char type;
};
vector<entityInfo> entitiesInfo;

struct {
	int w;
	int h;
	int numOfBlocks;
	rect** blocks;
} levelData;
void getData(string level){
	fstream file;
	file.open(level,ios::in);
	char block=NULL;
	char ch;
	int x=0;
	int y=0;
	bool xed=false;
	while(!file.eof()){
		file>>ch;
		if(ch=='n') {
			y++;
			xed=true;
		}else if(!xed)
			x++;
	}++y;
	cout<<"val"<<x<<endl;
	cout<<"val"<<y<<endl;
	levelData.w=x*gTileSize;
	levelData.h=y*gTileSize;
	levelData.numOfBlocks=x*y;
	levelData.blocks=(rect * *)malloc(sizeof(rect*) *levelData.numOfBlocks);
	for(int i=0;i<levelData.numOfBlocks;i++){
		levelData.blocks[i]=new rect;
	}
	file.clear();
	file.seekg(0);
	int posx=0,posy=0;
	for(int i=0;i<levelData.numOfBlocks;i++) {
		file>>ch;
		if(posx%x==0&&posx!=0){
			cout<<endl;
			posx=0;
			posy++;
		}
		if(ch=='n'){
		cout<<endl;
		i--;
		continue;
		}else{
		bool colorSet=false;
		switch(ch){
			case 'e':
				entityInfo temp;
				temp.x=posx;
				temp.y=posy;
				temp.w=50;
				temp.h=50;
				temp.type='e';
				entitiesInfo.push_back(temp);
				break;
			case 'r':
				if(!colorSet){colorSet=true;levelData.blocks[i]->setCol(0xFF,0x00,0x00);}
			case 'g':
				if(!colorSet){colorSet=true;levelData.blocks[i]->setCol(0x00,0xFF,0x00);}
			case 'b':
				if(!colorSet){colorSet=true;levelData.blocks[i]->setCol(0x00,0x00,0xFF);}
			case 'y':
				if(!colorSet){colorSet=true;levelData.blocks[i]->setCol(0xFF,0xFF,0x00);}
			case '#':
				if(!colorSet){colorSet=true;levelData.blocks[i]->setCol(0xDD,0xDD,0xDD);}

				levelData.blocks[i]->setw(gTileSize);
				levelData.blocks[i]->seth(gTileSize);
				levelData.blocks[i]->setx(posx*gTileSize);
				levelData.blocks[i]->sety(posy*gTileSize);
				levelData.blocks[i]->fill();
				break;
			case '0':
			default:;
		}
		posx++;
#ifdef DEBUG
 		cout<<'('<<posx<<", "<<posy<<')'<<ch;
#endif

		}
	}
	cout<<"W: "<<levelData.w<<endl;
	cout<<"H: "<<levelData.h<<endl;

	file.close();
	for(int i=0;i<entitiesInfo.size();i++){
		if(entities==NULL){
			entities=(entity**)malloc(sizeof(entity*)*entitiesInfo.size());
		}
		entities[i]=new entity(entitiesInfo[i].w,entitiesInfo[i].h,entitiesInfo[i].x*gTileSize,entitiesInfo[i].y*gTileSize);
		entities[i]->setTexturePath("data/graphics/entities/enemies/red.png");
		numOfEntities++;
		cout<<"entx"<<entities[i]->getx()<<endl;
		cout<<"enty"<<entities[i]->gety()<<endl;
	}
}

level::level(string level)
{
	getData(level);
	/*
	int width=1000;
	int height=1000;
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

	blocks[7]->setw(gTileSize * 4);
	blocks[7]->seth(gTileSize * 1);
	blocks[7]->setCol(0xDD,0xDD,0xDD);
	blocks[7]->setx(0);
	blocks[7]->sety(blocks[5]->gety() - 5*gTileSize);
	blocks[7]->fill();
	*/
	blocks=levelData.blocks;
	numOfBlocks=levelData.numOfBlocks;

	bgTexture = NULL;
	w = levelData.w, h = levelData.h;
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
