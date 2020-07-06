#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "ui.h"
#include <iostream>
using namespace std;

extern int screenwidth;
extern int screenheight;
extern SDL_Renderer* mRenderer;

ui::ui(string foreGroundPath, string scoreDisplayPath)
{
	foreGroundTexture = NULL;

	SDL_Surface* surface = NULL;
	surface = IMG_Load(foreGroundPath.c_str());
	if (surface == NULL)
	{
		cout << IMG_GetError() << endl;
	}
	else
	{
		foreGroundTexture = SDL_CreateTextureFromSurface(mRenderer, surface);
		if (foreGroundTexture == NULL)
		{
			cout << SDL_GetError() << endl;
		}
		SDL_FreeSurface(surface);
	}
	surface = NULL;
}

ui::~ui()
{
	close();
}

void ui::close()
{
	SDL_DestroyTexture(foreGroundTexture);
}

void ui::render(int score)
{
	SDL_Surface *surface = NULL;
	char *pathLeft, *pathRight;
	static int lastScore = 0;
	static bool zeroFlag = 1;
	if (lastScore != score || zeroFlag) {
	if( score < 10 )
	{
	pathLeft = paths[0];
	pathRight= paths[score];
	} else {
		if( score - 10 < 10 ) {
			pathLeft= paths[1];
			pathRight=paths[score-10];
		}
		else if( score - 20 < 20 ) {
			pathLeft= paths[2];
			pathRight=paths[score-20];
		}
		else if( score - 30 < 30 ) {
			pathLeft= paths[3];
			pathRight=paths[score-30];
		}
		else if( score - 40 < 40 ) {
			pathLeft= paths[4];
			pathRight=paths[score-40];
		}
		else if( score - 50 < 50 ) {
			pathLeft= paths[5];
			pathRight=paths[score-50];
		}
		else if( score - 60 < 60 ) {
			pathLeft= paths[6];
			pathRight=paths[score-60];
		}
		else if( score - 70 < 70 ) {
			pathLeft= paths[7];
			pathRight=paths[score-70];
		}
		else if( score - 80 < 80 ) {
			pathLeft= paths[8];
			pathRight=paths[score-80];
		}
		else if( score - 90 < 90 ) {
			pathLeft= paths[9];
			pathRight=paths[score-90];
		}
	}
	scoreDisplayTextureLeft = SDL_CreateTextureFromSurface(mRenderer, (surface = IMG_Load(pathLeft)) );
	SDL_FreeSurface(surface);
	scoreDisplayTextureRight = SDL_CreateTextureFromSurface(mRenderer, (surface = IMG_Load(pathRight)) );
	SDL_FreeSurface(surface);
	lastScore = score;
	zeroFlag = 0;
	}
	SDL_RenderCopy(mRenderer, foreGroundTexture, NULL, NULL);
	SDL_Rect rectLeft = {0,0,screenwidth/21,screenheight/16};
	SDL_Rect rectRight= {screenwidth/21,0,screenwidth/21,screenheight/16};
	SDL_RenderCopy(mRenderer, scoreDisplayTextureLeft, 0, &rectLeft);
	SDL_RenderCopy(mRenderer, scoreDisplayTextureRight, 0, &rectRight);
}
int ui::points()
{
	return score;
}
void ui::scoreup() {
	score++;
}
void ui::inputScore(int num) {
	score = num;
}
